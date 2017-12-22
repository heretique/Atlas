import "scripts/imgui" for Imgui
import "scripts/vector" for Vec2, Vec3


var Test = Vec3.one


class Main {

    static init() {}

    static update(delta) {
    	Test = Test + Vec3.one
    }

    static onGUI() {
//     	Imgui.text("Test vector: " + Test.toString())
//     	Imgui.button("Test")
		Imgui.setNextWindowSize(Vec2.new(800, 480))
		Imgui.begin("Test win")
		
		Imgui.end()
    }
 }
