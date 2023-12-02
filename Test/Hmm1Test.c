//
// Created by Olcay Taner YILDIZ on 18.02.2023.
//

#include <HashMap/HashSet.h>
#include <string.h>
#include <stdio.h>
#include <Memory/Memory.h>
#include <StringUtils.h>
#include "../src/Hmm1.h"

Array_list_ptr create_array_list2(char* array[], int size){
    Array_list_ptr result = create_array_list();
    for (int i = 0; i < size; i++){
        array_list_add(result, clone_string(array[i]));
    }
    return result;
}

int main() {
    Hash_set_ptr states = create_hash_set((unsigned int (*)(const void *, int)) hash_function_string,
                                          (int (*)(const void *, const void *)) compare_string);
    hash_set_insert(states, clone_string("HOT"));
    hash_set_insert(states, clone_string("COLD"));
    Array_list_ptr *observations;
    observations = malloc_(5 * sizeof(Array_list_ptr), "main_1");
    observations[0] = create_array_list2((char*[]){"HOT", "HOT", "HOT"}, 3);
    observations[1] = create_array_list2((char*[]){"HOT", "COLD", "COLD", "COLD"}, 4);
    observations[2] = create_array_list2((char*[]){"HOT", "COLD", "HOT", "COLD"}, 4);
    observations[3] = create_array_list2((char*[]){"COLD", "COLD", "COLD", "HOT", "HOT"}, 5);
    observations[4] = create_array_list2((char*[]){"COLD", "HOT", "HOT", "COLD", "COLD"}, 5);
    Array_list_ptr *emittedSymbols;
    emittedSymbols = malloc_(5 * sizeof(Array_list_ptr), "main_2");
    emittedSymbols[0] = create_array_list_of_int((int[]){3, 2, 3}, 3);
    emittedSymbols[1] = create_array_list_of_int((int[]){2, 2, 1, 1}, 4);
    emittedSymbols[2] = create_array_list_of_int((int[]){3, 1, 2, 1}, 4);
    emittedSymbols[3] = create_array_list_of_int((int[]){3, 1, 2, 2, 3}, 5);
    emittedSymbols[4] = create_array_list_of_int((int[]){1, 2, 3, 2, 1}, 5);
    Hmm1_ptr hmm = create_hmm1(states,
                               5,
                               observations,
                               emittedSymbols,
                               (unsigned int (*)(const void *, int)) hash_function_string,
                               (int (*)(const void *, const void *)) compare_string,
                               (void *(*)(void *)) clone_string,
                               (unsigned int (*)(const void *, int)) hash_function_int,
                               (int (*)(const void *, const void *)) compare_int,
                               (void *(*)(void *)) clone_string);
    free_hash_set(states, free_);
    for (int i = 0; i < 5; i++){
        free_array_list(observations[i], free_);
        free_array_list(emittedSymbols[i], free_);
    }
    free_(observations);
    free_(emittedSymbols);
    Array_list_ptr observed = create_array_list_of_int((int[]){1, 1, 1, 1, 1, 1}, 6);
    Array_list_ptr observedStates = viterbi_hmm1(hmm, observed);
    free_array_list(observed, free_);
    if (strcmp("COLD", array_list_get(observedStates, 0)) != 0) {
        printf("Error in test Hmm1Test 1\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 1)) != 0) {
        printf("Error in test Hmm1Test 2\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 2)) != 0) {
        printf("Error in test Hmm1Test 3\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 3)) != 0) {
        printf("Error in test Hmm1Test 4\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 4)) != 0) {
        printf("Error in test Hmm1Test 5\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 5)) != 0) {
        printf("Error in test Hmm1Test 6\n");
    }
    free_array_list(observedStates, NULL);
    observed = create_array_list_of_int((int[]){1, 2, 3, 3, 2, 1}, 6);
    observedStates = viterbi_hmm1(hmm, observed);
    free_array_list(observed, free_);
    if (strcmp("COLD", array_list_get(observedStates, 0)) != 0) {
        printf("Error in test Hmm1Test 7\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 1)) != 0) {
        printf("Error in test Hmm1Test 8\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 2)) != 0) {
        printf("Error in test Hmm1Test 9\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 3)) != 0) {
        printf("Error in test Hmm1Test 10\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 4)) != 0) {
        printf("Error in test Hmm1Test 11\n");
    }
    if (strcmp("COLD", array_list_get(observedStates, 5)) != 0) {
        printf("Error in test Hmm1Test 12\n");
    }
    free_array_list(observedStates, NULL);
    observed = create_array_list_of_int((int[]){3, 3, 3, 3, 3, 3}, 6);
    observedStates = viterbi_hmm1(hmm, observed);
    free_array_list(observed, free_);
    if (strcmp("HOT", array_list_get(observedStates, 0)) != 0) {
        printf("Error in test Hmm1Test 13\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 1)) != 0) {
        printf("Error in test Hmm1Test 14\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 2)) != 0) {
        printf("Error in test Hmm1Test 15\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 3)) != 0) {
        printf("Error in test Hmm1Test 16\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 4)) != 0) {
        printf("Error in test Hmm1Test 17\n");
    }
    if (strcmp("HOT", array_list_get(observedStates, 5)) != 0) {
        printf("Error in test Hmm1Test 18\n");
    }
    free_array_list(observedStates, NULL);
    free_hmm1(hmm);
}