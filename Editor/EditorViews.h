#ifndef EDITORVIEW_H
#define EDITORVIEW_H

namespace atlasEditor {

////////////// EditorView interface ////////
class EditorView
{
public:
    virtual void onGUI() = 0;
};

////////////// SceneView /////////////
class SceneView : public EditorView
{
public:
    SceneView();

    void onGUI() override;
};


} // atlasEditor



#endif // EDITORVIEW_H
