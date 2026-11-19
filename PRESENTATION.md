# Presentation Materials

**Google Slides deck:** https://docs.google.com/presentation/d/1S7XepYhJWi1tYl9BytsiJMJWrft5V0FO2-NVgvPa1Fo/edit
Title: "checkoff 3" - Run-Length Encoding (RLE), Oscar Liu, COMP 211

## Slides
1. **Title** - Run-Length Encoding (RLE) / Data Compression / Oscar Liu - COMP 211
2. **What is compression?** - Store same information in fewer bits. RLE is lossless: decoded = exact original. Key pattern: runs (same value repeated). [pixel alien + RLE diagram images]
3. **How it works** - AAAAAAAAAAAABBBBCCDDDD (22 chars) -> count the runs: 12xA, 4xB, 2xC, 4xD -> 12A4B2C4D (9 chars). Number = how many, letter = of what.
4. **I tested it in C** - Core of encode(): count a run, then write (count, value). Results: the 22-char example decoded back exactly; ABABABAB grew to 1A1B1A1B1A1B1A1B (no runs).
5. **When it wins / when it fails** - WIN: WWWWWWWWWW... (fax pages, simple images, flat color). FAIL: ABABABAB -> 1A1B1A1B1A1B (gets bigger).
6. **Real world uses** - Fax machines (mostly-white pages), BMP / TIFF image formats, simple graphics / icons / screenshots. Not good for normal text.
7. **Takeaway + question** - Takeaway: compression works by matching a method to a pattern in the data. Audience question: Would WWWWWWSSSSSSWW compress well or get bigger? (Answer: 6W6S2W - well, long runs.)
8. **Thank you**

Images: Wikimedia Commons - Space Invaders pixel sprite, RLE diagram,
Samsung SF-3100 fax machine photo.
