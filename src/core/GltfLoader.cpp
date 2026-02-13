#include "GltfLoader.h"

#include "AssetManager.h"
#include "Mesh.h"

#include <tiny_gltf.h>

#include <fstream>
#include <sstream>
#include <filesystem>

namespace Kazia {

GltfLoader::GltfLoader(filament::Engine* engine, AssetManager* assetManager) 
    : m_engine(engine), 
      m_assetManager(assetManager) {
}

bool GltfLoader::loadFromFile(const std::string& path, std::vector<std::shared_ptr<Mesh>>& meshes) {
    return parseGltfFile(path, meshes);
}

bool GltfLoader::parseGltfFile(const std::string& path, std::vector<std::shared_ptr<Mesh>>& meshes) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    // 解析 glTF 文件
    std::filesystem::path filePath(path);
    std::string extension = filePath.extension().string();
    
    bool ret = false;
    if (extension == ".gltf") {
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, path.c_str());
    } else if (extension == ".glb") {
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, path.c_str());
    }
    
    if (!warn.empty()) {
        // 输出警告
        std::cout << "GLTF Loader Warning: " << warn << std::endl;
    }
    
    if (!ret) {
        // 输出错误
        std::cout << "GLTF Loader Error: " << err << std::endl;
        return false;
    }
    
    // 加载纹理
    std::string basePath = filePath.parent_path().string();
    loadTextures(basePath, nullptr, 0);
    
    // 加载材质
    loadMaterials(nullptr, 0);
    
    // 加载网格
    // 这里需要从 model 中提取网格数据
    // 暂时使用简化实现
    for (const auto& mesh : model.meshes) {
        auto newMesh = std::make_shared<Mesh>();
        // 从 model 中提取网格数据并填充到 newMesh
        meshes.push_back(newMesh);
    }
    
    return true;
}

bool GltfLoader::loadTextures(const std::string& basePath, const void* gltfData, size_t gltfSize) {
    // 暂时使用简化实现
    return true;
}

bool GltfLoader::loadMaterials(const void* gltfData, size_t gltfSize) {
    // 暂时使用简化实现
    return true;
}

bool GltfLoader::loadMeshes(const void* gltfData, size_t gltfSize, std::vector<std::shared_ptr<Mesh>>& meshes) {
    // 暂时使用简化实现
    return true;
}

} // namespace Kazia
