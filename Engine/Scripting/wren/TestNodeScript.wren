
var Test = Vec3.one
var WindowSize = Vec2.new(800, 480)

class TestNodeScript is NodeScript {
    construct new() {}
    onInit() {}
    onAttach() {}
    onUpdate(dt) {}
    onGUI() {
        Imgui.setNextWindowSize(WindowSize)
        Imgui.begin("Wren IMGUI")
        Imgui.text("Test : " + Test.toString())
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
