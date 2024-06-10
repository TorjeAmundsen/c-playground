#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int row;
    int col;
    int distance;
    void *next;
} Node;

Node *head = NULL;
Node *tail = NULL;

Node *enqueue(int row, int col, int distance)
{
    Node *new = malloc(sizeof(Node));
    if (new == NULL)
        return NULL;

    new->row = row;
    new->col = col;
    new->distance = distance;
    new->next = NULL;
    
    if (head == NULL)
        head = new;
    
    tail = new;

    return new;
}

Node *dequeue()
{
    if (head == NULL)
        return NULL;
    
    Node *output = head;
    head = head->next;
    return output;
}

void printList()
{
    Node *current = head;
    while (current != NULL)
    {
        printf("[%d, %d], %d\n\\/", current->row, current->col, current->distance);
        current = current->next;
    }
    printf("\n");
    return;
}

void printMenu()
{
    printf("\nYou have the following options:\n");
    printf("\t[1] Print the list\n");
    printf("\t[2] Enqueue a node\n");
    printf("\t[3] Dequeue a node\n");
    printf("\t[5] Quit\n");
    return;
}

int main()
{
    int option = -1;
    int row = 0;
    int col = 0;
    int distance = 0;
    while (option != 5)
    {
        printMenu();
        int numReceived = scanf("%d", &option);

        if (numReceived == 1 && option > 0 && option <= 5)
        {
            switch (option)
            {
                case 1:
                    printList();
                    break;
                case 2:
                    printf("What data should I enqueue? Row, col, distance:\n");
                    scanf("%d%d%d", &row, &col, &distance);
                    tail = enqueue(row, col, distance);
                    break;
                case 3:
                    printf("Dequeueing...\n");
                    Node *popped = dequeue();
                    if (popped == NULL)
                        printf("Queue is empty!");
                    else
                        printf("Dequeued: [%d, %d], %d", popped->row, popped->col, popped->distance);
                        free(popped);
                        popped = NULL;
                    break;

                case 4:
                    break;

                case 5:
                    break;

            }
        }
    }
    return 0;
}