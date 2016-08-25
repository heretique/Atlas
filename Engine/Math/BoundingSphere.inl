#include "BoundingSphere.h"

namespace math
{

inline BoundingSphere& BoundingSphere::operator*=(const Matrix& matrix)
{
    transform(matrix);
    return *this;
}

inline const BoundingSphere operator*(const Matrix& matrix, const BoundingSphere& sphere)
{
    BoundingSphere s(sphere);
    s.transform(matrix);
    return s;
}

}
