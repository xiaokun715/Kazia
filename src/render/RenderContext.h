#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <memory>
#include <string>

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <filament/SwapChain.h>

#include "FilamentEntityMapper.h"

namespace Kazia {

class RenderContext {
public:
    RenderContext() = default;
    ~RenderContext() = default;
    
    // Filament 引擎组件
    filament::Engine* engine = nullptr;
    filament::Renderer* renderer = nullptr;
    filament::Scene* scene = nullptr;
    filament::View* view = nullptr;
    filament::Camera* camera = nullptr;
    filament::SwapChain* swapChain = nullptr;
    
    // 窗口信息
    int width = 0;
    int height = 0;
    
    // 清除选项
    filament::Renderer::ClearOptions clearOptions;
    
    // 实体映射器
    std::unique_ptr<FilamentEntityMapper> entityMapper;
    
    // 检查是否有效
    bool isValid() const {
        return engine != nullptr && renderer != nullptr && scene != nullptr && view != nullptr && camera != nullptr;
    }
};

} // namespace Kazia

#endif // RENDERCONTEXT_H
