#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define RESET      "\x1b[0m"
#define FG_RED     "\x1b[31m"
#define FG_GREEN   "\x1b[32m"
#define FG_YELLOW  "\x1b[33m"
#define FG_BLUE    "\x1b[34m"
#define FG_MAGENTA "\x1b[35m"
#define FG_CYAN    "\x1b[36m"
#define BG_RED     "\x1b[41m"
#define BG_GREEN   "\x1b[42m"
#define BG_YELLOW  "\x1b[43m"
#define BG_BLUE    "\x1b[44m"
#define BG_MAGENTA "\x1b[45m"
#define BG_CYAN    "\x1b[46m"

typedef struct coord
{
    int row;
    int col;
} coord;

typedef struct node
{
    int distance;
    bool visited;
    bool isWall;
    coord previous;
} node;

typedef struct
{
    int row;
    int col;
    int distance;
    void *next;
} listNode;

static int totalRows = 29;
static int totalCols = 39;

listNode *head = NULL;

void push(int row, int col, int distance, int *queueCount)
{
    *queueCount = *queueCount + 1;

    listNode *new = malloc(sizeof(listNode));
    if (new == NULL)
        return;

    new->row = row;
    new->col = col;
    new->distance = distance;
    new->next = head;
    head = new;

    printf("queueCount: %d\n", *queueCount);
}

listNode *popMin()
{
    //unfinished
    int i = 0;
    int highestIndex = 0;
    int highestDistance = 0;

    listNode *temp = head;

    while (temp != NULL)
    {
        printf("temp->distance: %d\n", temp->distance);

        if (temp->distance > highestDistance)
        {
            highestIndex = i;
            highestDistance = temp->distance;
        }
        ++i;
        temp = temp->next;
    }

    printf("highestIndex: %d, highestDistance: %d", highestIndex, highestDistance);

    if (highestIndex == 0)
    {
        return head;
    }
    else
    {
        temp = head;
        for (int i = 0; i < highestIndex; ++i)
        {
            
        }
    }

    return temp;
}

void setCursorPosition(int row, int col)
{
    printf(RESET "\033[%d;%df", row + 1, col * 2 + 1);
}

node **createNewGraph(int rows, int cols, coord *start, coord *target)
{
    node **graph = malloc(sizeof(node *) * rows);

    for (int i = 0; i < rows; ++i)
        graph[i] = malloc(sizeof(node) * cols);
    
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            graph[i][j].distance = INT_MAX;
            graph[i][j].visited = false;
            graph[i][j].isWall = false;
            graph[i][j].previous.row = -1;
            graph[i][j].previous.col = -1;
        }
    }

    start->row = 2;
    start->col = 2;

    target->row = rows - 3;
    target->col = cols - 3;

    printf("Graph created!\n");

    return graph;
}

void printGraph(node **graph, coord *start, coord *target)
{
    setCursorPosition(0, 0);

    for (int i = 0; i < totalRows; ++i)
    {
        for (int j = 0; j < totalCols; ++j)
        {
            if (graph[i][j].visited)
                printf(BG_GREEN "  ");
            else printf(BG_CYAN "  ");
        }
        printf(RESET "\n");
    }

    setCursorPosition(start->row, start->col);
    printf(BG_BLUE "  ");

    setCursorPosition(totalCols / 1.3 + 1, 0);
}

void drawSearching(int row, int col)
{
    setCursorPosition(row, col);
    printf(BG_YELLOW "  ");
}

bool isQueueable(node **graph, int row, int col)
{
    if (row < 0 || row >= totalRows || col < 0 || col >= totalCols)
        return false;
    if (graph[row][col].visited || graph[row][col].isWall)
        return false;
    return true;
}

void dijkstra(node **graph, coord *start, coord *target)
{
    int rowDirections[] = {1, -1, 0,  0};
    int colDirections[] = {0,  0, 1, -1};

    int queueCount = 0;

    graph[start->row][start->col].distance = 0;

    push(start->row, start->col, 0, &queueCount);

    printf("*head: %p\n", head);

    while (queueCount > 0)
    {
        printf("Entering Dijkstra's while-loop... queueCount: %d\n", queueCount);
        listNode *current = popMin();

        printf("Searching: [%d, %d]", current->row,  current->col);

        node *currentInGraph = &graph[current->row][current->col];

        if (currentInGraph->visited || currentInGraph->isWall)
            continue;
        
        currentInGraph->visited = true;

        drawSearching(current->row, current->col);

        if (current->row == target->row && current->col == target->col)
        {
            // found path! not implemented
            printf("Path found!\n");
            return;
        }
        for (int i = 0; i < 4; ++i)
        {
            printf("Entering directions for-loop...\n");
            int newRow = current->row + rowDirections[i];
            int newCol = current->col + colDirections[i];

            if (isQueueable(graph, newRow, newCol))
            {
                printf("Item is queueable!\n");
                int newDistance = current->distance + 1;
                if (newDistance < graph[newRow][newCol].distance)
                {
                    graph[newRow][newCol].distance = newDistance;
                    coord previous;
                    previous.row = current->row;
                    previous.col = current->col;
                    graph[newRow][newCol].previous = previous;
                    printf("Queued node at [%d, %d]\n", newRow, newCol);
                    
                }
            }
        }
    }
    printf("No path found!\n");
}

int main()
{
    system("cls");
    
    coord start;
    coord target;

    node **graph = createNewGraph(totalRows, totalCols, &start, &target);

    printGraph(graph, &start, &target);

    dijkstra(graph, &start, &target);

    for (int i = 0; i < totalRows; ++i)
        free(graph[i]);
    free(graph);

    printf("Graph memory freed.\n");

    return 0;
}