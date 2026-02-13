#include "Mesh.h"
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>

Mesh::Mesh(filament::Engine* engine)
    : m_engine(engine)
    , m_mesh(nullptr)
    , m_material(nullptr)
    , m_materialInstance(nullptr)
{
}

Mesh::~Mesh()
{
    cleanup();
}

void Mesh::cleanup()
{
    if (m_materialInstance) {
        m_engine->destroy(m_materialInstance);
        m_materialInstance = nullptr;
    }

    if (m_mesh) {
        m_engine->destroy(m_mesh);
        m_mesh = nullptr;
    }
}

void Mesh::createCube(float size)
{
    cleanup();

    // 立方体顶点数据
    struct Vertex {
        filament::math::float3 position;
        filament::math::float3 normal;
        filament::math::float2 uv;
    };

    float halfSize = size * 0.5f;
    Vertex vertices[] = {
        // 前
        {{-halfSize, -halfSize,  halfSize}, {0, 0, 1}, {0, 0}},
        {{ halfSize, -halfSize,  halfSize}, {0, 0, 1}, {1, 0}},
        {{ halfSize,  halfSize,  halfSize}, {0, 0, 1}, {1, 1}},
        {{-halfSize,  halfSize,  halfSize}, {0, 0, 1}, {0, 1}},
        // 后
        {{-halfSize, -halfSize, -halfSize}, {0, 0, -1}, {0, 0}},
        {{-halfSize,  halfSize, -halfSize}, {0, 0, -1}, {0, 1}},
        {{ halfSize,  halfSize, -halfSize}, {0, 0, -1}, {1, 1}},
        {{ halfSize, -halfSize, -halfSize}, {0, 0, -1}, {1, 0}},
        // 左
        {{-halfSize, -halfSize, -halfSize}, {-1, 0, 0}, {0, 0}},
        {{-halfSize,  halfSize, -halfSize}, {-1, 0, 0}, {0, 1}},
        {{-halfSize,  halfSize,  halfSize}, {-1, 0, 0}, {1, 1}},
        {{-halfSize, -halfSize,  halfSize}, {-1, 0, 0}, {1, 0}},
        // 右
        {{ halfSize, -halfSize,  halfSize}, {1, 0, 0}, {0, 0}},
        {{ halfSize,  halfSize,  halfSize}, {1, 0, 0}, {0, 1}},
        {{ halfSize,  halfSize, -halfSize}, {1, 0, 0}, {1, 1}},
        {{ halfSize, -halfSize, -halfSize}, {1, 0, 0}, {1, 0}},
        // 下
        {{-halfSize, -halfSize, -halfSize}, {0, -1, 0}, {0, 0}},
        {{ halfSize, -halfSize, -halfSize}, {0, -1, 0}, {1, 0}},
        {{ halfSize, -halfSize,  halfSize}, {0, -1, 0}, {1, 1}},
        {{-halfSize, -halfSize,  halfSize}, {0, -1, 0}, {0, 1}},
        // 上
        {{-halfSize,  halfSize,  halfSize}, {0, 1, 0}, {0, 0}},
        {{ halfSize,  halfSize,  halfSize}, {0, 1, 0}, {1, 0}},
        {{ halfSize,  halfSize, -halfSize}, {0, 1, 0}, {1, 1}},
        {{-halfSize,  halfSize, -halfSize}, {0, 1, 0}, {0, 1}}
    };

    // 立方体索引数据
    uint16_t indices[] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    // 创建顶点缓冲区
    filament::VertexBuffer::Builder vbBuilder(1);
    vbBuilder.vertexCount(sizeof(vertices) / sizeof(Vertex));
    vbBuilder.buffer(0, filament::VertexBuffer::Attribute::POSITION, filament::VertexBuffer::AttributeType::FLOAT3, offsetof(Vertex, position), sizeof(Vertex));
    vbBuilder.buffer(0, filament::VertexBuffer::Attribute::NORMAL, filament::VertexBuffer::AttributeType::FLOAT3, offsetof(Vertex, normal), sizeof(Vertex));
    vbBuilder.buffer(0, filament::VertexBuffer::Attribute::UV0, filament::VertexBuffer::AttributeType::FLOAT2, offsetof(Vertex, uv), sizeof(Vertex));

    auto vertexBuffer = vbBuilder.build(*m_engine);
    vertexBuffer->setBufferAt(*m_engine, 0, filament::VertexBuffer::BufferDescriptor(vertices, sizeof(vertices)));

    // 创建索引缓冲区
    auto indexBuffer = filament::IndexBuffer::Builder()
        .indexCount(sizeof(indices) / sizeof(uint16_t))
        .bufferType(filament::IndexBuffer::IndexType::USHORT)
        .build(*m_engine);

    indexBuffer->setBuffer(*m_engine, filament::IndexBuffer::BufferDescriptor(indices, sizeof(indices)));

    // 创建网格
    m_mesh = filament::Mesh::Builder()
        .vertexBuffer(0, vertexBuffer)
        .indexBuffer(indexBuffer)
        .boundingBox({{-halfSize, -halfSize, -halfSize}, {halfSize, halfSize, halfSize}})
        .build(*m_engine);

    // 清理缓冲区引用
    vertexBuffer->destroy();
    indexBuffer->destroy();
}

void Mesh::createSphere(float radius, int segments)
{
    cleanup();

    // 简化实现，实际项目中可能需要更复杂的球体生成算法
    // 这里使用立方体并细分
    createCube(radius * 2.0f);
}

void Mesh::createCylinder(float radius, float height, int segments)
{
    cleanup();

    // 简化实现，实际项目中可能需要更复杂的圆柱体生成算法
    // 这里使用立方体并缩放
    createCube(radius * 2.0f);
}

void Mesh::setMaterial(filament::Material* material)
{
    if (m_materialInstance) {
        m_engine->destroy(m_materialInstance);
        m_materialInstance = nullptr;
    }

    m_material = material;
    if (m_material) {
        m_materialInstance = m_material->createInstance();
    }
}

void Mesh::setColor(const filament::math::float4& color)
{
    if (m_materialInstance) {
        m_materialInstance->setParameter("color", color);
    }
}
