#include "MaterialManager.h"
#include <filament/MaterialReader.h>

MaterialManager::MaterialManager(filament::Engine* engine)
    : m_engine(engine)
{
}

MaterialManager::~MaterialManager()
{
    destroyAllMaterials();
}

filament::Material* MaterialManager::createMaterial(const std::string& name, const std::string& materialPath)
{
    // 检查材质是否已存在
    auto it = m_materials.find(name);
    if (it != m_materials.end()) {
        return it->second;
    }

    // 加载材质
    filament::Material* material = loadMaterialFromPath(materialPath);
    if (material) {
        m_materials[name] = material;
    }

    return material;
}

filament::Material* MaterialManager::getMaterial(const std::string& name) const
{
    auto it = m_materials.find(name);
    if (it != m_materials.end()) {
        return it->second;
    }
    return nullptr;
}

filament::MaterialInstance* MaterialManager::createMaterialInstance(const std::string& materialName)
{
    filament::Material* material = getMaterial(materialName);
    if (material) {
        return material->createInstance();
    }
    return nullptr;
}

void MaterialManager::destroyMaterial(const std::string& name)
{
    auto it = m_materials.find(name);
    if (it != m_materials.end()) {
        m_engine->destroy(it->second);
        m_materials.erase(it);
    }
}

void MaterialManager::destroyAllMaterials()
{
    for (auto& pair : m_materials) {
        m_engine->destroy(pair.second);
    }
    m_materials.clear();
}

filament::Material* MaterialManager::loadMaterialFromPath(const std::string& path)
{
    // 简化实现，实际项目中可能需要更复杂的材质加载逻辑
    // 这里使用默认的 PBR 材质
    // 注意：实际使用时需要提供有效的材质文件路径
    return nullptr;
}
