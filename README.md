# Data Compression: Run-Length Encoding (RLE)
COMP 211 Checkoff 3 - Oscar Liu

## What I explored
How computers store the same information in fewer bits, using
run-length encoding (RLE) - one of the simplest lossless
compression techniques.

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

## Resources
- COMP 211 course material on data representation / bitwise topics
- Run-length encoding - Wikipedia (overview and BMP/TIFF/fax uses)
