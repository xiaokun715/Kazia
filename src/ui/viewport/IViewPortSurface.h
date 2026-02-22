#ifndef I_VIEWPORT_SURFACE_H
#define I_VIEWPORT_SURFACE_H

#include <cstdint>

class IViewportSurface {
public:
    virtual ~IViewportSurface() = default;

    virtual void* nativeHandle() const = 0;
    virtual uint32_t width() const = 0;
    virtual uint32_t height() const = 0;

    virtual bool isValid() const = 0;
};

#endif // !I_VIEWPORT_SURFACE_H
