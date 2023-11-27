//
// Created by Olcay Taner YILDIZ on 18.02.2023.
//

#include <HashMap/HashSet.h>
#include <string.h>
#include <stdio.h>
#include <Memory/Memory.h>
#include "../src/Hmm2.h"

int main() {
    char *array[] = {"HOT", "COLD"};
    Hash_set_ptr states = create_hash_set_of_string(array, 2);
    Array_list_ptr *observations;
    observations = malloc_(5 * sizeof(Array_list_ptr), "main_1");
    char *array0[] = {"HOT", "HOT", "HOT"};
    char *array1[] = {"HOT", "COLD", "COLD", "COLD"};
    char *array2[] = {"HOT", "COLD", "HOT", "COLD"};
    char *array3[] = {"COLD", "COLD", "COLD", "HOT", "HOT"};
    char *array4[] = {"COLD", "HOT", "HOT", "COLD", "COLD"};
    observations[0] = create_array_list_of_string(array0, 3);
    observations[1] = create_array_list_of_string(array1, 4);
    observations[2] = create_array_list_of_string(array2, 4);
    observations[3] = create_array_list_of_string(array3, 5);
    observations[4] = create_array_list_of_string(array4, 5);
    Array_list_ptr *emittedSymbols;
    emittedSymbols = malloc_(5 * sizeof(Array_list_ptr), "main_2");
    int int_array0[] = {3, 2, 3};
    int int_array1[] = {2, 2, 1, 1};
    int int_array2[] = {3, 1, 2, 1};
    int int_array3[] = {3, 1, 2, 2, 3};
    int int_array4[] = {1, 2, 3, 2, 1};
    emittedSymbols[0] = create_array_list_of_int(int_array0, 3);
    emittedSymbols[1] = create_array_list_of_int(int_array1, 4);
    emittedSymbols[2] = create_array_list_of_int(int_array2, 4);
    emittedSymbols[3] = create_array_list_of_int(int_array3, 5);
    emittedSymbols[4] = create_array_list_of_int(int_array4, 5);
    Hmm2_ptr hmm = create_hmm2(states,
                               5,
                               observations,
                               emittedSymbols,
                               (unsigned int (*)(const void *, int)) hash_function_string,
                               (int (*)(const void *, const void *)) compare_string,
                               (unsigned int (*)(const void *, int)) hash_function_int,
                               (int (*)(const void *, const void *)) compare_int);
    for (int i = 0; i < 5; i++){
        free_array_list(observations[i], NULL);
        free_array_list(emittedSymbols[i], NULL);
    }
    free_(observations);
    free_(emittedSymbols);
    int observed0[] = {1, 1, 1, 1, 1, 1};
    Array_list_ptr observed = create_array_list_of_int(observed0, 6);
    Array_list_ptr observedStates = viterbi_hmm2(hmm, observed);
    free_array_list(observed, NULL);
    if (strcmp("COLD", array_list_get(observedStates, 0)) != 0) {
        printf("Error in test Hmm2Test 1\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 1)) != 0) {
        printf("Error in test Hmm2Test 2\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 2)) != 0) {
        printf("Error in test Hmm2Test 3\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 3)) != 0) {
        printf("Error in test Hmm2Test 4\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 4)) != 0) {
        printf("Error in test Hmm2Test 5\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 5)) != 0) {
        printf("Error in test Hmm2Test 6\n");
    }
    free_array_list(observedStates, NULL);
    int observed1[] = {1, 2, 3, 3, 2, 1};
    observed = create_array_list_of_int(observed1, 6);
    observedStates = viterbi_hmm2(hmm, observed);
    free_array_list(observed, NULL);
    if (strcmp("COLD", array_list_get(observedStates, 0)) != 0) {
        printf("Error in test Hmm2Test 7\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 1)) != 0) {
        printf("Error in test Hmm2Test 8\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 2)) != 0) {
        printf("Error in test Hmm2Test 9\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 3)) != 0) {
        printf("Error in test Hmm2Test 10\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 4)) != 0) {
        printf("Error in test Hmm2Test 11\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 5)) != 0) {
        printf("Error in test Hmm2Test 12\n");
    }
    free_array_list(observedStates, NULL);
    int observed2[] = {3, 3, 3, 3, 3, 3};
    observed = create_array_list_of_int(observed2, 6);
    observedStates = viterbi_hmm2(hmm, observed);
    free_array_list(observed, NULL);
    if (strcmp("HOT", array_list_get(observedStates, 0)) != 0) {
        printf("Error in test Hmm2Test 13\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 1)) != 0) {
        printf("Error in test Hmm2Test 14\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 2)) != 0) {
        printf("Error in test Hmm2Test 15\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 3)) != 0) {
        printf("Error in test Hmm2Test 16\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 4)) != 0) {
        printf("Error in test Hmm2Test 17\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 5)) != 0) {
        printf("Error in test Hmm2Test 18\n");
    }
    free_array_list(observedStates, NULL);
    free_hmm2(hmm);
    free_hash_set(states, NULL);
}