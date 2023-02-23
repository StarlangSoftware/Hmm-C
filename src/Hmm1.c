//
// Created by Olcay Taner YILDIZ on 16.02.2023.
//

#include <stdlib.h>
#include "Hmm1.h"
#include "HmmState.h"
#include "Hmm.h"

/**
 * A constructor of {@link Hmm1} class which takes a {@link Set} of states, an array of observations (which also
 * consists of an array of states) and an array of instances (which also consists of an array of emitted symbols).
 * The constructor calls its super method to calculate the emission probabilities for those states.
 *
 * @param states A {@link Set} of states, consisting of all possible states for this problem.
 * @param observations An array of instances, where each instance consists of an array of states.
 * @param emittedSymbols An array of instances, where each instance consists of an array of symbols.
 */
Hmm1_ptr
create_hmm1(Hash_set_ptr states,
            int observation_count,
            Array_list_ptr *observations,
            Array_list_ptr *emitted_symbols,
            unsigned int hash_function_state(void *number, int N),
            int compare_function_state(void *first, void *second),
            unsigned int hash_function_symbol(void *number, int N),
            int compare_function_symbol(void *first, void *second)) {
    Hmm1_ptr result;
    result = malloc(sizeof(Hmm1));
    result->state_count = states->hash_map->count;
    result->state_indexes = create_hash_map(hash_function_state, compare_function_state);
    result->states = create_array_list();
    Array_list_ptr state_list = hash_set_key_list(states);
    for (int i = 0; i < state_list->size; i++) {
        int *index;
        index = malloc(sizeof(int));
        *index = i;
        hash_map_insert(result->state_indexes, array_list_get(state_list, i), index);
    }
    calculate_pi_hmm1(result, observation_count, observations);
    for (int i = 0; i < state_list->size; i++) {
        Hash_map_ptr emission_probabilities = calculate_emission_probabilities(array_list_get(state_list, i),
                                                                               observation_count,
                                                                               observations,
                                                                               emitted_symbols,
                                                                               hash_function_symbol,
                                                                               compare_function_symbol);
        array_list_add(result->states, create_hmm_state(array_list_get(state_list, i),
                                                        emission_probabilities));
    }
    free_array_list(state_list, NULL);
    calculate_transition_probabilities_hmm1(result, observation_count, observations);
    return result;
}

void free_hmm1(Hmm1_ptr hmm) {
    free_matrix(hmm->transition_probabilities);
    free_hash_map(hmm->state_indexes, free);
    free_array_list(hmm->states, (void (*)(void *)) free_hmm_state);
    free_vector(hmm->pi);
    free(hmm);
}

/**
 * logOfColumn calculates the logarithm of each value in a specific column in the transition probability matrix.
 *
 * @param column Column index of the transition probability matrix.
 * @return A vector consisting of the logarithm of each value in the column in the transition probability matrix.
 */
Vector_ptr log_of_column_hmm1(Hmm1_ptr hmm, int column) {
    Vector_ptr result;
    int i;
    result = create_vector2(0, 0.0);
    for (i = 0; i < hmm->state_count; i++) {
        add_value_to_vector(result, safe_log(hmm->transition_probabilities->values[i][column]));
    }
    return result;
}

/**
 * calculatePi calculates the prior probability vector (initial probabilities for each state) from a set of
 * observations. For each observation, the function extracts the first state in that observation. Normalizing the
 * counts of the states returns us the prior probabilities for each state.
 *
 * @param observations A set of observations used to calculate the prior probabilities.
 */
void calculate_pi_hmm1(Hmm1_ptr hmm,
                       int observation_count,
                       Array_list_ptr *observations) {
    hmm->pi = create_vector2(hmm->state_count, 0.0);
    for (int i = 0; i < observation_count; i++) {
        Array_list_ptr observation = observations[i];
        int *index = hash_map_get(hmm->state_indexes, array_list_get(observation, 0));
        add_value(hmm->pi, *index, 1.0);
    }
    l1_normalize(hmm->pi);
}

/**
 * calculateTransitionProbabilities calculates the transition probabilities matrix from each state to another state.
 * For each observation and for each transition in each observation, the function gets the states. Normalizing the
 * counts of the pair of states returns us the transition probabilities.
 *
 * @param observations A set of observations used to calculate the transition probabilities.
 */
void calculate_transition_probabilities_hmm1(Hmm1_ptr hmm,
                                             int observation_count,
                                             Array_list_ptr *observations) {
    hmm->transition_probabilities = create_matrix(hmm->state_count, hmm->state_count);
    for (int i = 0; i < observation_count; i++) {
        Array_list_ptr current = observations[i];
        for (int j = 0; j < current->size - 1; j++) {
            int *from = hash_map_get(hmm->state_indexes, array_list_get(current, j));
            int *to = hash_map_get(hmm->state_indexes, array_list_get(current, j + 1));
            increment(hmm->transition_probabilities, *from, *to);
        }
    }
    column_wise_normalize(hmm->transition_probabilities);
}

/**
 * viterbi calculates the most probable state sequence for a set of observed symbols.
 *
 * @param s A set of observed symbols.
 * @return The most probable state sequence as an {@link ArrayList}.
 */
Array_list_ptr viterbi_hmm1(Hmm1_ptr hmm, Array_list_ptr s) {
    void *emission;
    Array_list_ptr result;
    int sequenceLength = s->size, maxIndex;
    Matrix_ptr gamma = create_matrix(sequenceLength, hmm->state_count);
    int *qs = calloc(sequenceLength, sizeof(int));
    double observationLikelihood;
    Matrix_ptr phi = create_matrix(sequenceLength, hmm->state_count);
    /*Initialize*/
    emission = array_list_get(s, 0);
    for (int i = 0; i < hmm->state_count; i++) {
        observationLikelihood = get_prob(((Hmm_state_ptr) array_list_get(hmm->states, i)), emission);
        gamma->values[0][i] = safe_log(get_value(hmm->pi, i)) + safe_log(observationLikelihood);
    }
    /*Iterate Dynamic Programming*/
    for (int t = 1; t < sequenceLength; t++) {
        emission = array_list_get(s, t);
        for (int j = 0; j < hmm->state_count; j++) {
            Vector_ptr tempArray = log_of_column_hmm1(hmm, j);
            add_vector(tempArray, get_row(gamma, t - 1));
            maxIndex = max_index_of_vector(tempArray);
            observationLikelihood = get_prob(((Hmm_state_ptr) array_list_get(hmm->states, j)), emission);
            gamma->values[t][j] = get_value(tempArray, maxIndex) + safe_log(observationLikelihood);
            free_vector(tempArray);
            phi->values[t][j] = maxIndex;
        }
    }
    /*Backtrack pointers*/
    result = create_array_list();
    qs[sequenceLength - 1] = max_index_of_vector(get_row(gamma, sequenceLength - 1));
    array_list_insert(result, 0, ((Hmm_state_ptr) array_list_get(hmm->states, qs[sequenceLength - 1]))->state);
    for (int i = sequenceLength - 2; i >= 0; i--) {
        qs[i] = phi->values[i + 1][qs[i + 1]];
        array_list_insert(result, 0, ((Hmm_state_ptr) array_list_get(hmm->states, qs[i]))->state);
    }
    free_matrix(gamma);
    free_matrix(phi);
    free(qs);
    return result;
}

