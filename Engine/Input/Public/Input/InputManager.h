#pragma once

#include <Hq/Math/MathTypes.h>
#include <Hq/Flags.h>

union SDL_Event;

namespace atlas
{
class InputManager
{
    friend class SDLWindow;
    friend class Engine;

public:
    enum class EMouseButtons : u8
    {
        None,
        Left   = 0x01,
        Right  = 0x02,
        Middle = 0x04,
    };
    using MouseButtonFlags = hq::Flags<EMouseButtons>;

    static InputManager& instance();

    float horizontalAxis() const;
    float verticalAxis() const;
    float mouseHorizontalAxis() const;
    float mouseVerticalAxis() const;

    bool             mouseDown(MouseButtonFlags flags) const;
    bool             mouseClick(MouseButtonFlags flags) const;
    hq::math::Vec2 mousePos() const;

    void handleInputEvent(const SDL_Event& e);
    void resetInput();

private:
    InputManager();

    bool                     _registerMouse {false};
    mutable MouseButtonFlags _down {EMouseButtons::None};
    mutable MouseButtonFlags _clicked {EMouseButtons::None};
    float                    _hAxis {0.f};
    float                    _vAxis {0.f};
    float                    _mhAxis {0.f};
    float                    _mvAxis {0.f};
};

}  // atlas
