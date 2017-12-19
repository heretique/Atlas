#ifndef WINDOW_H
#define WINDOW_H

#include "SDL/SDLWindow.h"

#include <chrono>
#include <memory>
#include <vector>
#include <wrenpp/Wren++.h>

namespace atlas
{
struct JobData;

class MainWindow : public SDLWindow
{
public:
    MainWindow(const char* title, int x, int y, int w, int h);
    ~MainWindow();

public:
    void init() override;
    void update(float dt) override;
    void onGUI() override;

private:
    wrenpp::Result _vmResult;
    wrenpp::Method _init;
    wrenpp::Method _update;
    wrenpp::Method _ongui;
};

}  // namespace atlas

#endif  // WINDOW_H
