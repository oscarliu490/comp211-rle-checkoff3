# Presentation Materials

**Google Slides deck:** https://docs.google.com/presentation/d/1S7XepYhJWi1tYl9BytsiJMJWrft5V0FO2-NVgvPa1Fo/edit
Title: "checkoff 3" - Data Compression, Oscar Liu, COMP 211
Sub-topic / focus: **PackBits**, the RLE variant (MacPaint / TIFF)
that fixes RLE's worst flaw. General RLE is presented only as
background; PackBits is the main content.

Presented Thu Nov 19, 2026, 12:15-1:00 PM slot, SN147.

## Slides (16, as presented)
1. **Title** - PackBits / An RLE variant - NOT plain RLE / Oscar Liu - COMP 211
2. **What is compression?** - Store same information in fewer bits. RLE is lossless: decoded = exact original. Key pattern: runs. Background only: plain RLE is the textbook version; PackBits redesigns it with packets.
3. **How plain RLE works (background)** - AAAAAAAAAAAABBBBCCDDDD (22 chars) -> 12A4B2C4D (9 chars). Number = how many, letter = of what.
4. **Code** - Screenshot of the plain RLE C code (rle.c).
5. **PackBits in one picture** - Full-slide diagram: input AAAABXYZ; packet 1 = header -3 + A (repeat 4 times), packet 2 = header +3 + BXYZ (copy 4 as-is); stored 7 bytes vs plain RLE's 10.
6. **PackBits vs plain RLE: the difference** - One signed header byte per packet: 0 to 127 = LITERAL (copy next header+1 bytes), -1 to -127 = RUN (repeat next byte 1-header times), -128 = no-op. AAAAAAAAAAAA -> 2 bytes; ABABABAB -> 9 bytes instead of plain RLE's 16.
7. **Step 1: Look at the data** - PackBits scans left to right asking: does this byte repeat, or not?
8. **Step 2: A run becomes a packet** - AAAA -> [-3][A]. Negative label = repeat mode.
9. **Step 3: No run? Just copy it** - BXYZ -> [+3][B][X][Y][Z]. Positive label = copy mode; plain RLE cannot do this.
10. **Step 4: Done - and decoding undoes it** - Stored [-3][A][+3][B][X][Y][Z] = 7 bytes; decoding rebuilds AAAABXYZ exactly (lossless).
11. **Code** - PackBits code / output image.
12. **When it wins / when it fails** - Plain RLE: WIN on runs (fax pages, simple images), FAIL on ABABABAB (doubles). PackBits removes the fail case: ABABABAB costs 9 bytes, not 16.
13. **Real world uses** - Fax machines, BMP image formats, simple graphics/icons/screenshots; PackBits specifically: MacPaint files and TIFF images.
14. **Where PackBits lives: MacPaint + TIFF** - MacPaint (1984): 576x720 black-and-white bitmaps on 400 KB floppies, PackBits invented to store them. TIFF (1986): built for scanned images; compression tag 32773 = PackBits; still used for scans, archives, print.
15. **Takeaway + question** - Takeaway: plain RLE bets everything on runs existing; PackBits hedges - repeat what repeats, copy what does not, one signed header byte decides. Audience question: decoding PackBits, the next label is -9 - repeat or copy, and how many bytes? (Answer: repeat, 10 copies.)
16. **Thank you** - Topic was PackBits, an RLE variant. Repo: github.com/oscarliu490/comp211-rle-checkoff3

## Code in this repo
- `packbits.c` - PackBits encoder/decoder (main code). Output matches the slides: AAAABXYZ -> 7 bytes (packets [-3][3]); ABABABAB -> 9 bytes; 12 A's -> 2 bytes.
- `rle.c` - Plain RLE encoder/decoder, kept for comparison (it demonstrates the flaw PackBits fixes).
