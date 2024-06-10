#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROR(num, count) (((num) >> (count)) | ((num) << (32 - (count))))

typedef unsigned int Word;
typedef unsigned char Byte;


void printBits(char num) {
    printf("Printing: %u\n", num);

    unsigned int size = sizeof(char);
    unsigned int maxPow = 1 << (size * 8 - 1);

    for(int i = 0; i < size * 8; ++i) {
        printf("%u", num&maxPow ? 1 : 0);
        num = num << 1;
    }
    printf("\n\n");
}

void printBits_long(long long num) {
    unsigned int size = sizeof(long long);
    printf("Printing: %u\nBitcount: %u\n", num, size * 8);

    unsigned int maxPow = 1 << (size * 8 - 1);

    for(int i = 0; i < size * 8; ++i) {
        printf("%u", num & maxPow ? 1 : 0);
        num = num << 1;
    }
    printf("\n\n");
}

void printBits_uint(unsigned int num) {
    printf("Printing: %u\n", num);

    unsigned int size = sizeof(unsigned int);
    unsigned int maxPow = 1 << (size * 8 - 1);

    for(int i = 0; i < size * 8; ++i) {
        printf("%u", num&maxPow ? 1 : 0);
        num = num << 1;
    }
    printf("\n");
}

unsigned int rotateRight(unsigned int num, int count) {
    unsigned int shifted = num >> count;
    unsigned int rotated = num << (32 - count);
    return shifted | rotated;
}

unsigned int decodeChars_uint(char num1, char num2, char num3, char num4) {
    return (num1 << 24) + (num2 << 16) + (num3 << 8) + num4;
}

int main(int argc, char *argv[]) {
    char a = 'a';
    char b = 'b';
    char c = 'c';

    char x = 0b00000001;

    int testArr[] = {0, 1, 2, 3, 4};

    char testInitArr[9] = {0};

    for (int j = 0; j < 9; ++j) {
        printf("testInitArr[%d]: %d\n", j, testInitArr[j]);
    }

    int i = 0;

    printf("i: %d\n", i);
    printf("testArr[i++]: %d\n", testArr[i++]);
    printf("testArr[++i]: %d\n", testArr[++i]);

    printf("0x80 == 0b10000000: %d\n", 0x80 == 0b10000000);

    printBits(a);
    printBits(b);
    printBits(c);

    printBits(x);

    printf("argc: %d\n", argc);

    if (argc == 2) {
        printf("(long long)strlen(argv[1]):\n");
        long long len = (long long)strlen(argv[1]);
        printBits_long(len);
    }

    unsigned int testShift = 65535;
    printBits_uint(testShift);

    unsigned int shifted = rotateRight(testShift, 4);
    printBits_uint(shifted);

    printBits_uint(ROR(testShift, 4));
    printBits_uint(testShift);

    printf("sizeof(unsigned long long int): %d\n", sizeof(unsigned long long int));

    char castTest = (char)strlen(argv[1]);

    printBits(castTest);

    printf("(char)strlen(argv[1]): %d\n", (char)strlen(argv[1]));

    char c1 = 1;
    char c2 = 1;
    char c3 = 1;
    char c4 = 1;

    unsigned int decodeResult = decodeChars_uint(c1, c2, c3, c4);

    printBits_uint(decodeResult);

    Word len = 8;
    Word *wordAlloced = malloc(sizeof(Word) * len);

    for (int i = 0; i < len; ++i) {
        wordAlloced[i] = 0;
        printBits_uint(wordAlloced[i]);
    }

    Byte testBitwise[4] = {0};

    Word n = 7;

    testBitwise[3] = n & 0xff;

    printf("Last 8 bits of %u: %u\n", n, testBitwise[3]);
    
    return 0;
}

/*
Current output ".\tests test":

testInitArr[0]: 0
testInitArr[1]: 0
testInitArr[2]: 0
testInitArr[3]: 0
testInitArr[4]: 0
testInitArr[5]: 0
testInitArr[6]: 0
testInitArr[7]: 0
testInitArr[8]: 0
i: 0
testArr[i++]: 0
testArr[++i]: 2
0x80 == 0b10000000: 1
Printing: 0
00000000

Printing: 1
00000001

Printing: 2
00000010

Printing: 1
00000001

argc: 2
(unsigned long long)strlen(argv[1]):
Printing: 4
Bitcount: 0
0000000000000000000000000000010000000000000000000000000000000000

Printing: 65535
00000000000000001111111111111111

Printing: 4026535935
11110000000000000000111111111111

*/