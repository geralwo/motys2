#ifndef MOTYS_H
#define MOTYS_H

typedef enum {
    TYPE_INT,
    TYPE_STRING,
    TYPE_FLOAT
} DataType;

typedef struct {
    DataType type;
    void* value;
} MotysData;

typedef struct {
    char* name;
    MotysData data;
} MotysVariable;

typedef struct {
	void* elements;
	int size;
	int capacity;
	int size_of_element;
} MotysArray;

MotysArray* motys_array_create(int size_of_element, int initial_capacity);
void motys_array_add(MotysArray* array, void* element);
void* motys_array_get(MotysArray* array, int index);
void motys_array_free(MotysArray* array);

#endif // MOTYS_H
