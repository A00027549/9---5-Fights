def RGBToWord(r, g, b):
    rvalue = 0
    rvalue = rvalue + (g >> 5)
    rvalue = rvalue + ((g & 7) << 13)
    rvalue = rvalue + ((r >> 3) << 8)
    rvalue = rvalue + ((b >> 3) << 3)
    return rvalue

from PIL import Image
from os import sys

def main():
    args = sys.argv
    if len(args) != 2:
        print("incorrect usage, please pass name of bmp to program")
        return -1

    ImageFileName = args[1]
    im = Image.open(ImageFileName)
    print(im.format, im.size, im.mode)
    pixels = list(im.getdata())

    # Build palette (unique colours, max 256)
    palette = []
    palette_map = {}
    for px in pixels:
        colour = RGBToWord(px[0], px[1], px[2])
        if colour not in palette_map:
            if len(palette) >= 256:
                print("WARNING: more than 256 unique colours, some will be remapped to closest")
                # find closest existing palette entry
                closest = min(palette, key=lambda c: abs(c - colour))
                palette_map[colour] = palette.index(closest)
            else:
                palette_map[colour] = len(palette)
                palette.append(colour)

    print(f"\n// Palette ({len(palette)} colours)")
    print("const uint16_t sprite_pal[] = {")
    print("    ", end="")
    for i, c in enumerate(palette):
        print(f"{c}", end=",")
        if (i + 1) % 16 == 0:
            print("\n    ", end="")
    print("\n};")

    # Output pixel indices
    print(f"\n// Pixel indices (8-bit, {len(pixels)} pixels)")
    print("const uint8_t sprite_px[] = {")
    print("    ", end="")
    for i, px in enumerate(pixels):
        colour = RGBToWord(px[0], px[1], px[2])
        print(f"{palette_map[colour]}", end=",")
        if (i + 1) % 31 == 0:  # new line every 31 pixels (sprite width)
            print("\n    ", end="")
    print("\n};")

if __name__ == "__main__":
    main()