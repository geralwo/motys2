#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "motys_stdlib.h"

MotysArray* motys_array_create(int size_of_element, int initial_capacity) {
    MotysArray* array = malloc(sizeof(MotysArray));
    array->elements = malloc(size_of_element * initial_capacity);
    array->size = 0;
    array->capacity = initial_capacity;
    array->size_of_element = size_of_element;
    return array;
}

void motys_array_add(MotysArray* array, void* element) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->elements = realloc(array->elements, array->size_of_element * array->capacity);
    }
    void* target = (char*)array->elements + (array->size * array->size_of_element);
    memcpy(target, element, array->size_of_element);
    array->size++;
}

void* motys_array_get(MotysArray* array, int index) {
    if (index < 0 || index >= array->size) {
        return NULL; // Return NULL for out-of-bounds access
    }
    return (char*)array->elements + (index * array->size_of_element);
}

void motys_array_free(MotysArray* array) {
    free(array->elements);
    free(array);
}
