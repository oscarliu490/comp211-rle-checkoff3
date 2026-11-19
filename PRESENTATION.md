# Presentation Materials

**Google Slides deck:** https://docs.google.com/presentation/d/1S7XepYhJWi1tYl9BytsiJMJWrft5V0FO2-NVgvPa1Fo/edit
Title: "checkoff 3" - Data Compression, Oscar Liu, COMP 211
Sub-topic / focus: **PackBits**, the RLE variant (MacPaint / TIFF)
that fixes RLE's worst flaw. General RLE is presented only as
background (slides 2-4); PackBits is the main content (slide 5).

## Slides
1. **Title** - Run-Length Encoding (RLE) / Data Compression / Oscar Liu - COMP 211 / Focus: PackBits - the RLE variant that fixes RLE's worst flaw
2. **What is compression?** - Store same information in fewer bits. RLE is lossless: decoded = exact original. Key pattern: runs (same value repeated). [pixel alien + RLE diagram images]
3. **How it works** - AAAAAAAAAAAABBBBCCDDDD (22 chars) -> count the runs: 12xA, 4xB, 2xC, 4xD -> 12A4B2C4D (9 chars). Number = how many, letter = of what.
4. **I tested it in C** - Core of encode(): count a run, then write (count, value). Results: the 22-char example decoded back exactly; ABABABAB grew to 1A1B1A1B1A1B1A1B (no runs) - the flaw PackBits fixes.
5. **PackBits: RLE that doesn't blow up** - One signed header byte per packet: 0 to 127 = LITERAL (copy next header+1 bytes), -1 to -127 = RUN (repeat next byte 1-header times), -128 = no-op. AAAAAAAAAAAA -> header -11 + A = 2 bytes. ABABABAB -> literal packet, 9 bytes instead of plain RLE's 16. Worst case ~1 byte per 128, not double.
6. **When it wins / when it fails** - WIN: WWWWWWWWWW... (fax pages, simple images, flat color). FAIL (plain RLE): ABABABAB -> 1A1B1A1B1A1B (gets bigger).
7. **Real world uses** - Fax machines (mostly-white pages), BMP / TIFF image formats (TIFF uses PackBits), simple graphics / icons / screenshots. Not good for normal text.
8. **Takeaway + question** - Takeaway: vanilla RLE bets everything on runs existing; PackBits hedges - it compresses runs when they're there and copies literals cheaply when they're not. Audience question: For ABABAB, will PackBits use a run packet or a literal packet? (Answer: literal.)
9. **Thank you** - with GitHub repo link: github.com/oscarliu490/comp211-rle-checkoff3

Images: Wikimedia Commons - Space Invaders pixel sprite, RLE diagram,
Samsung SF-3100 fax machine photo.
