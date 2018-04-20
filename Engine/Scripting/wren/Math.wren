
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

foreign class Vec4 {
    construct new( x, y, z, w ) {}

    foreign x
    foreign x=( rhs )
    foreign y
    foreign y=( rhs )
    foreign z
    foreign z=( rhs )
    foreign w
    foreign w=( rhs )
    foreign r
    foreign r=( rhs )
    foreign g
    foreign g=( rhs )
    foreign b
    foreign b=( rhs )
    foreign a
    foreign a=( rhs )

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

    foreign *( rhs )    // returns the result
    foreign +( rhs )     // returns the result
    foreign -( rhs )    // returns the result


    static one { Vec3.new(1, 1, 1, 1) }
    static zero { Vec3.new(0, 0, 0, 0) }
    isOne() { x == 1 && y == 1 && z == 1 && w == 1 }
    isZero() { x == 0 && y == 0 && z == 0 && w == 0 }
    toString() {"(%(x), %(y), %(z), %(w))"}
}

foreign class Quaternion {
    construct new() {}

    foreign x
    foreign x=( rhs )
    foreign y
    foreign y=( rhs )
    foreign z
    foreign z=( rhs )
    foreign w
    foreign w=( rhs )

    foreign static identity
    foreign static zero

    foreign setIdentity()
    foreign setFromValues(x, y, z, w)
    foreign setFromMatrix(m)
    foreign setFromAxisAngle(axis, angle) // vec3, float
    foreign setFromQuaternion(quaternion)

    foreign static lerp(q1, q2, t, qdest)
    foreign static slerp(q1, q2, t, qdest)

    foreign *( rhs )
}

foreign class Matrix {
    construct new() {}

    foreign static identity
    foreign static zero

    foreign setIdentity()
    foreign setZero()
}

foreign class Transform {
    construct new() {}

    foreign matrix
    foreign scale
    foreign scaleX
    foreign scaleY
    foreign scaleZ
    foreign rotation
    foreign translation
    foreign translationX
    foreign translationY
    foreign translationZ
    foreign forward
    foreign up
    foreign right
    foreign rotateWithQuaternionFloats(x, y, z, w)
    foreign rotateWithQuaternion(quaternion)
    foreign rotateWithAxisAngle(axis, angle)
    foreign rotateWithMatrix(matrix)
    foreign rotateX(angle)
    foreign rotateY(angle)
    foreign rotateZ(angle)
    foreign uniformScale(scale)
    foreign scaleWithFloats(x, y, z)
    foreign scaleWithVector(vector)
    foreign scaleX(scale)
    foreign scaleY(scale)
    foreign scaleZ(scale)
}
