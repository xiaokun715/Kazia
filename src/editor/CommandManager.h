#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <vector>
#include <memory>

#include "ICommand.h"

namespace Kazia {

class CommandManager {
private:
    std::vector<std::unique_ptr<ICommand>> m_undoStack;
    std::vector<std::unique_ptr<ICommand>> m_redoStack;
    size_t m_maxStackSize;
    
public:
    CommandManager(size_t maxStackSize = 100);
    ~CommandManager() = default;
    
    // 执行命令
    bool executeCommand(std::unique_ptr<ICommand> command);
    
    // 撤销命令
    bool undo();
    
    // 重做命令
    bool redo();
    
    // 清理命令栈
    void clear();
    
    // 获取命令栈状态
    bool canUndo() const { return !m_undoStack.empty(); }
    bool canRedo() const { return !m_redoStack.empty(); }
    size_t getUndoStackSize() const { return m_undoStack.size(); }
    size_t getRedoStackSize() const { return m_redoStack.size(); }
};

} // namespace Kazia

#endif // COMMANDMANAGER_H
