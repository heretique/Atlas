foreign class NodePtr {
    foreign get
}

foreign class Node {
    construct new(name, parent) {}
    foreign id
    foreign name
    foreign name=(rhs)
    foreign hash
    foreign enabled
    foreign enabled=(rhs)
    foreign parent
}

foreign class SceneManager {
    foreign root
    foreign addNode(name, parent)
}
