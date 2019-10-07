#pragma once

union SDL_Event;

namespace atlas
{
class InputManager
{
    friend class SDLWindow;

public:
    InputManager();
    float horizontalAxis() const;
    float verticalAxis() const;
    float mouseHorizontalAxis() const;
    float mouseVerticalAxis() const;

    void handleInputEvent(const SDL_Event& e);
    void resetInput();

private:
    bool  _registerMouse {false};
    float _hAxis {0.f};
    float _vAxis {0.f};
    float _mhAxis {0.f};
    float _mvAxis {0.f};
};

}  // atlas
