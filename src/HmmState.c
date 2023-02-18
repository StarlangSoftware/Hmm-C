//
// Created by Olcay Taner YILDIZ on 16.02.2023.
//

#include <stdlib.h>
#include "HmmState.h"

/**
 * A constructor of {@link HmmState} class which takes a {@link State} and emission probabilities as inputs and
 * initializes corresponding class variable with these inputs.
 *
 * @param state Data for this state.
 * @param emissionProbabilities Emission probabilities for this state
 */
Hmm_state_ptr create_hmm_state(void *state, Hash_map_ptr emission_probabilities) {
    Hmm_state_ptr result = malloc(sizeof(Hmm_state));
    result->state = state;
    result->emission_probabilities = emission_probabilities;
    return result;
}

void free_hmm_state(Hmm_state_ptr hmm_state) {
    free(hmm_state);
}

/**
 * getEmitProb method returns the emission probability for a specific symbol.
 *
 * @param symbol Symbol for which the emission probability will be get.
 * @return Emission probability for a specific symbol.
 */
double get_prob(Hmm_state_ptr hmm_state, void *symbol) {
    if (hash_map_contains(hmm_state->emission_probabilities, symbol)){
        return *(double*) hash_map_get(hmm_state->emission_probabilities, symbol);
    } else {
        return 0;
    }
}
