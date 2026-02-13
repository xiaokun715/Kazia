#include "AssetManager.h"

#include "Mesh.h"

#include <filament/Texture.h>
#include <filament/Material.h>

#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/md5.h>

namespace Kazia {

AssetManager::AssetManager(filament::Engine* engine) : m_engine(engine) {
}

AssetManager::~AssetManager() {
    clearAllCaches();
}

std::shared_ptr<Mesh> AssetManager::loadMesh(const std::string& path) {
    std::string hash = getAssetHash(path);
    
    // 检查缓存中是否已存在
    auto it = m_meshCache.find(hash);
    if (it != m_meshCache.end()) {
        // 增加引用计数
        it->second.refCount++;
        return it->second.mesh;
    }
    
    // 加载新网格
    auto mesh = std::make_shared<Mesh>();
    if (mesh->loadFromFile(path)) {
        // 添加到缓存
        m_meshCache[hash] = {mesh, 1};
        return mesh;
    }
    
    return nullptr;
}

void AssetManager::releaseMesh(const std::string& path) {
    std::string hash = getAssetHash(path);
    
    auto it = m_meshCache.find(hash);
    if (it != m_meshCache.end()) {
        // 减少引用计数
        it->second.refCount--;
        
        // 如果引用计数为 0，从缓存中移除
        if (it->second.refCount <= 0) {
            m_meshCache.erase(it);
        }
    }
}

filament::Texture* AssetManager::loadTexture(const std::string& path) {
    std::string hash = getAssetHash(path);
    
    // 检查缓存中是否已存在
    auto it = m_textureCache.find(hash);
    if (it != m_textureCache.end()) {
        // 增加引用计数
        it->second.refCount++;
        return it->second.texture;
    }
    
    // 加载新纹理
    // 这里使用 Filament 的 Texture::Builder 来创建纹理
    // 暂时返回 nullptr，实际项目中需要实现具体的纹理加载逻辑
    filament::Texture* texture = nullptr;
    
    if (texture) {
        // 添加到缓存
        m_textureCache[hash] = {texture, 1};
    }
    
    return texture;
}

void AssetManager::releaseTexture(const std::string& path) {
    std::string hash = getAssetHash(path);
    
    auto it = m_textureCache.find(hash);
    if (it != m_textureCache.end()) {
        // 减少引用计数
        it->second.refCount--;
        
        // 如果引用计数为 0，从缓存中移除并销毁
        if (it->second.refCount <= 0) {
            if (m_engine && it->second.texture) {
                m_engine->destroy(it->second.texture);
            }
            m_textureCache.erase(it);
        }
    }
}

filament::Material* AssetManager::loadMaterial(const std::string& path) {
    std::string hash = getAssetHash(path);
    
    // 检查缓存中是否已存在
    auto it = m_materialCache.find(hash);
    if (it != m_materialCache.end()) {
        // 增加引用计数
        it->second.refCount++;
        return it->second.material;
    }
    
    // 加载新材质
    // 这里使用 Filament 的 Material::Builder 来创建材质
    // 暂时返回 nullptr，实际项目中需要实现具体的材质加载逻辑
    filament::Material* material = nullptr;
    
    if (material) {
        // 添加到缓存
        m_materialCache[hash] = {material, 1};
    }
    
    return material;
}

void AssetManager::releaseMaterial(const std::string& path) {
    std::string hash = getAssetHash(path);
    
    auto it = m_materialCache.find(hash);
    if (it != m_materialCache.end()) {
        // 减少引用计数
        it->second.refCount--;
        
        // 如果引用计数为 0，从缓存中移除并销毁
        if (it->second.refCount <= 0) {
            if (m_engine && it->second.material) {
                m_engine->destroy(it->second.material);
            }
            m_materialCache.erase(it);
        }
    }
}

void AssetManager::clearAllCaches() {
    // 清理网格缓存
    m_meshCache.clear();
    
    // 清理纹理缓存
    for (auto& item : m_textureCache) {
        if (m_engine && item.second.texture) {
            m_engine->destroy(item.second.texture);
        }
    }
    m_textureCache.clear();
    
    // 清理材质缓存
    for (auto& item : m_materialCache) {
        if (m_engine && item.second.material) {
            m_engine->destroy(item.second.material);
        }
    }
    m_materialCache.clear();
}

void AssetManager::clearUnusedAssets() {
    // 清理未使用的网格
    for (auto it = m_meshCache.begin(); it != m_meshCache.end();) {
        if (it->second.refCount <= 0) {
            it = m_meshCache.erase(it);
        } else {
            ++it;
        }
    }
    
    // 清理未使用的纹理
    for (auto it = m_textureCache.begin(); it != m_textureCache.end();) {
        if (it->second.refCount <= 0) {
            if (m_engine && it->second.texture) {
                m_engine->destroy(it->second.texture);
            }
            it = m_textureCache.erase(it);
        } else {
            ++it;
        }
    }
    
    // 清理未使用的材质
    for (auto it = m_materialCache.begin(); it != m_materialCache.end();) {
        if (it->second.refCount <= 0) {
            if (m_engine && it->second.material) {
                m_engine->destroy(it->second.material);
            }
            it = m_materialCache.erase(it);
        } else {
            ++it;
        }
    }
}

std::string AssetManager::getAssetHash(const std::string& path) const {
    // 使用 MD5 哈希生成唯一标识符
    unsigned char md5_hash[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(path.c_str()), path.length(), md5_hash);
    
    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md5_hash[i]);
    }
    
    return ss.str();
}

} // namespace Kazia
