//
// Created by Olcay Taner YILDIZ on 18.02.2023.
//

#ifndef HMM_HMM2_H
#define HMM_HMM2_H

#include <Matrix.h>
#include <HashMap/HashMap.h>
#include <HashMap/HashSet.h>

struct hmm2 {
    Matrix_ptr transition_probabilities;
    Hash_map_ptr state_indexes;
    Array_list_ptr states;
    int state_count;
    Matrix_ptr pi;
};

typedef struct hmm2 Hmm2;
typedef Hmm2 *Hmm2_ptr;

Hmm2_ptr create_hmm2(const Hash_set* states,
                     int observation_count,
                     const Array_list_ptr *observations,
                     const Array_list_ptr *emitted_symbols,
                     unsigned int hash_function_state(const void *number, int N),
                     int compare_function_state(const void *first, const void *second),
                     void* clone_state(void* state),
                     unsigned int hash_function_symbol(const void *number, int N),
                     int compare_function_symbol(const void *first, const void *second),
                     void* clone_symbol(void* symbol));

void free_hmm2(Hmm2_ptr hmm);

Vector_ptr log_of_column_hmm2(const Hmm2* hmm, int column);

void calculate_pi_hmm2(Hmm2_ptr hmm,
                       int observation_count,
                       const Array_list_ptr *observations);

void calculate_transition_probabilities_hmm2(Hmm2_ptr hmm,
                                             int observation_count,
                                             const Array_list_ptr *observations);

Array_list_ptr viterbi_hmm2(const Hmm2* hmm, const Array_list* s);

#endif //HMM_HMM2_H
