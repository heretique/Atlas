import qbs
import qbs.ModUtils

StaticLibrary {
    id: bx
    name: "bx"
    files: [
        "../../bx/include/bx/allocator.h",
        "../../bx/include/bx/bx.h",
        "../../bx/include/bx/cl.h",
        "../../bx/include/bx/commandline.h",
        "../../bx/include/bx/config.h",
        "../../bx/include/bx/cpu.h",
        "../../bx/include/bx/crtimpl.h",
        "../../bx/include/bx/debug.h",
        "../../bx/include/bx/easing.h",
        "../../bx/include/bx/endian.h",
        "../../bx/include/bx/error.h",
        "../../bx/include/bx/float4x4_t.h",
        "../../bx/include/bx/foreach.h",
        "../../bx/include/bx/fpumath.h",
        "../../bx/include/bx/handlealloc.h",
        "../../bx/include/bx/hash.h",
        "../../bx/include/bx/macros.h",
        "../../bx/include/bx/maputil.h",
        "../../bx/include/bx/mpscqueue.h",
        "../../bx/include/bx/mutex.h",
        "../../bx/include/bx/os.h",
        "../../bx/include/bx/platform.h",
        "../../bx/include/bx/radixsort.h",
        "../../bx/include/bx/readerwriter.h",
        "../../bx/include/bx/ringbuffer.h",
        "../../bx/include/bx/rng.h",
        "../../bx/include/bx/sem.h",
        "../../bx/include/bx/simd_t.h",
        "../../bx/include/bx/spscqueue.h",
        "../../bx/include/bx/string.h",
        "../../bx/include/bx/thread.h",
        "../../bx/include/bx/timer.h",
        "../../bx/include/bx/uint32_t.h",
        "../../bx/include/compat/freebsd/alloca.h",
        "../../bx/include/compat/freebsd/dirent.h",
        "../../bx/include/compat/freebsd/malloc.h",
        "../../bx/include/compat/freebsd/signal.h",
        "../../bx/include/compat/ios/malloc.h",
        "../../bx/include/compat/mingw/alloca.h",
        "../../bx/include/compat/mingw/dirent.h",
        "../../bx/include/compat/mingw/sal.h",
        "../../bx/include/compat/mingw/specstrings_strict.h",
        "../../bx/include/compat/mingw/specstrings_undef.h",
        "../../bx/include/compat/msvc/alloca.h",
        "../../bx/include/compat/msvc/dirent.h",
        "../../bx/include/compat/msvc/inttypes.h",
        "../../bx/include/compat/msvc/pre1600/stdint.h",
        "../../bx/include/compat/msvc/stdbool.h",
        "../../bx/include/compat/nacl/memory.h",
        "../../bx/include/compat/osx/malloc.h",
        "../../bx/include/tinystl/allocator.h",
        "../../bx/include/tinystl/buffer.h",
        "../../bx/include/tinystl/hash.h",
        "../../bx/include/tinystl/hash_base.h",
        "../../bx/include/tinystl/new.h",
        "../../bx/include/tinystl/stddef.h",
        "../../bx/include/tinystl/string.h",
        "../../bx/include/tinystl/traits.h",
        "../../bx/include/tinystl/unordered_map.h",
        "../../bx/include/tinystl/unordered_set.h",
        "../../bx/include/tinystl/vector.h",
        "../../bx/src/commandline.cpp",
        "../../bx/src/crtimpl.cpp",
        "../../bx/src/debug.cpp",
        "../../bx/src/os.cpp",
        "../../bx/src/string.cpp"
    ]
    Depends { name: "cpp" }

    cpp.includePaths: [
        "../../bx/include"
    ]

    cpp.cxxLanguageVersion: "c++14"

    Properties {
        condition: qbs.targetOS.contains("windows")
        cpp.windowsApiCharacterSet: ""
    }

    Properties {
        condition: qbs.toolchain.contains("msvc")
        cpp.systemIncludePaths: outer.uniqueConcat([ "../../bx/include/compat/msvc" ])
    }

    Properties {
        condition: qbs.toolchain.contains("mingw")
        cpp.systemIncludePaths: outer.uniqueConcat([ "../../bx/include/compat/mingw" ])
    }

    Properties {
        condition: qbs.targetOS.contains("osx")
        cpp.systemIncludePaths: outer.uniqueConcat([ "../../bx/include/compat/osx" ])
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
