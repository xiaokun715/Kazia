#ifndef I_VIEWPORT_H
#define I_VIEWPORT_H

#include <cstdint>
#include "IViewPortSurface.h"

class IViewport {
public:
    virtual ~IViewport() = default;

    virtual void initialize() = 0;
    virtual void resize(uint32_t w, uint32_t h) = 0;
    virtual void render() = 0;

    virtual IViewportSurface* surface() = 0;
};

#endif // !I_VIEWPORT_H
