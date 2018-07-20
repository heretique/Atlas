var WindowSize = Vec2.new(800, 480)

class TestNodeScript is NodeScript {
    construct new() {}
    onInit() {
        _test = Vec3.one
        System.print("Script attached to node: %(node.name)")
        //node.getComponent(StringHash.new("Transform"))
    }
    onAttach() {}
    onUpdate(dt) {
        _test = _test + Vec3.one
    }
    onGUI() {
        Imgui.setNextWindowSize(WindowSize)
        Imgui.begin("Wren IMGUI")
        Imgui.text("Test : " + _test.toString())
        if (Imgui.button("Test")) Imgui.text("Pressed")
        WindowSize = Imgui.getWindowSize()
        for (i in 1..10) {
            var text = "TestString%(i)"
            Imgui.text("String: %(text), hash: %(StringHash.new(text).hash())")
        }
        Imgui.end()
    }
    onDestoy() {}
}
