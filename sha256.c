#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROTRIGHT(num, count) (((num) << (count)) | ((num) >> (32-(count))))
#define S0FUNC(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
#define S1FUNC(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))

typedef unsigned char Byte;
typedef unsigned int Word;

typedef struct {
    Byte data;
    Word datalen;
    Word hash_state[8];
} sha256_ctx;

// first 32 bits of the fractional parts of the cube roots of the first 64 primes (2, 311)
static const Word k[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

unsigned int rightRotate(unsigned int num, int count) {
    unsigned int shifted = num >> count;
    unsigned int rotated = num << (32 - count);
    return shifted | rotated;
}

unsigned int decodeChars_uint(char num1, char num2, char num3, char num4) {
    return (num1 << 24) + (num2 << 16) + (num3 << 8) + num4;
}

void printBits_uint(unsigned int num) {
    unsigned int size = sizeof(unsigned int);
    unsigned int maxPow = 1 << (size * 8 - 1);

    for(int i = 0; i < size * 8; ++i) {
        printf("%u", num&maxPow ? 1 : 0);
        num = num << 1;
    }
}

void printBits(char num) {
    unsigned int size = sizeof(char);
    unsigned int maxPow = 1 << (size * 8 - 1);

    for(int i = 0; i < size * 8; ++i) {
        printf("%u", num&maxPow ? 1 : 0);
        num = num << 1;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {

    if (argc == 0) {
        printf("Error: No input string given.\nUsage: sha256 <input>");
        return 0;
    }
    if (strlen(argv[1]) > 55) {
        printf("Error: Input longer than maximum allowed length (55 for this specific implementation of sha256)");
    }

    // first 32 bits of the fractional parts of the square roots of the first 8 primes (2...19)
    unsigned int h0 = 0x6a09e667;
    unsigned int h1 = 0xbb67ae85;
    unsigned int h2 = 0x3c6ef372;
    unsigned int h3 = 0xa54ff53a;
    unsigned int h4 = 0x510e527f;
    unsigned int h5 = 0x9b05688c;
    unsigned int h6 = 0x1f83d9ab;
    unsigned int h7 = 0x5be0cd19;

    // pre-processing (this only works if input length is limited to 55 characters as it is here)
    char bitArray[64] = {0};

    for (int i = 0; i < strlen(argv[1]); ++i) {
        bitArray[i] = argv[1][i];
    }

    bitArray[strlen(argv[1])] = 0x80; // 0b10000000

    long long inputLen_long = (long long)strlen(argv[1]);

    memcpy(&(bitArray[strlen(argv[1]) - (sizeof(long long))]), &inputLen_long, sizeof(long long));

    // processing the message in 512-bit chunks
    // message is always 512 bits here, so no breaking into chunks in this implementation
    unsigned int words[64] = {0};

    for (int i = 0, j = 0; i < 16; ++i, j +=4) {
        words[i] = decodeChars_uint(bitArray[j], bitArray[j + 1], bitArray[j + 2], bitArray[j + 3]);
    }

    for (int i = 16; i < 64; ++i) {
        unsigned int s0 = S0FUNC(words[i - 15]);
        unsigned int s1 = S1FUNC(words[i - 2]);
        words[i] = words[i - 16] + s0 + words[i - 7] + s1;
    }
    unsigned int a = h0;
    unsigned int b = h1;
    unsigned int c = h2;
    unsigned int d = h3;
    unsigned int e = h4;
    unsigned int f = h5;
    unsigned int g = h6;
    unsigned int h = h7;

    // compression main loop
    for (int i = 0; i < 64; ++i) {
        unsigned int S1 = rightRotate(e, 6) ^ rightRotate(e, 11) ^ rightRotate(e, 25);
        unsigned int ch = (e & f) ^ ((~ e) & g);
        unsigned int temp1 = h + S1 + ch + k[i] + words[i];
        unsigned int S0 = rightRotate(a, 2) ^ rightRotate(a, 13) ^ rightRotate(a, 22);
        unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
        unsigned int temp2 = S0 + maj;
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    h0 = h0 + a;
    h1 = h1 + b;
    h2 = h3 + c;
    h3 = h3 + d;
    h4 = h4 + e;
    h5 = h5 + f;
    h6 = h6 + g;
    h7 = h7 + h;

    unsigned int hashArr[8] = {h0, h1, h2, h3, h4, h5, h6, h7};

    for (int i = 0; i < 8; ++i) {
        printf("%x", hashArr[i]);
    }
    printf("\n");
    for (int i = 0; i < 8; ++i) {
        printBits_uint(hashArr[i]);
    }
    printf("\n");

    printf("strlen(argv[1]): %d\nargv[1]: %s", strlen(argv[1]), argv[1]);

    return 0;
}

/*
Note 1: All variables are 32 bit unsigned integers and addition is calculated modulo 232
Note 2: For each round, there is one round constant k[i] and one entry in the message schedule array w[i], 0 ≤ i ≤ 63
Note 3: The compression function uses 8 working variables, a through h
Note 4: Big-endian convention is used when expressing the constants in this pseudocode,
    and when parsing message block data from bytes to words, for example,
    the first word of the input message "abc" after padding is 0x61626380

Initialize hash values:
(first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19):
h0 := 0x6a09e667
h1 := 0xbb67ae85
h2 := 0x3c6ef372
h3 := 0xa54ff53a
h4 := 0x510e527f
h5 := 0x9b05688c
h6 := 0x1f83d9ab
h7 := 0x5be0cd19

Initialize array of round constants:
(first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311):
k[0..63] :=
   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2

Pre-processing (Padding):
begin with the original message of length L bits
append a single '1' bit
append K '0' bits, where K is the minimum number >= 0 such that (L + 1 + K + 64) is a multiple of 512
append L as a 64-bit big-endian integer, making the total post-processed length a multiple of 512 bits
such that the bits in the message are: <original message of length L> 1 <K zeros> <L as 64 bit integer> , (the number of bits will be a multiple of 512)

Process the message in successive 512-bit chunks:
break message into 512-bit chunks
for each chunk
    create a 64-entry message schedule array w[0..63] of 32-bit words
    (The initial values in w[0..63] don't matter, so many implementations zero them here)
    copy chunk into first 16 words w[0..15] of the message schedule array

    Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array:
    for i from 16 to 63
        s0 := (w[i-15] rightrotate  7) xor (w[i-15] rightrotate 18) xor (w[i-15] rightshift  3)
        s1 := (w[i-2] rightrotate 17) xor (w[i-2] rightrotate 19) xor (w[i-2] rightshift 10)
        w[i] := w[i-16] + s0 + w[i-7] + s1

    Initialize working variables to current hash value:
    a := h0
    b := h1
    c := h2
    d := h3
    e := h4
    f := h5
    g := h6
    h := h7

    Compression function main loop:
    for i from 0 to 63
        S1 := (e rightrotate 6) xor (e rightrotate 11) xor (e rightrotate 25)
        ch := (e and f) xor ((not e) and g)
        temp1 := h + S1 + ch + k[i] + w[i]
        S0 := (a rightrotate 2) xor (a rightrotate 13) xor (a rightrotate 22)
        maj := (a and b) xor (a and c) xor (b and c)
        temp2 := S0 + maj
 
        h := g
        g := f
        f := e
        e := d + temp1
        d := c
        c := b
        b := a
        a := temp1 + temp2

    Add the compressed chunk to the current hash value:
    h0 := h0 + a
    h1 := h1 + b
    h2 := h2 + c
    h3 := h3 + d
    h4 := h4 + e
    h5 := h5 + f
    h6 := h6 + g
    h7 := h7 + h

Produce the final hash value (big-endian):
digest := hash := h0 append h1 append h2 append h3 append h4 append h5 append h6 append h7

*/