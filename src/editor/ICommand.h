#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <string>

namespace Kazia {

class ICommand {
public:
    virtual ~ICommand() = default;
    
    // 执行命令
    virtual bool execute() = 0;
    
    // 撤销命令
    virtual bool undo() = 0;
    
    // 获取命令名称
    virtual std::string getName() const = 0;
};

} // namespace Kazia

#endif // ICOMMAND_H
