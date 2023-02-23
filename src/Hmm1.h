//
// Created by Olcay Taner YILDIZ on 16.02.2023.
//

#ifndef HMM_HMM1_H
#define HMM_HMM1_H

#include <HashMap/HashMap.h>
#include <HashMap/HashSet.h>
#include "Matrix.h"

struct hmm1 {
    Matrix_ptr transition_probabilities;
    Hash_map_ptr state_indexes;
    Array_list_ptr states;
    int state_count;
    Vector_ptr pi;
};

typedef struct hmm1 Hmm1;
typedef Hmm1 *Hmm1_ptr;

Hmm1_ptr create_hmm1(Hash_set_ptr states,
                     int observation_count,
                     Array_list_ptr *observations,
                     Array_list_ptr *emitted_symbols,
                     unsigned int hash_function_state(void *number, int N),
                     int compare_function_state(void *first, void *second),
                     unsigned int hash_function_symbol(void *number, int N),
                     int compare_function_symbol(void *first, void *second));

void free_hmm1(Hmm1_ptr hmm);

Vector_ptr log_of_column_hmm1(Hmm1_ptr hmm, int column);

void calculate_pi_hmm1(Hmm1_ptr hmm,
                       int observation_count,
                       Array_list_ptr *observations);

void calculate_transition_probabilities_hmm1(Hmm1_ptr hmm,
                                             int observation_count,
                                             Array_list_ptr *observations);

Array_list_ptr viterbi_hmm1(Hmm1_ptr hmm, Array_list_ptr s);

#endif //HMM_HMM1_H
