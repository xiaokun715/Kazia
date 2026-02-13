#include "CommandManager.h"

namespace Kazia {

CommandManager::CommandManager(size_t maxStackSize) : m_maxStackSize(maxStackSize) {
}

bool CommandManager::executeCommand(std::unique_ptr<ICommand> command) {
    if (!command) {
        return false;
    }
    
    // 执行命令
    if (!command->execute()) {
        return false;
    }
    
    // 添加到撤销栈
    m_undoStack.push_back(std::move(command));
    
    // 如果撤销栈超过最大大小，移除最早的命令
    if (m_undoStack.size() > m_maxStackSize) {
        m_undoStack.erase(m_undoStack.begin());
    }
    
    // 清空重做栈
    m_redoStack.clear();
    
    return true;
}

bool CommandManager::undo() {
    if (m_undoStack.empty()) {
        return false;
    }
    
    // 获取栈顶命令
    auto& command = m_undoStack.back();
    
    // 撤销命令
    if (!command->undo()) {
        return false;
    }
    
    // 将命令移到重做栈
    m_redoStack.push_back(std::move(command));
    m_undoStack.pop_back();
    
    return true;
}

bool CommandManager::redo() {
    if (m_redoStack.empty()) {
        return false;
    }
    
    // 获取栈顶命令
    auto& command = m_redoStack.back();
    
    // 执行命令
    if (!command->execute()) {
        return false;
    }
    
    // 将命令移到撤销栈
    m_undoStack.push_back(std::move(command));
    m_redoStack.pop_back();
    
    return true;
}

void CommandManager::clear() {
    m_undoStack.clear();
    m_redoStack.clear();
}

} // namespace Kazia
