#ifndef EDITOR_APP_H
#define EDITOR_APP_H

#include <memory>

class World;
class EditorContext;
class IRenderBackend;
class IViewport;
class MainWindow;

class EditorApp {
public:
    EditorApp();
    ~EditorApp();

    bool init();      // 核心：初始化整个编辑器
    void shutdown();  // 有序释放

private:
    bool initCore();
    bool initEditorLayer();
    bool initRenderLayer();
    bool initUI();

private:
    // ===== Core =====
    std::unique_ptr<World> world_;

    // ===== Editor =====
    std::unique_ptr<EditorContext> editorContext_;

    // ===== Render =====
    std::unique_ptr<IRenderBackend> renderBackend_;
    std::unique_ptr<IViewport> mainViewport_;

    // ===== UI =====
    MainWindow* mainWindow_ = nullptr;
};


#endif // EDITOR_APP_H