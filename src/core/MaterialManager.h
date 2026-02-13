#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <filament/Engine.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <unordered_map>
#include <string>

class MaterialManager
{
private:
    filament::Engine* m_engine;
    std::unordered_map<std::string, filament::Material*> m_materials;

public:
    MaterialManager(filament::Engine* engine);
    ~MaterialManager();

    filament::Material* createMaterial(const std::string& name, const std::string& materialPath);
    filament::Material* getMaterial(const std::string& name) const;
    filament::MaterialInstance* createMaterialInstance(const std::string& materialName);

    void destroyMaterial(const std::string& name);
    void destroyAllMaterials();

private:
    filament::Material* loadMaterialFromPath(const std::string& path);
};

#endif // MATERIALMANAGER_H
