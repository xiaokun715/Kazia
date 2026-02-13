#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <string>
#include <vector>

namespace Kazia {

class IPlugin {
public:
    virtual ~IPlugin() = default;
    
    // 插件信息
    virtual std::string getName() const = 0;
    virtual std::string getVersion() const = 0;
    virtual std::string getDescription() const = 0;
    
    // 生命周期方法
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    // 注册菜单项
    virtual void registerMenuItems() = 0;
};

} // namespace Kazia

#endif // IPLUGIN_H
