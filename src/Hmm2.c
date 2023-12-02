//
// Created by Olcay Taner YILDIZ on 18.02.2023.
//

#include <Memory/Memory.h>
#include "Hmm2.h"
#include "Hmm.h"
#include "HmmState.h"

/**
 * A constructor of {@link Hmm2} class which takes a Set of states, an array of observations (which also
 * consists of an array of states) and an array of instances (which also consists of an array of emitted symbols).
 * The constructor calls its super method to calculate the emission probabilities for those states.
 *
 * @param states A Set of states, consisting of all possible states for this problem.
 * @param observations An array of instances, where each instance consists of an array of states.
 * @param emittedSymbols An array of instances, where each instance consists of an array of symbols.
 */
Hmm2_ptr
create_hmm2(const Hash_set* states,
            int observation_count,
            const Array_list_ptr *observations,
            const Array_list_ptr *emitted_symbols,
            unsigned int hash_function_state(const void *number, int N),
            int compare_function_state(const void *first, const void *second),
            void* clone_state(void* state),
            unsigned int (*hash_function_symbol)(const void *, int),
            int (*compare_function_symbol)(const void *, const void *),
            void* clone_symbol(void* symbol)) {
    Hmm2_ptr result;
    result = malloc_(sizeof(Hmm2), "create_hmm2_1");
    result->state_count = states->hash_map->count;
    result->state_indexes = create_hash_map(hash_function_state, compare_function_state);
    result->states = create_array_list();
    Array_list_ptr state_list = hash_set_key_list(states);
    for (int i = 0; i < state_list->size; i++) {
        int *index;
        index = malloc_(sizeof(int), "create_hmm2_2");
        *index = i;
        hash_map_insert(result->state_indexes, clone_state(array_list_get(state_list, i)), index);
    }
    calculate_pi_hmm2(result, observation_count, observations);
    for (int i = 0; i < state_list->size; i++) {
        Hash_map_ptr emission_probabilities = calculate_emission_probabilities(array_list_get(state_list, i),
                                                                               observation_count,
                                                                               observations,
                                                                               emitted_symbols,
                                                                               compare_function_state,
                                                                               hash_function_symbol,
                                                                               compare_function_symbol,
                                                                               clone_symbol);
        array_list_add(result->states, create_hmm_state(clone_state(array_list_get(state_list, i)),
                                                        emission_probabilities));
    }
    free_array_list(state_list, NULL);
    calculate_transition_probabilities_hmm2(result, observation_count, observations);
    return result;
}

void free_hmm2(Hmm2_ptr hmm) {
    free_matrix(hmm->transition_probabilities);
    free_hash_map2(hmm->state_indexes, free_, free_);
    free_array_list(hmm->states, (void (*)(void *)) free_hmm_state);
    free_matrix(hmm->pi);
    free_(hmm);
}

/**
 * logOfColumn calculates the logarithm of each value in a specific column in the transition probability matrix.
 *
 * @param column Column index of the transition probability matrix.
 * @return A vector consisting of the logarithm of each value in the column in the transition probability matrix.
 */
Vector_ptr log_of_column_hmm2(const Hmm2* hmm, int column) {
    Vector_ptr result;
    int i;
    result = create_vector2(0, 0.0);
    for (i = 0; i < hmm->state_count; i++) {
        add_value_to_vector(result, safe_log(
                hmm->transition_probabilities->values[i * hmm->state_count + column / hmm->state_count][column %
                                                                                                        hmm->state_count]));
    }
    return result;
}

/**
 * calculatePi calculates the prior probability matrix (initial probabilities for each state combinations)
 * from a set of observations. For each observation, the function extracts the first and second states in
 * that observation.  Normalizing the counts of the pair of states returns us the prior probabilities for each
 * pair of states.
 *
 * @param observations A set of observations used to calculate the prior probabilities.
 */
void calculate_pi_hmm2(Hmm2_ptr hmm, int observation_count, const Array_list_ptr *observations) {
    hmm->pi = create_matrix(hmm->state_count, hmm->state_count);
    for (int i = 0; i < observation_count; i++) {
        Array_list_ptr observation = observations[i];
        int *first = hash_map_get(hmm->state_indexes, array_list_get(observation, 0));
        int *second = hash_map_get(hmm->state_indexes, array_list_get(observation, 1));
        increment(hmm->pi, *first, *second);
    }
    column_wise_normalize(hmm->pi);
}

/**
 * calculateTransitionProbabilities calculates the transition probabilities matrix from each state to another state.
 * For each observation and for each transition in each observation, the function gets the states. Normalizing the
 * counts of the three of states returns us the transition probabilities.
 *
 * @param observations A set of observations used to calculate the transition probabilities.
 */
void calculate_transition_probabilities_hmm2(Hmm2_ptr hmm, int observation_count, const Array_list_ptr *observations) {
    hmm->transition_probabilities = create_matrix(hmm->state_count * hmm->state_count, hmm->state_count);
    for (int i = 0; i < observation_count; i++) {
        Array_list_ptr current = observations[i];
        for (int j = 0; j < current->size - 2; j++) {
            int *from1 = hash_map_get(hmm->state_indexes, array_list_get(current, j));
            int *from2 = hash_map_get(hmm->state_indexes, array_list_get(current, j + 1));
            int *to = hash_map_get(hmm->state_indexes, array_list_get(current, j + 2));
            increment(hmm->transition_probabilities, (*from1) * hmm->state_count + (*from2), *to);
        }
    }
    column_wise_normalize(hmm->transition_probabilities);
}

/**
 * viterbi calculates the most probable state sequence for a set of observed symbols.
 *
 * @param s A set of observed symbols.
 * @return The most probable state sequence as an ArrayList.
 */
Array_list_ptr viterbi_hmm2(const Hmm2* hmm, const Array_list* s) {
    void *emission, *emission1, *emission2;
    Array_list_ptr result;
    int sequenceLength = s->size, maxIndex;
    Matrix_ptr gamma = create_matrix(sequenceLength, hmm->state_count * hmm->state_count);
    int *qs = calloc_(sequenceLength, sizeof(int), "viterbi_hmm2");
    double observationLikelihood;
    Matrix_ptr phi = create_matrix(sequenceLength, hmm->state_count * hmm->state_count);
    /*Initialize*/
    emission1 = array_list_get(s, 0);
    emission2 = array_list_get(s, 1);
    for (int i = 0; i < hmm->state_count; i++) {
        for (int j = 0; j < hmm->state_count; j++) {
            observationLikelihood = get_prob(((Hmm_state_ptr) array_list_get(hmm->states, i)), emission1) *
                                    get_prob(((Hmm_state_ptr) array_list_get(hmm->states, j)), emission2);
            gamma->values[1][i * hmm->state_count + j] =
                    safe_log(hmm->pi->values[i][j]) + safe_log(observationLikelihood);
        }
    }
    /*Iterate Dynamic Programming*/
    for (int t = 2; t < sequenceLength; t++) {
        emission = array_list_get(s, t);
        for (int j = 0; j < hmm->state_count * hmm->state_count; j++) {
            Vector_ptr current = log_of_column_hmm2(hmm, j);
            Vector_ptr temp = get_row(gamma, t - 1);
            Vector_ptr previous = skip_vector(temp, hmm->state_count, j / hmm->state_count);
            free_vector(temp);
            add_vector(current, previous);
            free_vector(previous);
            maxIndex = max_index_of_vector(current);
            observationLikelihood = get_prob(((Hmm_state_ptr) array_list_get(hmm->states, j % hmm->state_count)),
                                             emission);
            gamma->values[t][j] = get_value(current, maxIndex) + safe_log(observationLikelihood);
            free_vector(current);
            phi->values[t][j] = maxIndex * hmm->state_count + j / hmm->state_count;
        }
    }
    /*Backtrack pointers*/
    result = create_array_list();
    Vector_ptr row_vector = get_row(gamma, sequenceLength - 1);
    qs[sequenceLength - 1] = max_index_of_vector(row_vector);
    free_vector(row_vector);
    array_list_insert(result, 0,
                      ((Hmm_state_ptr) array_list_get(hmm->states, qs[sequenceLength - 1] % hmm->state_count))->state);
    for (int i = sequenceLength - 2; i >= 1; i--) {
        qs[i] = phi->values[i + 1][qs[i + 1]];
        array_list_insert(result, 0, ((Hmm_state_ptr) array_list_get(hmm->states, qs[i] % hmm->state_count))->state);
    }
    array_list_insert(result, 0, ((Hmm_state_ptr) array_list_get(hmm->states, qs[1] / hmm->state_count))->state);
    free_matrix(gamma);
    free_matrix(phi);
    free_(qs);
    return result;
}
