/* packbits.c - PackBits encoder/decoder demo (COMP 211 Checkoff 3)
 *
 * PackBits is the RLE variant used in MacPaint files and TIFF images.
 * Data is stored in packets. Each packet starts with ONE signed
 * header byte that says what kind of packet it is:
 *
 *   header   0 to 127 : LITERAL packet - copy the next (header + 1)
 *                       bytes exactly as they are.
 *   header  -1 to -127: RUN packet - repeat the next byte
 *                       (1 - header) times.
 *   header -128       : no-op, do nothing (reserved).
 *
 * Unlike plain RLE, data with no repeats does NOT blow up:
 * a literal packet costs only 1 header byte per up to 128 bytes.
 *
 * Build: gcc -std=c11 -Wall -Wextra -o packbits packbits.c
 * Run:   ./packbits
 */
#include <stdio.h>
#include <string.h>

/* Count how many times the byte at position i repeats (max 128,
   because one packet can describe at most 128 bytes). */
static int run_length(const unsigned char *in, int n, int i) {
    int run = 1;
    while (i + run < n && in[i + run] == in[i] && run < 128) {
        run++;
    }
    return run;
}

/* Encode in[0..n) into out. Returns the number of bytes written. */
int packbits_encode(const unsigned char *in, int n, unsigned char *out) {
    int i = 0; /* read position in the input */
    int j = 0; /* write position in the output */
    while (i < n) {
        int run = run_length(in, n, i);
        if (run >= 3) {
            /* RUN packet: header = 1 - run (a negative byte),
               then the byte itself, once. */
            out[j++] = (unsigned char)(1 - run);
            out[j++] = in[i];
            i += run;
        } else {
            /* LITERAL packet: gather bytes until just before the
               next run of 3+, or until we have 128 of them. */
            int start = i;
            int count = 0;
            while (i < n && count < 128 && run_length(in, n, i) < 3) {
                i++;
                count++;
            }
            out[j++] = (unsigned char)(count - 1); /* header 0..127 */
            memcpy(out + j, in + start, count);
            j += count;
        }
    }
    return j;
}

/* Decode in[0..n) into out. Returns the number of bytes written. */
int packbits_decode(const unsigned char *in, int n, unsigned char *out) {
    int i = 0;
    int j = 0;
    while (i < n) {
        int header = (signed char)in[i++]; /* read as a SIGNED byte */
        if (header >= 0) {
            /* LITERAL: copy the next header + 1 bytes as-is. */
            for (int k = 0; k < header + 1; k++) {
                out[j++] = in[i++];
            }
        } else if (header >= -127) {
            /* RUN: repeat the next byte 1 - header times. */
            unsigned char c = in[i++];
            for (int k = 0; k < 1 - header; k++) {
                out[j++] = c;
            }
        }
        /* header == -128: no-op, nothing to do. */
    }
    return j;
}

/* Size plain RLE would need for the same data: 2 bytes per run
   (one count byte + one value byte), like the text form 12A4B2C. */
static int plain_rle_size(const unsigned char *in, int n) {
    int size = 0;
    int i = 0;
    while (i < n) {
        i += run_length(in, n, i);
        size += 2;
    }
    return size;
}

static void demo(const char *label, const unsigned char *data, int n) {
    unsigned char encoded[4096];
    unsigned char decoded[4096];
    int enc_n = packbits_encode(data, n, encoded);
    int dec_n = packbits_decode(encoded, enc_n, decoded);
    int ok = (dec_n == n && memcmp(data, decoded, n) == 0);

    printf("%s\n", label);
    printf("  original : %d bytes\n", n);
    printf("  PackBits : %d bytes (packets:", enc_n);
    for (int i = 0; i < enc_n;) {
        int header = (signed char)encoded[i];
        printf(" [%d]", header);
        i += (header >= 0) ? header + 2 : 2; /* skip packet body */
    }
    printf(" )\n");
    printf("  plain RLE: %d bytes\n", plain_rle_size(data, n));
    printf("  round-trip decode matches original: %s\n\n", ok ? "YES" : "NO");
}

int main(void) {
    demo("Mixed data: AAAABXYZ (the slide example)",
         (const unsigned char *)"AAAABXYZ", 8);
    demo("No repeats: ABABABAB (plain RLE's worst case)",
         (const unsigned char *)"ABABABAB", 8);
    demo("Long run: AAAAAAAAAAAA",
         (const unsigned char *)"AAAAAAAAAAAA", 12);

    /* Edge cases, checked silently unless something is wrong. */
    unsigned char big[300];
    unsigned char enc[4096];
    unsigned char dec[4096];
    memset(big, 'W', sizeof big); /* run of 300 -> packets 128+128+44 */
    int enc_n = packbits_encode(big, 300, enc);
    int dec_n = packbits_decode(enc, enc_n, dec);
    printf("Run of 300 W's: PackBits %d bytes, round-trip %s\n",
           enc_n, (dec_n == 300 && memcmp(big, dec, 300) == 0) ? "YES" : "NO");

    /* Deterministic pseudo-random binary data (plenty of short runs). */
    unsigned char rnd[1000];
    unsigned int seed = 42;
    for (int i = 0; i < 1000; i++) {
        seed = seed * 1103515245 + 12345;
        rnd[i] = (unsigned char)((seed >> 16) % 4 + 'A');
    }
    enc_n = packbits_encode(rnd, 1000, enc);
    dec_n = packbits_decode(enc, enc_n, dec);
    printf("1000 pseudo-random bytes: PackBits %d bytes, round-trip %s\n",
           enc_n, (dec_n == 1000 && memcmp(rnd, dec, 1000) == 0) ? "YES" : "NO");
    return 0;
}
