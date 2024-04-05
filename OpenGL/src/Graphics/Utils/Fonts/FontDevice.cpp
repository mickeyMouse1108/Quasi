#include "FontDevice.h"

#include "GLDebug.h"

#include "ft2build.h"
#include FT_FREETYPE_H

namespace Graphics {
    OPENGL_API FontDevice* FontDevice::Instance = nullptr;

    FontDevice::FontDevice() {
        if (Instance) return;
        if (const int error = FT_Init_FreeType(&libHandle)) {
            GLLogger().Error({"Freetype Init failed with err code {}"}, error);
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

    FT_Library FontDevice::Library() { return Instance->libHandle; }
    uint FontDevice::DPI() { return Instance->dpi; }
}
