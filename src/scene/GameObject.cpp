#include "GameObject.h"

GameObject::GameObject(const std::string& name)
    : m_name(name)
    , m_position({0.0f, 0.0f, 0.0f})
    , m_rotation({0.0f, 0.0f, 0.0f})
    , m_scale({1.0f, 1.0f, 1.0f})
{
}

GameObject::~GameObject()
{
}

void GameObject::update(float deltaTime)
{
    // 基类默认实现，子类可以重写
}

void GameObject::render()
{
    // 基类默认实现，子类可以重写
}
