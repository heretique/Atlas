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
                Imgui.begin("Ce faci Alin?")
                Imgui.text("Test ALIN: " + Test.toString())
                Imgui.button("Test")
		Imgui.end()
    }
 }
