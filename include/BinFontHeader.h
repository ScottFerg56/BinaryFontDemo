#ifndef _BINFONTHEADER_H_
#define _BINFONTHEADER_H_

#include <Adafruit_GFX.h>

#if UINTPTR_MAX != 0xFFFFFFFFu
// only 32-bit pointers have been tested:
// rebuilding the GFXFont structure and its pointer fields may require adjustment
// for architecures with different pointer sizes
#error "untested processor pointer size!"
#endif

typedef struct {
  uint32_t bitmapOffset;
  uint32_t glyphOffset;
  uint16_t first;
  uint16_t last;
  uint8_t yAdvance;
} BinFontHeader;

GFXfont* FixBinaryFont(uint8_t* pdata)
{
    // replace the BinFontHeader at the start of the font data
    // with a GFXfont structure built from the font data
    // these structures are the same size for a 32-bit pointer architecture
    // the GFXfont structure will be smaller on a 16-bit pointer architecture
    BinFontHeader* phdr = (BinFontHeader*)pdata;
    GFXfont* pfont = (GFXfont*)pdata;
    // bitmap pointer calculated with offset
    pfont->bitmap = pdata + phdr->bitmapOffset;
    // glyph pointer calculated with offset
    pfont->glyph = (GFXglyph*)(pdata + phdr->glyphOffset);
    // other fields copied
    pfont->first = phdr->first;
    pfont->last  = phdr->last;
    pfont->yAdvance = phdr->yAdvance;
    return pfont;
}

#endif // _BINFONTHEADER_H_
