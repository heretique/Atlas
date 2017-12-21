
foreign class Vec2 {
    construct new(x, y) {}

    foreign x
    foreign x=(rhs)
    foreign y
    foreign y=(rhs)
    foreign i
    foreign i=(rhs)
    foreign j
    foreign j=(rhs)

    foreign static angle(v1, v2)
    foreign clamp(min, max)
    foreign distance(v)
    foreign distanceSquared(v)
    foreign dot(rhs)
    foreign static dot(v1, v2)
    foreign length()
    foreign lengthSquared()
    foreign negate()
    foreign normalize() // normalizes the vector in place
    foreign scale(s)
    foreign smooth(target, elapsedTime, responseTime)

    foreign +(rhs)
    foreign -(rhs)
    foreign *(rhs)
    

    static one { Vec2.new(1, 1) }
    static zero { Vec2.new(0, 0) }
    isOne() { x == 1 && y == 1 }
    isZero() { x == 0 && y == 0  }
    toString() {"(%(x), %(y))"}
}

foreign class Vec3 {
    construct new( x, y, z ) {}

    foreign x
    foreign x=( rhs )
    foreign y
    foreign y=( rhs )
    foreign z
    foreign z=( rhs )
    foreign r
    foreign r=( rhs )
    foreign g
    foreign g=( rhs )
    foreign b
    foreign b=( rhs )

    foreign static angle(v1, v2)
    foreign clamp(min, max)
    foreign cross( rhs )    // returns the result
    foreign distance(v)
    foreign distanceSquared(v)
    foreign dot(rhs)
    foreign static dot(v1, v2)
    foreign length()
    foreign lengthSquared()
    foreign negate()
    foreign normalize() // normalizes the vector in place
    foreign scale(s)
    foreign smooth(target, elapsedTime, responseTime)

    foreign *( rhs )    // returns the result
    foreign +( rhs )     // returns the result
    foreign -( rhs )    // returns the result


    static one { Vec3.new(1, 1, 1) }
    static zero { Vec3.new(0, 0, 0) }
    isOne() { x == 1 && y == 1 && z == 1 }
    isZero() { x == 0 && y == 0 && z == 0 }
    toString() {"(%(x), %(y), %(z))"}
}

// foreign class Vec4 {
//     construct new( x, y, z, w ) {}

//     foreign x
//     foreign x=( rhs )
//     foreign y
//     foreign y=( rhs )
//     foreign z
//     foreign z=( rhs )
//     foreign w
//     foreign w=( rhs )
//     foreign r
//     foreign r=( rhs )
//     foreign g
//     foreign g=( rhs )
//     foreign b
//     foreign b=( rhs )
//     foreign a
//     foreign a=( rhs )

//     foreign norm()
//     foreign normSquared()
//     foreign normalize()
//     foreign dot( rhs )
//     foreign *( rhs )
//     foreign +( rhs )
//     foreign -( rhs )
//     foreign scale( s )

//     static identity {
//         return Vec4.new(1.0, 1.0, 1.0, 1.0)
//     }
// }
