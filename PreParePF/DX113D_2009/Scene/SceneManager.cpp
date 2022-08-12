#include "Framework.h"
#include "SceneManager.h"


SceneManager::SceneManager()
    :curScene(nullptr)
{
}
SceneManager::~SceneManager()
{
    for (auto scene : scenes)
        delete scene.second;
}

void SceneManager::Update()
{
    if (curScene == nullptr)
        return;
    curScene->Update();
}

void SceneManager::Render()
{
    if (curScene == nullptr)
        return;
    curScene->Render();
}

void SceneManager::PostRender()
{
    if (curScene == nullptr)
        return;
    curScene->PostRender();
}

void SceneManager::PreRender()
{
    if (curScene == nullptr)
        return;
    curScene->PreRender();
}

Scene* SceneManager::Add(int key, Scene* scene)
{
    if (scenes.count(key) > 0)
        return scenes[key];

    scenes[key] = scene;
    return scene;
}

//void SceneManager::ChangeScene(int key, Vector3 pos)
//{
//    if (scenes.count(key) == 0)
//        return;
//    if (scenes[key] == curScene)
//        return;
//    if (curScene != nullptr)
//        curScene->End();
//
//
//    curScene = scenes[key];
//    curScene->Start(pos);
//}

void SceneManager::ChangeScene(int key)
{
    if (scenes.count(key) == 0)
        return;
    if (scenes[key] == curScene)
        return;
    if (curScene != nullptr)
        curScene->End();


    curScene = scenes[key];
    curScene->Start();
}

//void SceneManager::ChangeScene(int loadingKey, int key, Vector2 pos)
//{
//    if (scenes.count(loadingKey) == 0)
//        return;
//    if (scenes[loadingKey] == curScene)
//        return;
//    if (curScene != nullptr)
//        curScene->End();
//
//
//    curScene = scenes[loadingKey];
//    curScene->Start(pos, key);
//}
