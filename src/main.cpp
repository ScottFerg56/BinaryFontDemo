/***************************************************
  This GFX Library example uses the Adafruit ILI9341 TFT FeatherWing
    http://www.adafruit.com/products/3315
  To demonstrate the use of the binary font file format produced by
  the GFX Font Editor.
    https://github.com/ScottFerg56/GFXFontEditor

  The binary file format (.GFXFNTB) is loaded on demand from a file
  into dynamically allocated memory. In this example, the file is
  loaded from the ESP32 SPIFF (flash) file storage into ESP32 PSRAM
  (pseudo ram). The GFXFont structure must be reconstructed
  from the loaded data where it may be selected with Adafruit_GFX::setFont().

  The binary file could be loaded from flash, SD card or other
  data source through network or other communications technology.

  The memory to load the font data into could be allocated from any
  resource accessible to the processor.

  MIT license, use at your own risk.
 ****************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "SPIFFS.h"

// include the code that returns a GFXfont pointer
// from the loaded binary font data
#include "BinFontHeader.h"

#ifdef ESP32
   #define TFT_CS   15
   #define TFT_DC   33
#else
// need proper pin definitions for the above
// a different processor may require attention to pointer size and alignment issues
// an alternate file storage implementation may be required for loading the binary font
// and allocating dynamic memory
#error "Processor not supported!"
#endif

// this font, in header (.h) format was loaded into the GFX Font Editor
// and saved to an altered file name, for demonstration purposes
// this version is actually a couple of bytes smaller in size than the original
#include "FreeMonoBold12pt7b_.h"
//#include "Fonts/FreeMonoBold12pt7b.h"   // the Adafruit equivalent font

// the binary font file resides in the PlatformIO project's 'data' subdirectory
// and is loaded into the ESP32 SPIFF file system using the
// "Build Filesystem image" and "Upload Filesystem Image" commands in the PlatformIO UI
#define FONT_PATH "/FreeMonoBold12pt7b.gfxfntb"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// the binary font will replace the tilde '~' with a heart shape to demonstrate the difference! :)
const char* ascii = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

GFXfont* pBinFont;

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("FeatherWing TFT Binary font Test!");

    // load the binary font file from any available resource
    SPIFFS.begin(true);
    File file = SPIFFS.open(FONT_PATH, "rb");
    size_t size = file.size();

    // allocate memory from any suitable resource
    uint8_t* pdata = (uint8_t*)ps_malloc(size);
    size_t read = file.readBytes((char*)pdata, size);
    file.close();

    // fixup a font pointer (using the allocated memory)
    pBinFont = FixBinaryFont(pdata);

    // GFX text output, as usual, with the new font
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 16);
    tft.setTextSize(1);
    // show the equivalent font included via header file
    tft.setFont(&FreeMonoBold12pt7b_);
    tft.setTextColor(ILI9341_GREEN);
    tft.println(ascii);
    // show the binary font
    // (with the '~' replaced with a heart glyph using the GFX Font Editor)
    tft.setFont(pBinFont);
    tft.setTextColor(ILI9341_ORANGE);
    tft.println(ascii);

    Serial.println(F("Setup done"));
}

void loop(void)
{
}
