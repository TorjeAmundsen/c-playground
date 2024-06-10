#include <stdio.h>
#include <stdlib.h>

void setArray(int value, int *array)
{
    for (int i = 0; i < 5; i++) array[i] = value;
}

int *heapArray(int value)
{
    int *array = malloc(sizeof(int) * 5);

    for (int i = 0; i < 5; i++)
        array[i] = value;
    return array;
}

int main()
{
    int array[] = {1, 2, 3, 4, 5};

    setArray(69, array);

    for (int i = 0; i < 5; i++)
        printf("array[%d] = %d\n", i, array[i]);

    int *heapTest = heapArray(222);

    heapTest[2] = 1;

    for (int i = 0; i < 5; i++)
        printf("heapTest[%d] = %d\n", i, heapTest[i]);

    free(heapTest);

    return 0;
}