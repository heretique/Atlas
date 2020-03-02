#include "Managers/InputManager.h"
#include "SDL2/SDL_events.h"

namespace atlas
{
InputManager::InputManager() {}

float InputManager::horizontalAxis() const
{
    return _hAxis;
}

float InputManager::verticalAxis() const
{
    return _vAxis;
}

float InputManager::mouseHorizontalAxis() const
{
    return _mhAxis;
}

float InputManager::mouseVerticalAxis() const
{
    return _mvAxis;
}

bool InputManager::mouseDown() const
{
    return _registerMouse;
}

::hq::math::Vec2 InputManager::mousePos() const
{
    ::hq::math::Vec2 pos;
    SDL_GetMouseState((int*)&pos.x,(int*)&pos.y);
    return pos;
}

void InputManager::handleInputEvent(const SDL_Event& e)
{
    _mhAxis = 0.f;
    _mvAxis = 0.f;

    switch (e.type)
    {
        case SDL_MOUSEMOTION:
            if (_registerMouse)
            {
                _mhAxis = e.motion.xrel;
                _mvAxis = e.motion.yrel;
            }
            //            io.MousePos = ImVec2(e.motion.x, e.motion.y);
            break;
        case SDL_MOUSEWHEEL:
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                _registerMouse = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                _registerMouse = false;
            }
            break;
        case SDL_KEYDOWN:
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_w:
                    _vAxis = 1.f;
                    break;
                case SDLK_a:
                    _hAxis = 1.f;
                    break;
                case SDLK_s:
                    _vAxis = -1.f;
                    break;
                case SDLK_d:
                    _hAxis = -1.f;
                    break;
            }
        }
        break;
        case SDL_KEYUP:
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_w:
                    if (_vAxis == 1.f)
                        _vAxis = 0.f;
                    break;
                case SDLK_a:
                    if (_hAxis == 1.f)
                        _hAxis = 0;
                    break;
                case SDLK_s:
                    if (_vAxis == -1.f)
                        _vAxis = 0;
                    break;
                case SDLK_d:
                    if (_hAxis == -1.f)
                        _hAxis = 0;
                    break;
            }
        }
        break;
    }
}

void InputManager::resetInput()
{
    _mhAxis = 0.f;
    _mvAxis = 0.f;
}

}  // atlas
