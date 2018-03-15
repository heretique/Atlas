import "scripts/ImGui" for Imgui
import "scripts/Vector" for Vec2, Vec3
import "scripts/Utils" for StringHash
import "scripts/Assets" for AssetHandle, AssetTypes, AssetManager
import "scripts/Engine" for Engine


var Test = Vec3.one
var WindowSize = Vec2.new(800, 480)

class Main {

    static init() {
        var assetManager = Engine.assets()
        var handle = assetManager.addAsset(AssetTypes.Code, "test", 0)
         if (handle != AssetHandle.invalid) {
            System.print("Handle: (%(handle.index), %(handle.generation))")
         }
        assetManager.loadAssets()
    }

    static update(delta) {
        Test = Test + Vec3.one
    }

    static onGUI() {
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
 }
