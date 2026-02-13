#include "PluginManager.h"

#include <filesystem>
#include <iostream>

// 动态库加载相关
#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace Kazia {

PluginManager::PluginManager() : m_pluginDirectory("plugins") {
}

PluginManager::~PluginManager() {
    shutdownPlugins();
}

void PluginManager::loadPlugins() {
    // 确保插件目录存在
    std::filesystem::path pluginDir(m_pluginDirectory);
    if (!std::filesystem::exists(pluginDir)) {
        std::cout << "Plugin directory does not exist: " << m_pluginDirectory << std::endl;
        return;
    }
    
    // 遍历插件目录
    for (const auto& entry : std::filesystem::directory_iterator(pluginDir)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            
            // 检查是否为动态库文件
            if (extension == ".dll" || extension == ".so" || extension == ".dylib") {
                // 加载插件
                loadPlugin(entry.path().string());
            }
        }
    }
}

void PluginManager::initializePlugins() {
    for (auto& plugin : m_plugins) {
        if (plugin->initialize()) {
            std::cout << "Initialized plugin: " << plugin->getName() << " v" << plugin->getVersion() << std::endl;
            plugin->registerMenuItems();
        } else {
            std::cout << "Failed to initialize plugin: " << plugin->getName() << std::endl;
        }
    }
}

void PluginManager::shutdownPlugins() {
    for (auto& plugin : m_plugins) {
        plugin->shutdown();
        std::cout << "Shutdown plugin: " << plugin->getName() << std::endl;
    }
    
    m_plugins.clear();
    m_pluginMap.clear();
}

IPlugin* PluginManager::getPlugin(const std::string& name) const {
    auto it = m_pluginMap.find(name);
    if (it != m_pluginMap.end()) {
        return it->second;
    }
    return nullptr;
}

bool PluginManager::loadPlugin(const std::string& pluginPath) {
    std::cout << "Loading plugin: " << pluginPath << std::endl;
    
    // 加载动态库
    #ifdef _WIN32
        HMODULE handle = LoadLibraryA(pluginPath.c_str());
        if (!handle) {
            std::cout << "Failed to load plugin: " << GetLastError() << std::endl;
            return false;
        }
        
        // 获取插件创建函数
        typedef IPlugin* (*CreatePluginFunc)();
        CreatePluginFunc createPlugin = (CreatePluginFunc)GetProcAddress(handle, "createPlugin");
        if (!createPlugin) {
            std::cout << "Failed to get createPlugin function: " << GetLastError() << std::endl;
            FreeLibrary(handle);
            return false;
        }
        
        // 创建插件实例
        IPlugin* plugin = createPlugin();
        if (!plugin) {
            std::cout << "Failed to create plugin instance" << std::endl;
            FreeLibrary(handle);
            return false;
        }
        
        // 注意：这里不释放 handle，因为插件实例可能在运行时需要使用库中的其他函数
        // 实际项目中需要管理动态库的生命周期
    #else
        // 非 Windows 平台的实现
        void* handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cout << "Failed to load plugin: " << dlerror() << std::endl;
            return false;
        }
        
        // 获取插件创建函数
        typedef IPlugin* (*CreatePluginFunc)();
        CreatePluginFunc createPlugin = (CreatePluginFunc)dlsym(handle, "createPlugin");
        if (!createPlugin) {
            std::cout << "Failed to get createPlugin function: " << dlerror() << std::endl;
            dlclose(handle);
            return false;
        }
        
        // 创建插件实例
        IPlugin* plugin = createPlugin();
        if (!plugin) {
            std::cout << "Failed to create plugin instance" << std::endl;
            dlclose(handle);
            return false;
        }
        
        // 注意：这里不释放 handle，因为插件实例可能在运行时需要使用库中的其他函数
        // 实际项目中需要管理动态库的生命周期
    #endif
    
    // 添加到插件列表
    m_plugins.push_back(std::unique_ptr<IPlugin>(plugin));
    m_pluginMap[plugin->getName()] = plugin;
    
    std::cout << "Loaded plugin: " << plugin->getName() << " v" << plugin->getVersion() << std::endl;
    return true;
}

} // namespace Kazia
