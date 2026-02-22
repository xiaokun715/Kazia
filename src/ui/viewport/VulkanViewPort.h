#ifndef VULKAN_VIEWPORT_H
#define VULKAN_VIEWPORT_H

#include "IViewPort.h"
#include "IViewPortSurface.h"
#include "render/IRenderBackend.h"
#include <QVulkanWindow>

class VulkanViewport final
    : public QVulkanWindow
    , public IViewportSurface
    , public IViewport
{
public:
    VulkanViewport(IRenderBackend* backend);

    // IViewport
    void initialize() override;
    void resize(uint32_t w, uint32_t h) override;
    void render() override;

    // IViewportSurface
    void* nativeHandle() const override;
    uint32_t width() const override;
    uint32_t height() const override;
    bool isValid() const override;

protected:
    //void initResources() override;
    //void startNextFrame() override;

private:
    IRenderBackend* backend_;
};

#endif // !VULKAN_VIEWPORT_H
