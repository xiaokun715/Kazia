#ifndef I_RENDER_BACKEND_H
#define I_RENDER_BACKEND_H

#include "ui/viewport/IViewPortSurface.h"
#include <cstdint>

class IRenderBackend {
public:
    virtual ~IRenderBackend() = default;

    virtual void attachSurface(IViewportSurface* surface) = 0;
    virtual void detachSurface() = 0;

    virtual void resize(uint32_t w, uint32_t h) = 0;
    virtual void renderFrame() = 0;
};

#endif // !I_RENDER_BACKEND_H
