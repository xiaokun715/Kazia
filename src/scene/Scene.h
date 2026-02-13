#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "GameObject.h"

class Scene
{
private:
    std::vector<GameObject*> m_gameObjects;
    std::string m_name;

public:
    Scene(const std::string& name = "Scene");
    ~Scene();

    void addGameObject(GameObject* gameObject);
    void removeGameObject(GameObject* gameObject);
    void removeGameObject(const std::string& name);
    GameObject* getGameObject(const std::string& name) const;
    std::vector<GameObject*> getGameObjects() const { return m_gameObjects; }

    void update(float deltaTime);
    void render();

    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
};

#endif // SCENE_H
