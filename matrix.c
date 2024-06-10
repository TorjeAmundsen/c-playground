#include <stdio.h>
#include <stdlib.h>

int **createMatrix(int rows, int cols, int fillValue)
{
    int **matrix;
    matrix = malloc(sizeof(int *) * rows);

    for (int i = 0; i < rows; i++)
        matrix[i] = malloc(sizeof(int) * cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix[i][j] = fillValue;

    return matrix;
}

int main()
{
    system("cls");

    int **matrix;

    int rows = 5;
    int cols = 6;

    matrix = createMatrix(rows, cols, 25);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }

    for (int i = 0; i < rows; i++)
        free(matrix[i]);

    free(matrix);
    return 0;
}