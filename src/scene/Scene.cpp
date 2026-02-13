#include "Scene.h"

Scene::Scene(const std::string& name)
    : m_name(name)
{
}

Scene::~Scene()
{
    // 清理所有游戏对象
    for (GameObject* gameObject : m_gameObjects) {
        delete gameObject;
    }
    m_gameObjects.clear();
}

void Scene::addGameObject(GameObject* gameObject)
{
    if (gameObject) {
        m_gameObjects.push_back(gameObject);
    }
}

void Scene::removeGameObject(GameObject* gameObject)
{
    if (!gameObject) return;

    auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
    if (it != m_gameObjects.end()) {
        delete *it;
        m_gameObjects.erase(it);
    }
}

void Scene::removeGameObject(const std::string& name)
{
    auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
        [name](GameObject* gameObject) {
            return gameObject->getName() == name;
        });

    if (it != m_gameObjects.end()) {
        delete *it;
        m_gameObjects.erase(it);
    }
}

GameObject* Scene::getGameObject(const std::string& name) const
{
    auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
        [name](GameObject* gameObject) {
            return gameObject->getName() == name;
        });

    if (it != m_gameObjects.end()) {
        return *it;
    }

    return nullptr;
}

void Scene::update(float deltaTime)
{
    // 更新所有游戏对象
    for (GameObject* gameObject : m_gameObjects) {
        gameObject->update(deltaTime);
    }
}

void Scene::render()
{
    // 渲染所有游戏对象
    for (GameObject* gameObject : m_gameObjects) {
        gameObject->render();
    }
}
