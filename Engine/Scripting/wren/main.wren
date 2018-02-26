import "scripts/imgui" for Imgui
import "scripts/vector" for Vec2, Vec3


var Test = Vec3.one
var WindowSize = Vec2.new(800, 480)


class Main {

    static init() {}

    static update(delta) {
        Test = Test + Vec3.one
    }

    static onGUI() {
                Imgui.setNextWindowSize(WindowSize)
                Imgui.begin("Wren IMGUI")
                Imgui.text("Test : " + Test.toString())
                if (Imgui.button("Test")) Imgui.text("Pressed")
                WindowSize = Imgui.getWindowSize()
		Imgui.end()
    }
 }
