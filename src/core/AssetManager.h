#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <atomic>

#include <filament/Engine.h>
#include <filament/Material.h>
#include <filament/Texture.h>

namespace Kazia {

class Mesh;

class AssetManager {
private:
    filament::Engine* m_engine;
    
    // 缓存结构
    struct MeshCacheItem {
        std::shared_ptr<Mesh> mesh;
        std::atomic<int> refCount;
    };
    
    struct TextureCacheItem {
        filament::Texture* texture;
        std::atomic<int> refCount;
    };
    
    struct MaterialCacheItem {
        filament::Material* material;
        std::atomic<int> refCount;
    };
    
    // 缓存
    std::unordered_map<std::string, MeshCacheItem> m_meshCache;
    std::unordered_map<std::string, TextureCacheItem> m_textureCache;
    std::unordered_map<std::string, MaterialCacheItem> m_materialCache;
    
public:
    AssetManager(filament::Engine* engine);
    ~AssetManager();
    
    // 网格相关
    std::shared_ptr<Mesh> loadMesh(const std::string& path);
    void releaseMesh(const std::string& path);
    
    // 纹理相关
    filament::Texture* loadTexture(const std::string& path);
    void releaseTexture(const std::string& path);
    
    // 材质相关
    filament::Material* loadMaterial(const std::string& path);
    void releaseMaterial(const std::string& path);
    
    // 清理相关
    void clearAllCaches();
    void clearUnusedAssets();
    
private:
    // 辅助方法
    std::string getAssetHash(const std::string& path) const;
};

} // namespace Kazia

#endif // ASSETMANAGER_H
