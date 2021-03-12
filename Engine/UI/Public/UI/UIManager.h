#pragma once

#include "Hq/StringHash.h"
#include <unordered_map>
#include <memory>

namespace atlas
{
class UIWindow;

class UIManager
{
    friend class Engine;
public:
    ~UIManager();

    static UIManager& instance();

    // takes ownership of the window
    bool addWindow(UIWindow* window);
    void removeWindow(hq::StringHash windowId);
    void update(float deltaTime);
    void setStyle();


private:
    UIManager();
    void release();

    std::unordered_map<hq::StringHash, std::unique_ptr<UIWindow> > _windows;
    std::vector<hq::StringHash>                                    _closedWindows;
    bool                                                           _released {false};
};

}  // atlas
