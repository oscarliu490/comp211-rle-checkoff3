# Data Compression: PackBits - an RLE Variant
COMP 211 Checkoff 3 - Oscar Liu
Sub-topic: PackBits (the TIFF/MacPaint form of run-length encoding).
General RLE is the background; PackBits is my focus, because it
fixes RLE's worst flaw - data with no repeats getting bigger.

## What I explored
How computers store the same information in fewer bits, using
run-length encoding (RLE) - one of the simplest lossless
compression techniques - and specifically its PackBits variant.

## How RLE works
Data often contains "runs": the same value repeated in a row.
Instead of storing every repeat, RLE stores a pair:
(count, value).

Example:
```
Original: AAAAAAAAAAAABBBBCCDDDD   (22 characters)
Encoded:  12A4B2C4D                (9 characters)
Decoded:  AAAAAAAAAAAABBBBCCDDDD   (exact original - lossless)
```

Decoding reads a number and a value, and repeats the value
that many times.

## My most interesting discoveries
1. **RLE can make data BIGGER.** With no runs, e.g. `ABABABAB`,
   RLE produces `1A1B1A1B1A1B1A1B` - twice the size. The counts
   become pure overhead.
2. **Compression depends on the data, not just the algorithm.**
   RLE is great for data with long runs (fax pages, simple images
   with flat color in BMP/TIFF) and useless for normal text,
   where letters rarely repeat consecutively.
3. **Lossless vs lossy matters.** RLE is lossless - you get the
   exact original back, which is required for text and files.
   Formats like JPEG can throw away detail, RLE cannot.

## My focus: PackBits
PackBits (Apple, used in MacPaint and TIFF) encodes data in
packets. Each packet starts with one signed header byte:

- **0 to 127 - LITERAL packet:** copy the next (header + 1) bytes as-is.
- **-1 to -127 - RUN packet:** repeat the next byte (1 - header) times.
  Example: header -11 followed by `A` means 12 A's, in 2 bytes total.
- **-128 - no-op:** do nothing (reserved).

Why it matters: plain RLE turns `ABABABAB` into 16 bytes (double).
PackBits uses a literal packet instead: header 7 + the 8 bytes =
9 bytes. Its worst case is about 1 extra byte per 128 literals
(~0.8% growth), not 100%. The encoder picks per packet whichever
form - run or literal - is smaller.

## Code
`rle.c` - a small C program that encodes and decodes strings
with RLE and shows a good case, an image-like case, and a
bad case where the data grows.

### Run it
```bash
gcc -std=c11 -Wall -Wextra -o rle rle.c
./rle
./rle AAAAABBBCC
```

## Presentation
Google Slides deck: https://docs.google.com/presentation/d/1S7XepYhJWi1tYl9BytsiJMJWrft5V0FO2-NVgvPa1Fo/edit
See `PRESENTATION.md` in this repo for the full slide-by-slide
content of the deck.

## Reflection on using AI
I learned a lot from my time doing this project. I used AI (Muse)
as a tutor to explore RLE: I started by asking it to summarize the
assignment and my topic, then asked for a C example I could compile
and run myself, and then asked follow-up questions until I could
explain the code on my own.

Prompts I actually used in that conversation:
- "can u summarize my duties and help me start" (with the assignment)
- "what does sprintf do in C"
- "can u explain each part of the code for me"

AI is really useful for explaining new things and helping people
learn - the multi-digit count in decode (count = count * 10 + digit)
only clicked for me after that explanation. It is not always right,
though: the AI first told me the encoded example was 8 characters,
and running the program showed it was actually 9, so testing things
myself mattered. AI is so powerful that we can't misuse it - it
should be used to understand things, not to avoid learning them.
What I would do differently: start earlier, and test first instead
of reading first.

AI conversation: Muse (Meta AI), Nov 19, 2026 - this app does not
generate a public permalink; transcript available on request.

## Resources
- COMP 211 course material on data representation / bitwise topics
- Run-length encoding - Wikipedia (overview and BMP/TIFF/fax uses)
