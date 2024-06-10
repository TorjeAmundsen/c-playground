#include <stdio.h>
#include <stdbool.h>

bool upOrDown(int *other) {
    volatile int x;

    if (!other) return upOrDown(&x);
    else return &x > other;
}

int main() {
    printf("%s\n", upOrDown(NULL) ? "Stack grows up" : "Stack grows down");
    return 0;
}