//
// Created by Olcay Taner YILDIZ on 17.02.2023.
//

#ifndef HMM_HMM_H
#define HMM_HMM_H

#include <HashMap/HashMap.h>

double safe_log(double x);

Hash_map_ptr calculate_emission_probabilities(void *state,
                                              int observation_count,
                                              Array_list_ptr *observations,
                                              Array_list_ptr *emitted_symbols,
                                              unsigned int hash_function_symbol(void *number, int N),
                                              int compare_function_symbol(void *first, void *second));

#endif //HMM_HMM_H
