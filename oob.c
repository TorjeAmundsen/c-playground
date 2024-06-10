/*
Experimenting with Out-of-Bounds reads, aka reading the value of an array an
index outside of the array's size, resulting in reading values from other
variables or other things stored in memory adjacent to the array
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *first_arr = malloc(sizeof(int) * 4);
    int *second_arr = malloc(sizeof(int) * 4);

    first_arr[0] = 10;
    first_arr[1] = 11;
    first_arr[2] = 12;
    first_arr[3] = 13;
    
    second_arr[0] = 20;
    second_arr[1] = 21;
    second_arr[2] = 22;
    second_arr[3] = 23;


    for (int i = 0; i < 11; i++)
    {
        printf("first_arr[%d]: %d - address: %p\n", i, first_arr[i], &first_arr[i]);
    }

    return 0;
}