class TestNodeScript {
    construct new() {}

    onInit() {}
    onAttach() {}
    onUpdate(dt) {
        System.print("Update from script - dt: %(dt)")
    }
    onDestoy() {}
}
