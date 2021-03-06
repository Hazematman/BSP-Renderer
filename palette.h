#ifndef PALETTE_H
#define PALETTE_H

#include "types.h"

struct s_Palette;
typedef struct s_Palette Palette;

Palette *readPalette(void *paletteData);
colour3UByte getPaletteColour(Palette *plt, uint8_t index);
void freePalette(Palette *plt);

#endif
