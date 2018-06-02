foreign class NodePtr {
    foreign get
    foreign useCount
}


foreign class NodeTypes {
    foreign static Undefined
    foreign static Root
    foreign static Spatial
}

foreign class Node {
    construct new(name, parent) {}
    foreign name
    foreign name=(rhs)
    foreign hash
    foreign enabled
    foreign enabled=(rhs)
    foreign parent // parent: Node
    foreign parentPtr // parentPtr :NodePtr
    foreign childCount
    foreign childAt(index) // return: Node
    foreign childPtrAt(index) // return: NodePtr
    foreign attachScript(script)
}

foreign class SceneManager {
    foreign root
    foreign addNode(type, name, parent)
    foreign removeNode(node)
    foreign reparentNode(node, newParent)

    attachScript(node, script) {
        node.get.attachScript(script)
    }
}


class NodeScript {
    onInit() {}
    onAttach() {}
    onDetach() {}
    onUpdate(dt) {}
    onDestoy() {}
}
