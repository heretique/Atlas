foreign class NodePtr {
    foreign get
    foreign use_count
}


foreign class Node {
    construct new(name, parent) {}
    foreign id
    foreign name
    foreign name=(rhs)
    foreign hash
    foreign enabled
    foreign enabled=(rhs)
    foreign parent // parent: NodePtr
    foreign addChild(child) // child: NodePtr
}

foreign class SceneManager {
    foreign root
    foreign addNode(name, parent)
}
