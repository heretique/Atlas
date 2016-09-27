#ifndef IMGUI_WREN_H
#define IMGUI_WREN_H

static const char* IMGUI_WREN = " \
foreign class ImguiFlag { \
    construct new() {} \
    foreign setTitleBar()   // on by default \
    foreign unsetTitleBar() \
    foreign setResize()     // on by default \
    foreign unsetResize() \
    foreign setMove()       // on by default \
    foreign unsetMove() \
    foreign setShowBorders()    // off by default \
    foreign unsetShowBorders() \
} \
\
class ImGui { \
\
    // windows \
    foreign static begin( name )    // begin a window scope \
    foreign static begin( name, flags ) // begin a window scope with window flags \
    foreign static end()            // end a window scope \
\
    // window control \
    foreign static setNextWindowPos( size ) \
    foreign static setNextWindowSize( size )    // call before begin() \
\
} \
";


#endif // IMGUI_WREN_H
