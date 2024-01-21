#include "FontDevice.h"

#include "Debugging.h"

namespace Graphics {
    FontDevice::FontDevice() {
        if (Instance) return;
        if (const int error = FT_Init_FreeType(&libHandle)) {
            LOG("[ERR]: freetype init failed with error code " << error);
            return;
        }
        Instance = this;
    }

    FontDevice::~FontDevice() {
        FT_Done_FreeType(libHandle);
    }

    void FontDevice::Transfer(FontDevice& dest, FontDevice&& from) {
        dest.libHandle = from.libHandle;
        from.libHandle = nullptr;
        Instance = &dest;

        dest.dpi = from.dpi;
    }
}
