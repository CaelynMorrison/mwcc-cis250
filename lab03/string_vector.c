#include <string_vector.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

StringVector* vector_create(size_t initial_capacity) {
    StringVector *new_vector = (StringVector*)malloc(sizeof(StringVector));
    if (new_vector == NULL) {
        return NULL;
    }
    new_vector->data = (char**)malloc(initial_capacity * sizeof(char));
    if (new_vector->data == NULL) {
        return NULL;
    }
    new_vector->capacity = initial_capacity;
    new_vector->size = 0;
    return new_vector;
}

int vector_push(StringVector *vec, const char *str){
    if (vec->size == vec->capacity) {
        vec = realloc(vec, vec->capacity * sizeof(char) * 2);
        if (vec == NULL) {
            return 0;
        }
    }
    char *new_string = strdup(str);
    if (new_string == NULL) {
        return 0;
    }
    vec->data[vec->size] = new_string;
    vec->size++;
    return 1;
}

const char* vector_get(const StringVector *vec, size_t index) {
    if (vec->size == index) {
        return NULL;
    }
    return vec->data[index];
}

void vector_free(StringVector *vec) {
    int size = vec->size - 1;
    for(int i = 0; i < size; i++) {
        if (vec->data[i] != NULL) {
            free(vec->data[i]);
        }
        
    }
    free(vec->data);
    free(vec);
}
