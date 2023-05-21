//
// Created by Olcay Taner YILDIZ on 17.02.2023.
//

#ifndef HMM_HMM_H
#define HMM_HMM_H

#include <HashMap/HashMap.h>

double safe_log(double x);

Hash_map_ptr calculate_emission_probabilities(const void *state,
                                              int observation_count,
                                              const Array_list_ptr *observations,
                                              const Array_list_ptr *emitted_symbols,
                                              unsigned int hash_function_symbol(const void *number, int N),
                                              int compare_function_symbol(const void *first, const void *second));

#endif //HMM_HMM_H
