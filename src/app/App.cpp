//#include "EditorApp.h"
//
//// Core
//#include "core/World.h"
//
//// Editor
//#include "editor/EditorContext.h"
//
//// Render
////#include "render/IRenderBackend.h"
////#include "render/OpenGL/FilamentOpenGLBackend.h"
////#include "render/viewport/OpenGLViewport.h"
//
//// UI
//#include "ui/MainWindow.h"
//
//EditorApp::EditorApp() = default;
//EditorApp::~EditorApp() = default;
//
//bool EditorApp::init()
//{
//    // 初始化顺序极其重要
//    if (!initCore())
//        return false;
//
//    if (!initEditorLayer())
//        return false;
//
//    if (!initRenderLayer())
//        return false;
//
//    if (!initUI())
//        return false;
//
//    return true;
//}
//
//bool EditorApp::initCore()
//{
//    world_ = std::make_unique<World>();
//
//    // 创建默认 Scene / Camera / Light
//    auto& scene = world_->scene();
//
//    Entity camera = scene.createEntity();
//    scene.addCamera(camera);
//    scene.addTransform(camera);
//
//    Entity light = scene.createEntity();
//    scene.addLight(light);
//    scene.addTransform(light);
//
//    return true;
//}
//
//bool EditorApp::initEditorLayer()
//{
//    editorContext_ = std::make_unique<EditorContext>();
//
//    editorContext_->setWorld(world_.get());
//
//    // Selection / Command / Undo
//    editorContext_->initSelectionSystem();
//    editorContext_->initCommandStack();
//    editorContext_->initToolSystem();
//
//    // 默认工具
//    editorContext_->toolSystem().setActiveTool("Move");
//
//    return true;
//}
//
//bool EditorApp::initRenderLayer()
//{
//    // 1. 创建 Backend（还不绑定窗口）
//    renderBackend_ = std::make_unique<FilamentOpenGLBackend>();
//
//    // 2. 创建 Viewport（Qt Widget）
//    mainViewport_ = std::make_unique<OpenGLViewport>(renderBackend_.get());
//
//    // 3. 连接 SceneSyncSystem
//    renderBackend_->setWorld(world_.get());
//
//    return true;
//}
//
//bool EditorApp::initUI()
//{
//    mainWindow_ = new MainWindow;
//
//    // 设置中央 Viewport
//    mainWindow_->setViewportWidget(
//        dynamic_cast<QWidget*>(mainViewport_.get())
//    );
//
//    // 注入 EditorContext（事件 → 编辑语义）
//    mainWindow_->setEditorContext(editorContext_.get());
//
//    mainWindow_->show();
//    return true;
//}
