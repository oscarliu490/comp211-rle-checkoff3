/*
 * rle.c - Run-Length Encoding (RLE) demo for COMP 211 Checkoff 3
 * Oscar Liu - Data Compression: Run-Length Encoding
 *
 * Idea: instead of storing every repeated value, store (count, value).
 *   AAAAAAAAAAAA BBBB CC DDDD  ->  12A4B2C4D
 *
 * RLE is LOSSLESS: decoding gives back the exact original string.
 *
 * Compile: gcc -std=c11 -Wall -Wextra -o rle rle.c
 * Run:     ./rle
 *          ./rle AAAAABBBCC      (try your own string)
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Encode: walk the string once, counting each run of the same char. */
void encode(const char *in, char *out) {
    int n = strlen(in);
    int j = 0;
    for (int i = 0; i < n; ) {
        char c = in[i];
        int count = 1;
        while (i + count < n && in[i + count] == c) {
            count++;
        }
        j += sprintf(out + j, "%d%c", count, c);
        i += count;
    }
    out[j] = '\0';
}

/* Decode: read a number, then repeat the next char that many times. */
void decode(const char *in, char *out) {
    int j = 0;
    for (int i = 0; in[i] != '\0'; ) {
        int count = 0;
        while (isdigit((unsigned char)in[i])) {
            count = count * 10 + (in[i] - '0');
            i++;
        }
        char c = in[i++];
        for (int k = 0; k < count; k++) {
            out[j++] = c;
        }
    }
    out[j] = '\0';
}

void demo(const char *original) {
    char encoded[1024], decoded[1024];
    encode(original, encoded);
    decode(encoded, decoded);

    printf("Original (%2zu chars): %s\n", strlen(original), original);
    printf("Encoded  (%2zu chars): %s\n", strlen(encoded), encoded);
    printf("Decoded  (%2zu chars): %s\n", strlen(decoded), decoded);
    printf("Round-trip matches? %s\n", strcmp(original, decoded) == 0 ? "YES" : "NO");
    if (strlen(encoded) < strlen(original))
        printf("Result: COMPRESSED by %zu chars\n\n", strlen(original) - strlen(encoded));
    else
        printf("Result: GREW by %zu chars - no long runs, so RLE hurts here!\n\n",
               strlen(encoded) - strlen(original));
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        demo(argv[1]);
        return 0;
    }
    printf("=== Good case: long runs ===\n");
    demo("AAAAAAAAAAAABBBBCCDDDD");

    printf("=== Image-like case: a row of mostly white (W) pixels ===\n");
    demo("WWWWWWWWWWWWBWWWWWWWWWWWWWWBBBWW");

    printf("=== Bad case: no repeats at all ===\n");
    demo("ABABABAB");
    return 0;
}
