#include "Matrix.h"

#include "maths/Maths.h"
#include "maths/Vec3.h"

Matrix
identityMatrix()
{
    Matrix m;
    D3DXMatrixIdentity(&m);

    return m;
}

Matrix
perspectiveMatrix(const QSize &size, float fov)
{
    float vx = float(size.width());
    float vy = float(size.height());

    Matrix m;
    D3DXMatrixPerspectiveFovLH(&m, fov, vx / vy, 0.01f, 32767.0f);

    return m;
}

Matrix
orthoMatrix(float width, float height)
{
    Matrix m;
    D3DXMatrixOrthoLH(&m, width, height, -32767.0f, 32767.0f);

    return m;
}

Matrix
orthoMatrix(const QSize &size, float pixelsPerUnit)
{
    float width;
    float height;

    if(size.width() < size.height())
    {
        height = float(size.height()) / pixelsPerUnit;
        width = height * (float(size.width()) / float(size.height()));
    }
    else
    {
        width = float(size.width()) / pixelsPerUnit;
        height = width * (float(size.height()) / float(size.width()));
    }

    return orthoMatrix(width, height);
}

Matrix
orthoMatrix(const Vec3 &pos, const QSize &size, float fov)
{
    return orthoMatrix(size, orthoUnit(pos, size, fov));
}

float
orthoUnit(const Vec3 &pos, const QSize &size, float fov)
{
    Matrix m = lookAtMatrix(pos, pos + Vec3(0, 0, 1), Vec3(0, 1, 0)) * perspectiveMatrix(size, fov);

    Vec3 a(-0.5f, 0, 0);
    a = transformCoord(a, m);

    Vec3 b(0.5f, 0, 0);
    b = transformCoord(b, m);

    float ap = float((size.width()) / 2.0f) * a.x;
    float bp = float((size.width()) / 2.0f) * b.x;

    return fabs(ap - bp);
}

Matrix
lookAtMatrix(const Vec3 &eye, const Vec3 &look, const Vec3 &up)
{
    D3DXVECTOR3 ev = eye, lv = look, uv = up;

    Matrix m;
    D3DXMatrixLookAtLH(&m, &ev, &lv, &uv);

    return m;
}

Matrix
rotationXMatrix(float angle)
{
    Matrix m;
    D3DXMatrixRotationX(&m, angle);

    return m;
}

Matrix
rotationYMatrix(float angle)
{
    Matrix m;
    D3DXMatrixRotationY(&m, angle);

    return m;
}

Matrix
rotationZMatrix(float angle)
{
    Matrix m;
    D3DXMatrixRotationZ(&m, angle);

    return m;
}

Matrix
rotationMatrix(const Vec3 &v)
{
    Matrix x = rotationXMatrix(v.x);
    Matrix y = rotationYMatrix(v.y);
    Matrix z = rotationZMatrix(v.z);

    return x * y * z;
}

Matrix
rotationMatrixDegrees(const Vec3 &v)
{
    Matrix x = rotationXMatrix(degreeToRadian(v.x));
    Matrix y = rotationYMatrix(degreeToRadian(v.y));
    Matrix z = rotationZMatrix(degreeToRadian(v.z));

    return x * y * z;
}

Matrix
rotationAxisMatrix(const Vec3 &axis, float angle)
{
    D3DXVECTOR3 a = axis;
    Matrix m;
    D3DXMatrixRotationAxis(&m, &a, angle);

    return m;
}

Matrix
translationMatrix(const Vec3 &v)
{
    Matrix m;
    D3DXMatrixTranslation(&m, v.x, v.y, v.z);

    return m;
}

Matrix
scalingMatrix(float scale)
{
    Matrix m;
    D3DXMatrixScaling(&m, scale, scale, scale);

    return m;
}

Matrix
scalingMatrix(float x, float y, float z)
{
    Matrix m;
    D3DXMatrixScaling(&m, x, y, z);

    return m;
}

Matrix
scalingMatrix(const Vec3 &v)
{
    Matrix m;
    D3DXMatrixScaling(&m, v.x, v.y, v.z);

    return m;
}

Matrix
inverseMatrix(const Matrix &m)
{
    Matrix r;
    D3DXMatrixInverse(&r, NULL, &m);

    return r;
}

Matrix
quaternionMatrix(const Quaternion &q)
{
    Matrix r;
    D3DXMatrixRotationQuaternion(&r, &q);

    return r;
}

Matrix
filterMatrix(const Matrix &m, const Vec3 &scale, const Vec3 &translate)
{
    Vec3 s, t;
    Quaternion q;
    D3DXMatrixDecompose(&s, &q, &t, &m);

    s.x = scale.x ? s.x : 1;
    s.y = scale.y ? s.y : 1;
    s.z = scale.z ? s.z : 1;

    t = mulVectors(t, translate);

    return scalingMatrix(absVector(s)) * quaternionMatrix(q) * translationMatrix(t);
}
