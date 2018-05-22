import "scripts/ImGui" for Imgui
import "scripts/Math" for Vec2, Vec3
import "scripts/Utils" for StringHash
import "scripts/Assets" for AssetPtr, AssetTypes, AssetManager, TextureFlags, ShaderTypes
import "scripts/Engine" for Engine
import "scripts/Scene" for Node, NodePtr, NodeTypes, SceneManager


var Test = Vec3.one
var WindowSize = Vec2.new(800, 480)
var AssetType = AssetTypes.Geometry


class Main {

    static init() {
        var assetManager = Engine.assets()
        var sceneManager = Engine.scene()
        var carHandle = assetManager.addAsset(AssetTypes.Geometry, "assets/caruta.obj", 0)
        var vs = assetManager.addAsset(AssetTypes.Shader, "assets/vs_cubes.bin", ShaderTypes.Vertex)
        var fs = assetManager.addAsset(AssetTypes.Shader, "assets/fs_cubes.bin", ShaderTypes.Fragment)
        var material = assetManager.addAsset(AssetTypes.Material, "assets/unlit_textured.material", 0)
        assetManager.loadAssets()
        var node = sceneManager.addNode(NodeTypes.Spatial, "Test Node", sceneManager.root)
        System.print("Node name: %(node.get.name), use count: %(node.useCount)")
        var node2 = sceneManager.addNode(NodeTypes.Spatial, "Test2 Node", node)
        System.print("Node name: %(node.get.name), use count: %(node.useCount)")
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
