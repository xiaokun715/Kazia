#ifndef MESH_H
#define MESH_H

#include <filament/Engine.h>
#include <filament/Material.h>
#include <filament/Mesh.h>

class Mesh
{
private:
    filament::Engine* m_engine;
    filament::Mesh* m_mesh;
    filament::Material* m_material;
    filament::MaterialInstance* m_materialInstance;

public:
    Mesh(filament::Engine* engine);
    ~Mesh();

    void createCube(float size = 1.0f);
    void createSphere(float radius = 1.0f, int segments = 32);
    void createCylinder(float radius = 1.0f, float height = 2.0f, int segments = 32);

    filament::Mesh* getMesh() const { return m_mesh; }
    filament::MaterialInstance* getMaterialInstance() const { return m_materialInstance; }

    void setMaterial(filament::Material* material);
    void setColor(const filament::math::float4& color);

private:
    void cleanup();
};

#endif // MESH_H
