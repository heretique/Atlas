foreign class StringHash {
    construct new (string) {}
    foreign hash()
    toString() {
        var hash = hash()
        return "%(hash)"
    }
}
