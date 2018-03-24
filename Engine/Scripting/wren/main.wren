import "scripts/ImGui" for Imgui
import "scripts/Vector" for Vec2, Vec3
import "scripts/Utils" for StringHash
import "scripts/Assets" for AssetHandle, AssetTypes, AssetManager, TextureFlags, ShaderTypes
import "scripts/Engine" for Engine


var Test = Vec3.one
var WindowSize = Vec2.new(800, 480)
var AssetType = AssetTypes.Geometry


class Main {

    static init() {
        var assetManager = Engine.assets()
        var carHandle = assetManager.addAsset(AssetTypes.Geometry, "assets/caruta.obj", 0)
        var vs = assetManager.addAsset(AssetTypes.Shader, "assets/vs_cubes.sc", ShaderTypes.Vertex)
        var fs = assetManager.addAsset(AssetTypes.Shader, "assets/fs_cubes.sc", ShaderTypes.Fragment)
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
