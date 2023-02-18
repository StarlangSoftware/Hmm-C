//
// Created by Olcay Taner YILDIZ on 16.02.2023.
//

#ifndef HMM_HMMSTATE_H
#define HMM_HMMSTATE_H

#include <HashMap/HashMap.h>

struct hmm_state{
    Hash_map_ptr emission_probabilities;
    void* state;
};

typedef struct hmm_state Hmm_state;
typedef Hmm_state* Hmm_state_ptr;

Hmm_state_ptr create_hmm_state(void* state, Hash_map_ptr emission_probabilities);
void free_hmm_state(Hmm_state_ptr hmm_state);
double get_prob(Hmm_state_ptr hmm_state, void* symbol);

#endif //HMM_HMMSTATE_H
