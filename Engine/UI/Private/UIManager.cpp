#include "UI/UIManager.h"
#include "Core/Debug.h"
#include "UI/UIWindow.h"

namespace atlas
{
UIManager::UIManager() { }

UIManager::~UIManager() { }

UIManager& UIManager::instance()
{
    static UIManager _instance;
    assert(!_instance._released);
    return _instance;
}

bool UIManager::addWindow(UIWindow* window)
{
    assert(window);
    return _windows.emplace(std::make_pair(window->windowId(), window)).second;
}

void UIManager::removeWindow(hq::StringHash windowId)
{
    _windows.erase(windowId);
}

void UIManager::update(float deltaTime)
{
    for (auto& pair : _windows)
    {
        if (!pair.second->update(deltaTime))
        {
            _closedWindows.emplace_back(pair.second->windowId());
        }
    }

    for (auto id : _closedWindows)
    {
        _windows.erase(id);
    }
    _closedWindows.clear();
}

void UIManager::release() { 
    _windows.clear();
    _released = true;
}

}  // atlas
