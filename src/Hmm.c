//
// Created by Olcay Taner YILDIZ on 17.02.2023.
//

#include <math.h>
#include <limits.h>
#include <CounterHashMap.h>
#include <stdlib.h>
#include "Hmm.h"
#include "Memory/Memory.h"

/**
 * safeLog calculates the logarithm of a number. If the number is less than 0, the logarithm is not defined, therefore
 * the function returns -Infinity.
 *
 * @param x Input number
 * @return the logarithm of x. If x < 0 return -infinity.
 */
double safe_log(double x) {
    if (x <= 0) {
        return INT_MIN;
    } else {
        return log(x);
    }
}

/**
 * A constructor of Hmm class which takes a Set of states, an array of observations (which also
 * consists of an array of states) and an array of instances (which also consists of an array of emitted symbols).
 * The constructor initializes the state array with the set of states and uses observations and emitted symbols
 * to calculate the emission probabilities for those states.
 *
 * @param states A Set of states, consisting of all possible states for this problem.
 * @param observations An array of instances, where each instance consists of an array of states.
 * @param emittedSymbols An array of instances, where each instance consists of an array of symbols.
 */
Hash_map_ptr calculate_emission_probabilities(const void *state,
                                              int observation_count,
                                              const Array_list_ptr *observations,
                                              const Array_list_ptr *emitted_symbols,
                                              int compare_function_state(const void *first, const void *second),
                                              unsigned int hash_function_symbol(const void *number, int N),
                                              int compare_function_symbol(const void *first, const void *second),
                                              void* clone_symbol(void* symbol)) {
    Counter_hash_map_ptr counts;
    Hash_map_ptr emission_probabilities;
    void *current_state;
    void *current_symbol;
    counts = create_counter_hash_map(hash_function_symbol, compare_function_symbol);
    for (int i = 0; i < observation_count; i++) {
        for (int j = 0; j < observations[i]->size; j++) {
            current_state = array_list_get(observations[i], j);
            current_symbol = array_list_get(emitted_symbols[i], j);
            if (compare_function_state(current_state, state) == 0) {
                put_counter_hash_map(counts, current_symbol);
            }
        }
    }
    emission_probabilities = create_hash_map(hash_function_symbol, compare_function_symbol);
    double sum = sum_of_counts_counter_hash_map(counts);
    Array_list_ptr list = key_value_list(counts->map);
    for (int i = 0; i < list->size; i++) {
        Hash_node_ptr hash_node = array_list_get(list, i);
        double *p = malloc_(sizeof(double), "calculate_emission_probabilities");
        *p = *((int *) hash_node->value) / sum;
        hash_map_insert(emission_probabilities, clone_symbol(hash_node->key), p);
    }
    free_array_list(list, NULL);
    free_counter_hash_map(counts);
    return emission_probabilities;
}
