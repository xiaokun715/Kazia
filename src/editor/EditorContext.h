#ifndef EDITOR_CONTEXT_H
#define EDITOR_CONTEXT_H

class World;
class SelectionSystem;
class CommandStack;
class ToolSystem;

class EditorContext {
public:
    void setWorld(World* world);

    void initSelectionSystem();
    void initCommandStack();
    void initToolSystem();

    SelectionSystem& selection();
    CommandStack& commandStack();
    ToolSystem& toolSystem();

private:
    World* world_ = nullptr;

    SelectionSystem* selection_ = nullptr;
    CommandStack* commandStack_ = nullptr;
    ToolSystem* toolSystem_ = nullptr;
};

#endif // !EDITOR_CONTEXT_H