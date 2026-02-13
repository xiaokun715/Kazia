#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "IPlugin.h"

namespace Kazia {

class PluginManager {
private:
    std::vector<std::unique_ptr<IPlugin>> m_plugins;
    std::unordered_map<std::string, IPlugin*> m_pluginMap;
    std::string m_pluginDirectory;
    
public:
    PluginManager();
    ~PluginManager();
    
    // 设置插件目录
    void setPluginDirectory(const std::string& directory) { m_pluginDirectory = directory; }
    
    // 扫描并加载插件
    void loadPlugins();
    
    // 初始化所有插件
    void initializePlugins();
    
    // 关闭所有插件
    void shutdownPlugins();
    
    // 获取插件
    IPlugin* getPlugin(const std::string& name) const;
    
    // 获取所有插件
    const std::vector<std::unique_ptr<IPlugin>>& getPlugins() const { return m_plugins; }
    
private:
    // 加载单个插件
    bool loadPlugin(const std::string& pluginPath);
};

} // namespace Kazia

#endif // PLUGINMANAGER_H
