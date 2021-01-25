#pragma once

#include <Hq/Math/MathTypes.h>

union SDL_Event;

namespace atlas
{
class InputManager
{
    friend class SDLWindow;

public:
    InputManager();

    static InputManager& instance();

    float horizontalAxis() const;
    float verticalAxis() const;
    float mouseHorizontalAxis() const;
    float mouseVerticalAxis() const;

    bool             mouseDown() const;
    bool             mouseClick() const;
    ::hq::math::Vec2 mousePos() const;

    void handleInputEvent(const SDL_Event& e);
    void resetInput();

private:
    bool         _registerMouse {false};
    mutable bool _clicked {false};
    float        _hAxis {0.f};
    float        _vAxis {0.f};
    float        _mhAxis {0.f};
    float        _mvAxis {0.f};
};

}  // atlas
