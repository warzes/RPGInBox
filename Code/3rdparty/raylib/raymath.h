#pragma once

#include "raylib.h"             // Required for structs: Vector3, Matrix

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#ifndef PI
    #define PI 3.14159265358979323846f
#endif

#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif

#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif

// Get float vector for Matrix
#ifndef MatrixToFloat
    #define MatrixToFloat(mat) (MatrixToFloatV(mat).v)
#endif

// Get float vector for Vector3
#ifndef Vector3ToFloat
    #define Vector3ToFloat(vec) (Vector3ToFloatV(vec).v)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// NOTE: Helper types to be used instead of array return types for *ToFloat functions
typedef struct float3 { float v[3]; } float3;
typedef struct float16 { float v[16]; } float16;

#include <math.h>       // Required for: sinf(), cosf(), tan(), atan2f(), sqrtf(), fminf(), fmaxf(), fabs()

//----------------------------------------------------------------------------------
// Module Functions Definition - Utils math
//----------------------------------------------------------------------------------

// Clamp float value
inline float Clamp(float value, float min, float max)
{
    const float res = value < min ? min : value;
    return res > max ? max : res;
}

// Calculate linear interpolation between two floats
inline float Lerp(float start, float end, float amount)
{
    return start + amount*(end - start);
}

// Normalize input value within input range
inline float Normalize(float value, float start, float end)
{
    return (value - start)/(end - start);
}

// Remap input value within input range to output range
inline float Remap(float value, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
    return (value - inputStart)/(inputEnd - inputStart)*(outputEnd - outputStart) + outputStart;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Vector2 math
//----------------------------------------------------------------------------------

// Vector with components value 0.0f
inline Vector2 Vector2Zero(void)
{
    Vector2 result = { 0.0f, 0.0f };
    return result;
}

// Vector with components value 1.0f
inline Vector2 Vector2One(void)
{
    Vector2 result = { 1.0f, 1.0f };
    return result;
}

// Add two vectors (v1 + v2)
inline Vector2 Vector2Add(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x + v2.x, v1.y + v2.y };
    return result;
}

// Add vector and float value
inline Vector2 Vector2AddValue(Vector2 v, float add)
{
    Vector2 result = { v.x + add, v.y + add };
    return result;
}

// Subtract two vectors (v1 - v2)
inline Vector2 Vector2Subtract(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x - v2.x, v1.y - v2.y };
    return result;
}

// Subtract vector by float value
inline Vector2 Vector2SubtractValue(Vector2 v, float sub)
{
    Vector2 result = { v.x - sub, v.y - sub };
    return result;
}

// Calculate vector length
inline float Vector2Length(Vector2 v)
{
    float result = sqrtf((v.x*v.x) + (v.y*v.y));
    return result;
}

// Calculate vector square length
inline float Vector2LengthSqr(Vector2 v)
{
    float result = (v.x*v.x) + (v.y*v.y);
    return result;
}

// Calculate two vectors dot product
inline float Vector2DotProduct(Vector2 v1, Vector2 v2)
{
    float result = (v1.x*v2.x + v1.y*v2.y);
    return result;
}

// Calculate distance between two vectors
inline float Vector2Distance(Vector2 v1, Vector2 v2)
{
    float result = sqrtf((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
    return result;
}

// Calculate angle from two vectors in X-axis
inline float Vector2Angle(Vector2 v1, Vector2 v2)
{
    float result = atan2f(v2.y - v1.y, v2.x - v1.x)*(180.0f/PI);
    if (result < 0) result += 360.0f;
    return result;
}

// Scale vector (multiply by value)
inline Vector2 Vector2Scale(Vector2 v, float scale)
{
    Vector2 result = { v.x*scale, v.y*scale };
    return result;
}

// Multiply vector by vector
inline Vector2 Vector2Multiply(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x*v2.x, v1.y*v2.y };
    return result;
}

// Negate vector
inline Vector2 Vector2Negate(Vector2 v)
{
    Vector2 result = { -v.x, -v.y };
    return result;
}

// Divide vector by vector
inline Vector2 Vector2Divide(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x/v2.x, v1.y/v2.y };
    return result;
}

// Normalize provided vector
inline Vector2 Vector2Normalize(Vector2 v)
{
    Vector2 result = Vector2Scale(v, 1/Vector2Length(v));
    return result;
}

// Calculate linear interpolation between two vectors
inline Vector2 Vector2Lerp(Vector2 v1, Vector2 v2, float amount)
{
    Vector2 result = { 0 };

    result.x = v1.x + amount*(v2.x - v1.x);
    result.y = v1.y + amount*(v2.y - v1.y);

    return result;
}

// Calculate reflected vector to normal
inline Vector2 Vector2Reflect(Vector2 v, Vector2 normal)
{
    Vector2 result = { 0 };

    float dotProduct = Vector2DotProduct(v, normal);

    result.x = v.x - (2.0f*normal.x)*dotProduct;
    result.y = v.y - (2.0f*normal.y)*dotProduct;

    return result;
}

// Rotate Vector by float in Degrees.
inline Vector2 Vector2Rotate(Vector2 v, float degs)
{
    float rads = degs*DEG2RAD;
    Vector2 result = {v.x*cosf(rads) - v.y*sinf(rads) , v.x*sinf(rads) + v.y*cosf(rads) };
    return result;
}

// Move Vector towards target
inline Vector2 Vector2MoveTowards(Vector2 v, Vector2 target, float maxDistance)
{
    Vector2 result = { 0 };
    float dx = target.x - v.x;
    float dy = target.y - v.y;
    float value = (dx*dx) + (dy*dy);

    if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance*maxDistance))) return target;

    float dist = sqrtf(value);

    result.x = v.x + dx/dist*maxDistance;
    result.y = v.y + dy/dist*maxDistance;

    return result;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Vector3 math
//----------------------------------------------------------------------------------

// Vector with components value 0.0f
inline Vector3 Vector3Zero(void)
{
    Vector3 result = { 0.0f, 0.0f, 0.0f };
    return result;
}

// Vector with components value 1.0f
inline Vector3 Vector3One(void)
{
    Vector3 result = { 1.0f, 1.0f, 1.0f };
    return result;
}

// Add two vectors
inline Vector3 Vector3Add(Vector3 v1, Vector3 v2)
{
    Vector3 result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    return result;
}

// Add vector and float value
inline Vector3 Vector3AddValue(Vector3 v, float add)
{
    Vector3 result = { v.x + add, v.y + add, v.z + add };
    return result;
}

// Subtract two vectors
inline Vector3 Vector3Subtract(Vector3 v1, Vector3 v2)
{
    Vector3 result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
    return result;
}

// Subtract vector by float value
inline Vector3 Vector3SubtractValue(Vector3 v, float sub)
{
    Vector3 result = { v.x - sub, v.y - sub, v.z - sub };
    return result;
}

// Multiply vector by scalar
inline Vector3 Vector3Scale(Vector3 v, float scalar)
{
    Vector3 result = { v.x*scalar, v.y*scalar, v.z*scalar };
    return result;
}

// Multiply vector by vector
inline Vector3 Vector3Multiply(Vector3 v1, Vector3 v2)
{
    Vector3 result = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z };
    return result;
}

// Calculate two vectors cross product
inline Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2)
{
    Vector3 result = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
    return result;
}

// Calculate one vector perpendicular vector
inline Vector3 Vector3Perpendicular(Vector3 v)
{
    Vector3 result = { 0 };

    float min = (float) fabs(v.x);
    Vector3 cardinalAxis = {1.0f, 0.0f, 0.0f};

    if (fabs(v.y) < min)
    {
        min = (float) fabs(v.y);
        Vector3 tmp = {0.0f, 1.0f, 0.0f};
        cardinalAxis = tmp;
    }

    if (fabs(v.z) < min)
    {
        Vector3 tmp = {0.0f, 0.0f, 1.0f};
        cardinalAxis = tmp;
    }

    result = Vector3CrossProduct(v, cardinalAxis);

    return result;
}

// Calculate vector length
inline float Vector3Length(const Vector3 v)
{
    float result = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    return result;
}

// Calculate vector square length
inline float Vector3LengthSqr(const Vector3 v)
{
    float result = v.x*v.x + v.y*v.y + v.z*v.z;
    return result;
}

// Calculate two vectors dot product
inline float Vector3DotProduct(Vector3 v1, Vector3 v2)
{
    float result = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
    return result;
}

// Calculate distance between two vectors
inline float Vector3Distance(Vector3 v1, Vector3 v2)
{
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;
    float result = sqrtf(dx*dx + dy*dy + dz*dz);
    return result;
}

// Negate provided vector (invert direction)
inline Vector3 Vector3Negate(Vector3 v)
{
    Vector3 result = { -v.x, -v.y, -v.z };
    return result;
}

// Divide vector by vector
inline Vector3 Vector3Divide(Vector3 v1, Vector3 v2)
{
    Vector3 result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z };
    return result;
}

// Normalize provided vector
inline Vector3 Vector3Normalize(Vector3 v)
{
    Vector3 result = v;

    float length, ilength;
    length = Vector3Length(v);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f/length;

    result.x *= ilength;
    result.y *= ilength;
    result.z *= ilength;

    return result;
}

// Orthonormalize provided vectors
// Makes vectors normalized and orthogonal to each other
// Gram-Schmidt function implementation
inline void Vector3OrthoNormalize(Vector3 *v1, Vector3 *v2)
{
    *v1 = Vector3Normalize(*v1);
    Vector3 vn = Vector3CrossProduct(*v1, *v2);
    vn = Vector3Normalize(vn);
    *v2 = Vector3CrossProduct(vn, *v1);
}

// Transforms a Vector3 by a given Matrix
inline Vector3 Vector3Transform(Vector3 v, Matrix mat)
{
    Vector3 result = { 0 };
    float x = v.x;
    float y = v.y;
    float z = v.z;

    result.x = mat.m0*x + mat.m4*y + mat.m8*z + mat.m12;
    result.y = mat.m1*x + mat.m5*y + mat.m9*z + mat.m13;
    result.z = mat.m2*x + mat.m6*y + mat.m10*z + mat.m14;

    return result;
}

// Transform a vector by quaternion rotation
inline Vector3 Vector3RotateByQuaternion(Vector3 v, Quaternion q)
{
    Vector3 result = { 0 };

    result.x = v.x*(q.x*q.x + q.w*q.w - q.y*q.y - q.z*q.z) + v.y*(2*q.x*q.y - 2*q.w*q.z) + v.z*(2*q.x*q.z + 2*q.w*q.y);
    result.y = v.x*(2*q.w*q.z + 2*q.x*q.y) + v.y*(q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z) + v.z*(-2*q.w*q.x + 2*q.y*q.z);
    result.z = v.x*(-2*q.w*q.y + 2*q.x*q.z) + v.y*(2*q.w*q.x + 2*q.y*q.z)+ v.z*(q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z);

    return result;
}

// Calculate linear interpolation between two vectors
inline Vector3 Vector3Lerp(Vector3 v1, Vector3 v2, float amount)
{
    Vector3 result = { 0 };

    result.x = v1.x + amount*(v2.x - v1.x);
    result.y = v1.y + amount*(v2.y - v1.y);
    result.z = v1.z + amount*(v2.z - v1.z);

    return result;
}

// Calculate reflected vector to normal
inline Vector3 Vector3Reflect(Vector3 v, Vector3 normal)
{
    // I is the original vector
    // N is the normal of the incident plane
    // R = I - (2*N*( DotProduct[ I,N] ))

    Vector3 result = { 0 };

    float dotProduct = Vector3DotProduct(v, normal);

    result.x = v.x - (2.0f*normal.x)*dotProduct;
    result.y = v.y - (2.0f*normal.y)*dotProduct;
    result.z = v.z - (2.0f*normal.z)*dotProduct;

    return result;
}

// Get min value for each pair of components
inline Vector3 Vector3Min(Vector3 v1, Vector3 v2)
{
    Vector3 result = { 0 };

    result.x = fminf(v1.x, v2.x);
    result.y = fminf(v1.y, v2.y);
    result.z = fminf(v1.z, v2.z);

    return result;
}

// Get max value for each pair of components
inline Vector3 Vector3Max(Vector3 v1, Vector3 v2)
{
    Vector3 result = { 0 };

    result.x = fmaxf(v1.x, v2.x);
    result.y = fmaxf(v1.y, v2.y);
    result.z = fmaxf(v1.z, v2.z);

    return result;
}

// Compute barycenter coordinates (u, v, w) for point p with respect to triangle (a, b, c)
// NOTE: Assumes P is on the plane of the triangle
inline Vector3 Vector3Barycenter(Vector3 p, Vector3 a, Vector3 b, Vector3 c)
{
    //Vector v0 = b - a, v1 = c - a, v2 = p - a;

    Vector3 v0 = Vector3Subtract(b, a);
    Vector3 v1 = Vector3Subtract(c, a);
    Vector3 v2 = Vector3Subtract(p, a);
    float d00 = Vector3DotProduct(v0, v0);
    float d01 = Vector3DotProduct(v0, v1);
    float d11 = Vector3DotProduct(v1, v1);
    float d20 = Vector3DotProduct(v2, v0);
    float d21 = Vector3DotProduct(v2, v1);

    float denom = d00*d11 - d01*d01;

    Vector3 result = { 0 };

    result.y = (d11*d20 - d01*d21)/denom;
    result.z = (d00*d21 - d01*d20)/denom;
    result.x = 1.0f - (result.z + result.y);

    return result;
}

// Get Vector3 as float array
inline float3 Vector3ToFloatV(Vector3 v)
{
    float3 buffer = { 0 };

    buffer.v[0] = v.x;
    buffer.v[1] = v.y;
    buffer.v[2] = v.z;

    return buffer;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Matrix math
//----------------------------------------------------------------------------------

// Compute matrix determinant
inline float MatrixDeterminant(Matrix mat)
{
    // Cache the matrix values (speed optimization)
    float a00 = mat.m0, a01 = mat.m1, a02 = mat.m2, a03 = mat.m3;
    float a10 = mat.m4, a11 = mat.m5, a12 = mat.m6, a13 = mat.m7;
    float a20 = mat.m8, a21 = mat.m9, a22 = mat.m10, a23 = mat.m11;
    float a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

    float result = a30*a21*a12*a03 - a20*a31*a12*a03 - a30*a11*a22*a03 + a10*a31*a22*a03 +
                   a20*a11*a32*a03 - a10*a21*a32*a03 - a30*a21*a02*a13 + a20*a31*a02*a13 +
                   a30*a01*a22*a13 - a00*a31*a22*a13 - a20*a01*a32*a13 + a00*a21*a32*a13 +
                   a30*a11*a02*a23 - a10*a31*a02*a23 - a30*a01*a12*a23 + a00*a31*a12*a23 +
                   a10*a01*a32*a23 - a00*a11*a32*a23 - a20*a11*a02*a33 + a10*a21*a02*a33 +
                   a20*a01*a12*a33 - a00*a21*a12*a33 - a10*a01*a22*a33 + a00*a11*a22*a33;

    return result;
}

// Get the trace of the matrix (sum of the values along the diagonal)
inline float MatrixTrace(Matrix mat)
{
    float result = (mat.m0 + mat.m5 + mat.m10 + mat.m15);
    return result;
}

// Transposes provided matrix
inline Matrix MatrixTranspose(Matrix mat)
{
    Matrix result = { 0 };

    result.m0 = mat.m0;
    result.m1 = mat.m4;
    result.m2 = mat.m8;
    result.m3 = mat.m12;
    result.m4 = mat.m1;
    result.m5 = mat.m5;
    result.m6 = mat.m9;
    result.m7 = mat.m13;
    result.m8 = mat.m2;
    result.m9 = mat.m6;
    result.m10 = mat.m10;
    result.m11 = mat.m14;
    result.m12 = mat.m3;
    result.m13 = mat.m7;
    result.m14 = mat.m11;
    result.m15 = mat.m15;

    return result;
}

// Invert provided matrix
inline Matrix MatrixInvert(Matrix mat)
{
    Matrix result = { 0 };

    // Cache the matrix values (speed optimization)
    float a00 = mat.m0, a01 = mat.m1, a02 = mat.m2, a03 = mat.m3;
    float a10 = mat.m4, a11 = mat.m5, a12 = mat.m6, a13 = mat.m7;
    float a20 = mat.m8, a21 = mat.m9, a22 = mat.m10, a23 = mat.m11;
    float a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

    float b00 = a00*a11 - a01*a10;
    float b01 = a00*a12 - a02*a10;
    float b02 = a00*a13 - a03*a10;
    float b03 = a01*a12 - a02*a11;
    float b04 = a01*a13 - a03*a11;
    float b05 = a02*a13 - a03*a12;
    float b06 = a20*a31 - a21*a30;
    float b07 = a20*a32 - a22*a30;
    float b08 = a20*a33 - a23*a30;
    float b09 = a21*a32 - a22*a31;
    float b10 = a21*a33 - a23*a31;
    float b11 = a22*a33 - a23*a32;

    // Calculate the invert determinant (inlined to avoid double-caching)
    float invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

    result.m0 = (a11*b11 - a12*b10 + a13*b09)*invDet;
    result.m1 = (-a01*b11 + a02*b10 - a03*b09)*invDet;
    result.m2 = (a31*b05 - a32*b04 + a33*b03)*invDet;
    result.m3 = (-a21*b05 + a22*b04 - a23*b03)*invDet;
    result.m4 = (-a10*b11 + a12*b08 - a13*b07)*invDet;
    result.m5 = (a00*b11 - a02*b08 + a03*b07)*invDet;
    result.m6 = (-a30*b05 + a32*b02 - a33*b01)*invDet;
    result.m7 = (a20*b05 - a22*b02 + a23*b01)*invDet;
    result.m8 = (a10*b10 - a11*b08 + a13*b06)*invDet;
    result.m9 = (-a00*b10 + a01*b08 - a03*b06)*invDet;
    result.m10 = (a30*b04 - a31*b02 + a33*b00)*invDet;
    result.m11 = (-a20*b04 + a21*b02 - a23*b00)*invDet;
    result.m12 = (-a10*b09 + a11*b07 - a12*b06)*invDet;
    result.m13 = (a00*b09 - a01*b07 + a02*b06)*invDet;
    result.m14 = (-a30*b03 + a31*b01 - a32*b00)*invDet;
    result.m15 = (a20*b03 - a21*b01 + a22*b00)*invDet;

    return result;
}

// Normalize provided matrix
inline Matrix MatrixNormalize(Matrix mat)
{
    Matrix result = { 0 };

    float det = MatrixDeterminant(mat);

    result.m0 = mat.m0/det;
    result.m1 = mat.m1/det;
    result.m2 = mat.m2/det;
    result.m3 = mat.m3/det;
    result.m4 = mat.m4/det;
    result.m5 = mat.m5/det;
    result.m6 = mat.m6/det;
    result.m7 = mat.m7/det;
    result.m8 = mat.m8/det;
    result.m9 = mat.m9/det;
    result.m10 = mat.m10/det;
    result.m11 = mat.m11/det;
    result.m12 = mat.m12/det;
    result.m13 = mat.m13/det;
    result.m14 = mat.m14/det;
    result.m15 = mat.m15/det;

    return result;
}

// Get identity matrix
inline Matrix MatrixIdentity(void)
{
    Matrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

// Add two matrices
inline Matrix MatrixAdd(Matrix left, Matrix right)
{
    Matrix result = MatrixIdentity();

    result.m0 = left.m0 + right.m0;
    result.m1 = left.m1 + right.m1;
    result.m2 = left.m2 + right.m2;
    result.m3 = left.m3 + right.m3;
    result.m4 = left.m4 + right.m4;
    result.m5 = left.m5 + right.m5;
    result.m6 = left.m6 + right.m6;
    result.m7 = left.m7 + right.m7;
    result.m8 = left.m8 + right.m8;
    result.m9 = left.m9 + right.m9;
    result.m10 = left.m10 + right.m10;
    result.m11 = left.m11 + right.m11;
    result.m12 = left.m12 + right.m12;
    result.m13 = left.m13 + right.m13;
    result.m14 = left.m14 + right.m14;
    result.m15 = left.m15 + right.m15;

    return result;
}

// Subtract two matrices (left - right)
inline Matrix MatrixSubtract(Matrix left, Matrix right)
{
    Matrix result = MatrixIdentity();

    result.m0 = left.m0 - right.m0;
    result.m1 = left.m1 - right.m1;
    result.m2 = left.m2 - right.m2;
    result.m3 = left.m3 - right.m3;
    result.m4 = left.m4 - right.m4;
    result.m5 = left.m5 - right.m5;
    result.m6 = left.m6 - right.m6;
    result.m7 = left.m7 - right.m7;
    result.m8 = left.m8 - right.m8;
    result.m9 = left.m9 - right.m9;
    result.m10 = left.m10 - right.m10;
    result.m11 = left.m11 - right.m11;
    result.m12 = left.m12 - right.m12;
    result.m13 = left.m13 - right.m13;
    result.m14 = left.m14 - right.m14;
    result.m15 = left.m15 - right.m15;

    return result;
}

// Get two matrix multiplication
// NOTE: When multiplying matrices... the order matters!
inline Matrix MatrixMultiply(Matrix left, Matrix right)
{
    Matrix result = { 0 };

    result.m0 = left.m0*right.m0 + left.m1*right.m4 + left.m2*right.m8 + left.m3*right.m12;
    result.m1 = left.m0*right.m1 + left.m1*right.m5 + left.m2*right.m9 + left.m3*right.m13;
    result.m2 = left.m0*right.m2 + left.m1*right.m6 + left.m2*right.m10 + left.m3*right.m14;
    result.m3 = left.m0*right.m3 + left.m1*right.m7 + left.m2*right.m11 + left.m3*right.m15;
    result.m4 = left.m4*right.m0 + left.m5*right.m4 + left.m6*right.m8 + left.m7*right.m12;
    result.m5 = left.m4*right.m1 + left.m5*right.m5 + left.m6*right.m9 + left.m7*right.m13;
    result.m6 = left.m4*right.m2 + left.m5*right.m6 + left.m6*right.m10 + left.m7*right.m14;
    result.m7 = left.m4*right.m3 + left.m5*right.m7 + left.m6*right.m11 + left.m7*right.m15;
    result.m8 = left.m8*right.m0 + left.m9*right.m4 + left.m10*right.m8 + left.m11*right.m12;
    result.m9 = left.m8*right.m1 + left.m9*right.m5 + left.m10*right.m9 + left.m11*right.m13;
    result.m10 = left.m8*right.m2 + left.m9*right.m6 + left.m10*right.m10 + left.m11*right.m14;
    result.m11 = left.m8*right.m3 + left.m9*right.m7 + left.m10*right.m11 + left.m11*right.m15;
    result.m12 = left.m12*right.m0 + left.m13*right.m4 + left.m14*right.m8 + left.m15*right.m12;
    result.m13 = left.m12*right.m1 + left.m13*right.m5 + left.m14*right.m9 + left.m15*right.m13;
    result.m14 = left.m12*right.m2 + left.m13*right.m6 + left.m14*right.m10 + left.m15*right.m14;
    result.m15 = left.m12*right.m3 + left.m13*right.m7 + left.m14*right.m11 + left.m15*right.m15;

    return result;
}

// Get translation matrix
inline Matrix MatrixTranslate(float x, float y, float z)
{
    Matrix result = { 1.0f, 0.0f, 0.0f, x,
                      0.0f, 1.0f, 0.0f, y,
                      0.0f, 0.0f, 1.0f, z,
                      0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

// Create rotation matrix from axis and angle
// NOTE: Angle should be provided in radians
inline Matrix MatrixRotate(Vector3 axis, float angle)
{
    Matrix result = { 0 };

    float x = axis.x, y = axis.y, z = axis.z;

    float lengthSquared = x*x + y*y + z*z;

    if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f))
    {
        float inverseLength = 1.0f/sqrtf(lengthSquared);
        x *= inverseLength;
        y *= inverseLength;
        z *= inverseLength;
    }

    float sinres = sinf(angle);
    float cosres = cosf(angle);
    float t = 1.0f - cosres;

    result.m0 = x*x*t + cosres;
    result.m1 = y*x*t + z*sinres;
    result.m2 = z*x*t - y*sinres;
    result.m3 = 0.0f;

    result.m4 = x*y*t - z*sinres;
    result.m5 = y*y*t + cosres;
    result.m6 = z*y*t + x*sinres;
    result.m7 = 0.0f;

    result.m8 = x*z*t + y*sinres;
    result.m9 = y*z*t - x*sinres;
    result.m10 = z*z*t + cosres;
    result.m11 = 0.0f;

    result.m12 = 0.0f;
    result.m13 = 0.0f;
    result.m14 = 0.0f;
    result.m15 = 1.0f;

    return result;
}

// Get x-rotation matrix (angle in radians)
inline Matrix MatrixRotateX(float angle)
{
    Matrix result = MatrixIdentity();

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.m5 = cosres;
    result.m6 = -sinres;
    result.m9 = sinres;
    result.m10 = cosres;

    return result;
}

// Get y-rotation matrix (angle in radians)
inline Matrix MatrixRotateY(float angle)
{
    Matrix result = MatrixIdentity();

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.m0 = cosres;
    result.m2 = sinres;
    result.m8 = -sinres;
    result.m10 = cosres;

    return result;
}

// Get z-rotation matrix (angle in radians)
inline Matrix MatrixRotateZ(float angle)
{
    Matrix result = MatrixIdentity();

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.m0 = cosres;
    result.m1 = -sinres;
    result.m4 = sinres;
    result.m5 = cosres;

    return result;
}


// Get xyz-rotation matrix (angles in radians)
inline Matrix MatrixRotateXYZ(Vector3 ang)
{
    Matrix result = MatrixIdentity();

    float cosz = cosf(-ang.z);
    float sinz = sinf(-ang.z);
    float cosy = cosf(-ang.y);
    float siny = sinf(-ang.y);
    float cosx = cosf(-ang.x);
    float sinx = sinf(-ang.x);

    result.m0 = cosz*cosy;
    result.m4 = (cosz*siny*sinx) - (sinz*cosx);
    result.m8 = (cosz*siny*cosx) + (sinz*sinx);

    result.m1 = sinz*cosy;
    result.m5 = (sinz*siny*sinx) + (cosz*cosx);
    result.m9 = (sinz*siny*cosx) - (cosz*sinx);

    result.m2 = -siny;
    result.m6 = cosy*sinx;
    result.m10= cosy*cosx;

    return result;
}

// Get zyx-rotation matrix (angles in radians)
inline Matrix MatrixRotateZYX(Vector3 ang)
{
    Matrix result = { 0 };

    float cz = cosf(ang.z);
    float sz = sinf(ang.z);
    float cy = cosf(ang.y);
    float sy = sinf(ang.y);
    float cx = cosf(ang.x);
    float sx = sinf(ang.x);

    result.m0 = cz*cy;
    result.m1 = cz*sy*sx - cx*sz;
    result.m2 = sz*sx + cz*cx*sy;
    result.m3 = 0;

    result.m4 = cy*sz;
    result.m5 = cz*cx + sz*sy*sx;
    result.m6 = cx*sz*sy - cz*sx;
    result.m7 = 0;

    result.m8 = -sy;
    result.m9 = cy*sx;
    result.m10 = cy*cx;
    result.m11 = 0;

    result.m12 = 0;
    result.m13 = 0;
    result.m14 = 0;
    result.m15 = 1;

    return result;
}

// Get scaling matrix
inline Matrix MatrixScale(float x, float y, float z)
{
    Matrix result = { x, 0.0f, 0.0f, 0.0f,
                      0.0f, y, 0.0f, 0.0f,
                      0.0f, 0.0f, z, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

// Get perspective projection matrix
inline Matrix MatrixFrustum(double left, double right, double bottom, double top, double near, double far)
{
    Matrix result = { 0 };

    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(far - near);

    result.m0 = ((float) near*2.0f)/rl;
    result.m1 = 0.0f;
    result.m2 = 0.0f;
    result.m3 = 0.0f;

    result.m4 = 0.0f;
    result.m5 = ((float) near*2.0f)/tb;
    result.m6 = 0.0f;
    result.m7 = 0.0f;

    result.m8 = ((float)right + (float)left)/rl;
    result.m9 = ((float)top + (float)bottom)/tb;
    result.m10 = -((float)far + (float)near)/fn;
    result.m11 = -1.0f;

    result.m12 = 0.0f;
    result.m13 = 0.0f;
    result.m14 = -((float)far*(float)near*2.0f)/fn;
    result.m15 = 0.0f;

    return result;
}

// Get perspective projection matrix
// NOTE: Angle should be provided in radians
inline Matrix MatrixPerspective(double fovy, double aspect, double near, double far)
{
    double top = near*tan(fovy*0.5);
    double right = top*aspect;
    Matrix result = MatrixFrustum(-right, right, -top, top, near, far);

    return result;
}

// Get orthographic projection matrix
inline Matrix MatrixOrtho(double left, double right, double bottom, double top, double near, double far)
{
    Matrix result = { 0 };

    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(far - near);

    result.m0 = 2.0f/rl;
    result.m1 = 0.0f;
    result.m2 = 0.0f;
    result.m3 = 0.0f;
    result.m4 = 0.0f;
    result.m5 = 2.0f/tb;
    result.m6 = 0.0f;
    result.m7 = 0.0f;
    result.m8 = 0.0f;
    result.m9 = 0.0f;
    result.m10 = -2.0f/fn;
    result.m11 = 0.0f;
    result.m12 = -((float)left + (float)right)/rl;
    result.m13 = -((float)top + (float)bottom)/tb;
    result.m14 = -((float)far + (float)near)/fn;
    result.m15 = 1.0f;

    return result;
}

// Get camera look-at matrix (view matrix)
inline Matrix MatrixLookAt(Vector3 eye, Vector3 target, Vector3 up)
{
    Matrix result = { 0 };

    Vector3 z = Vector3Subtract(eye, target);
    z = Vector3Normalize(z);
    Vector3 x = Vector3CrossProduct(up, z);
    x = Vector3Normalize(x);
    Vector3 y = Vector3CrossProduct(z, x);

    result.m0 = x.x;
    result.m1 = y.x;
    result.m2 = z.x;
    result.m3 = 0.0f;
    result.m4 = x.y;
    result.m5 = y.y;
    result.m6 = z.y;
    result.m7 = 0.0f;
    result.m8 = x.z;
    result.m9 = y.z;
    result.m10 = z.z;
    result.m11 = 0.0f;
    result.m12 = -Vector3DotProduct(x, eye);
    result.m13 = -Vector3DotProduct(y, eye);
    result.m14 = -Vector3DotProduct(z, eye);
    result.m15 = 1.0f;

    return result;
}

// Get float array of matrix data
inline float16 MatrixToFloatV(Matrix mat)
{
    float16 buffer = { 0 };

    buffer.v[0] = mat.m0;
    buffer.v[1] = mat.m1;
    buffer.v[2] = mat.m2;
    buffer.v[3] = mat.m3;
    buffer.v[4] = mat.m4;
    buffer.v[5] = mat.m5;
    buffer.v[6] = mat.m6;
    buffer.v[7] = mat.m7;
    buffer.v[8] = mat.m8;
    buffer.v[9] = mat.m9;
    buffer.v[10] = mat.m10;
    buffer.v[11] = mat.m11;
    buffer.v[12] = mat.m12;
    buffer.v[13] = mat.m13;
    buffer.v[14] = mat.m14;
    buffer.v[15] = mat.m15;

    return buffer;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Quaternion math
//----------------------------------------------------------------------------------

// Add two quaternions
inline Quaternion QuaternionAdd(Quaternion q1, Quaternion q2)
{
    Quaternion result = {q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w};
    return result;
}

// Add quaternion and float value
inline Quaternion QuaternionAddValue(Quaternion q, float add)
{
    Quaternion result = {q.x + add, q.y + add, q.z + add, q.w + add};
    return result;
}

// Subtract two quaternions
inline Quaternion QuaternionSubtract(Quaternion q1, Quaternion q2)
{
    Quaternion result = {q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w};
    return result;
}

// Subtract quaternion and float value
inline Quaternion QuaternionSubtractValue(Quaternion q, float sub)
{
    Quaternion result = {q.x - sub, q.y - sub, q.z - sub, q.w - sub};
    return result;
}

// Get identity quaternion
inline Quaternion QuaternionIdentity(void)
{
    Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };
    return result;
}

// Computes the length of a quaternion
inline float QuaternionLength(Quaternion q)
{
    float result = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    return result;
}

// Normalize provided quaternion
inline Quaternion QuaternionNormalize(Quaternion q)
{
    Quaternion result = { 0 };

    float length, ilength;
    length = QuaternionLength(q);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f/length;

    result.x = q.x*ilength;
    result.y = q.y*ilength;
    result.z = q.z*ilength;
    result.w = q.w*ilength;

    return result;
}

// Invert provided quaternion
inline Quaternion QuaternionInvert(Quaternion q)
{
    Quaternion result = q;
    float length = QuaternionLength(q);
    float lengthSq = length*length;

    if (lengthSq != 0.0)
    {
        float i = 1.0f/lengthSq;

        result.x *= -i;
        result.y *= -i;
        result.z *= -i;
        result.w *= i;
    }

    return result;
}

// Calculate two quaternion multiplication
inline Quaternion QuaternionMultiply(Quaternion q1, Quaternion q2)
{
    Quaternion result = { 0 };

    float qax = q1.x, qay = q1.y, qaz = q1.z, qaw = q1.w;
    float qbx = q2.x, qby = q2.y, qbz = q2.z, qbw = q2.w;

    result.x = qax*qbw + qaw*qbx + qay*qbz - qaz*qby;
    result.y = qay*qbw + qaw*qby + qaz*qbx - qax*qbz;
    result.z = qaz*qbw + qaw*qbz + qax*qby - qay*qbx;
    result.w = qaw*qbw - qax*qbx - qay*qby - qaz*qbz;

    return result;
}

// Scale quaternion by float value
inline Quaternion QuaternionScale(Quaternion q, float mul)
{
    Quaternion result = { 0 };

    float qax = q.x, qay = q.y, qaz = q.z, qaw = q.w;

    result.x = qax*mul + qaw*mul + qay*mul - qaz*mul;
    result.y = qay*mul + qaw*mul + qaz*mul - qax*mul;
    result.z = qaz*mul + qaw*mul + qax*mul - qay*mul;
    result.w = qaw*mul - qax*mul - qay*mul - qaz*mul;

    return result;
}

// Divide two quaternions
inline Quaternion QuaternionDivide(Quaternion q1, Quaternion q2)
{
    Quaternion result = { q1.x/q2.x, q1.y/q2.y, q1.z/q2.z, q1.w/q2.w };
    return result;
}

// Calculate linear interpolation between two quaternions
inline Quaternion QuaternionLerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = { 0 };

    result.x = q1.x + amount*(q2.x - q1.x);
    result.y = q1.y + amount*(q2.y - q1.y);
    result.z = q1.z + amount*(q2.z - q1.z);
    result.w = q1.w + amount*(q2.w - q1.w);

    return result;
}

// Calculate slerp-optimized interpolation between two quaternions
inline Quaternion QuaternionNlerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = QuaternionLerp(q1, q2, amount);
    result = QuaternionNormalize(result);

    return result;
}

// Calculates spherical linear interpolation between two quaternions
inline Quaternion QuaternionSlerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = { 0 };

    float cosHalfTheta = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;

    if (cosHalfTheta < 0)
    {
        q2.x = -q2.x; q2.y = -q2.y; q2.z = -q2.z; q2.w = -q2.w;
        cosHalfTheta = -cosHalfTheta;
    }

    if (fabs(cosHalfTheta) >= 1.0f) result = q1;
    else if (cosHalfTheta > 0.95f) result = QuaternionNlerp(q1, q2, amount);
    else
    {
        float halfTheta = acosf(cosHalfTheta);
        float sinHalfTheta = sqrtf(1.0f - cosHalfTheta*cosHalfTheta);

        if (fabs(sinHalfTheta) < 0.001f)
        {
            result.x = (q1.x*0.5f + q2.x*0.5f);
            result.y = (q1.y*0.5f + q2.y*0.5f);
            result.z = (q1.z*0.5f + q2.z*0.5f);
            result.w = (q1.w*0.5f + q2.w*0.5f);
        }
        else
        {
            float ratioA = sinf((1 - amount)*halfTheta)/sinHalfTheta;
            float ratioB = sinf(amount*halfTheta)/sinHalfTheta;

            result.x = (q1.x*ratioA + q2.x*ratioB);
            result.y = (q1.y*ratioA + q2.y*ratioB);
            result.z = (q1.z*ratioA + q2.z*ratioB);
            result.w = (q1.w*ratioA + q2.w*ratioB);
        }
    }

    return result;
}

// Calculate quaternion based on the rotation from one vector to another
inline Quaternion QuaternionFromVector3ToVector3(Vector3 from, Vector3 to)
{
    Quaternion result = { 0 };

    float cos2Theta = Vector3DotProduct(from, to);
    Vector3 cross = Vector3CrossProduct(from, to);

    result.x = cross.x;
    result.y = cross.y;
    result.z = cross.z;
    result.w = 1.0f + cos2Theta;     // NOTE: Added QuaternioIdentity()

    // Normalize to essentially nlerp the original and identity to 0.5
    result = QuaternionNormalize(result);

    // Above lines are equivalent to:
    //Quaternion result = QuaternionNlerp(q, QuaternionIdentity(), 0.5f);

    return result;
}

// Get a quaternion for a given rotation matrix
inline Quaternion QuaternionFromMatrix(Matrix mat)
{
    Quaternion result = { 0 };

    if ((mat.m0 > mat.m5) && (mat.m0 > mat.m10))
    {
        float s = sqrtf(1.0f + mat.m0 - mat.m5 - mat.m10)*2;

        result.x = 0.25f*s;
        result.y = (mat.m4 + mat.m1)/s;
        result.z = (mat.m2 + mat.m8)/s;
        result.w = (mat.m9 - mat.m6)/s;
    }
    else if (mat.m5 > mat.m10)
    {
        float s = sqrtf(1.0f + mat.m5 - mat.m0 - mat.m10)*2;
        result.x = (mat.m4 + mat.m1)/s;
        result.y = 0.25f*s;
        result.z = (mat.m9 + mat.m6)/s;
        result.w = (mat.m2 - mat.m8)/s;
    }
    else
    {
        float s = sqrtf(1.0f + mat.m10 - mat.m0 - mat.m5)*2;
        result.x = (mat.m2 + mat.m8)/s;
        result.y = (mat.m9 + mat.m6)/s;
        result.z = 0.25f*s;
        result.w = (mat.m4 - mat.m1)/s;
    }

    return result;
}

// Get a matrix for a given quaternion
inline Matrix QuaternionToMatrix(Quaternion q)
{
    Matrix result = MatrixIdentity();

    float a2 = q.x*q.x;
    float b2 = q.y*q.y;
    float c2 = q.z*q.z;
    float ac = q.x*q.z;
    float ab = q.x*q.y;
    float bc = q.y*q.z;
    float ad = q.w*q.x;
    float bd = q.w*q.y;
    float cd = q.w*q.z;

    result.m0 = 1 - 2*(b2 + c2);
    result.m1 = 2*(ab + cd);
    result.m2 = 2*(ac - bd);

    result.m4 = 2*(ab - cd);
    result.m5 = 1 - 2*(a2 + c2);
    result.m6 = 2*(bc + ad);

    result.m8 = 2*(ac + bd);
    result.m9 = 2*(bc - ad);
    result.m10 = 1 - 2*(a2 + b2);

    return result;
}

// Get rotation quaternion for an angle and axis
// NOTE: angle must be provided in radians
inline Quaternion QuaternionFromAxisAngle(Vector3 axis, float angle)
{
    Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };

    if (Vector3Length(axis) != 0.0f)

    angle *= 0.5f;

    axis = Vector3Normalize(axis);

    float sinres = sinf(angle);
    float cosres = cosf(angle);

    result.x = axis.x*sinres;
    result.y = axis.y*sinres;
    result.z = axis.z*sinres;
    result.w = cosres;

    result = QuaternionNormalize(result);

    return result;
}

// Get the rotation angle and axis for a given quaternion
inline void QuaternionToAxisAngle(Quaternion q, Vector3 *outAxis, float *outAngle)
{
    if (fabs(q.w) > 1.0f) q = QuaternionNormalize(q);

    Vector3 resAxis = { 0.0f, 0.0f, 0.0f };
    float resAngle = 2.0f*acosf(q.w);
    float den = sqrtf(1.0f - q.w*q.w);

    if (den > 0.0001f)
    {
        resAxis.x = q.x/den;
        resAxis.y = q.y/den;
        resAxis.z = q.z/den;
    }
    else
    {
        // This occurs when the angle is zero.
        // Not a problem: just set an arbitrary normalized axis.
        resAxis.x = 1.0f;
    }

    *outAxis = resAxis;
    *outAngle = resAngle;
}

// Get the quaternion equivalent to Euler angles
// NOTE: Rotation order is ZYX
inline Quaternion QuaternionFromEuler(float pitch, float yaw, float roll)
{
    Quaternion q = { 0 };

    float x0 = cosf(pitch*0.5f);
    float x1 = sinf(pitch*0.5f);
    float y0 = cosf(yaw*0.5f);
    float y1 = sinf(yaw*0.5f);
    float z0 = cosf(roll*0.5f);
    float z1 = sinf(roll*0.5f);

    q.x = x1*y0*z0 - x0*y1*z1;
    q.y = x0*y1*z0 + x1*y0*z1;
    q.z = x0*y0*z1 - x1*y1*z0;
    q.w = x0*y0*z0 + x1*y1*z1;

    return q;
}

// Get the Euler angles equivalent to quaternion (roll, pitch, yaw)
// NOTE: Angles are returned in a Vector3 struct in degrees
inline Vector3 QuaternionToEuler(Quaternion q)
{
    Vector3 result = { 0 };

    // roll (x-axis rotation)
    float x0 = 2.0f*(q.w*q.x + q.y*q.z);
    float x1 = 1.0f - 2.0f*(q.x*q.x + q.y*q.y);
    result.x = atan2f(x0, x1)*RAD2DEG;

    // pitch (y-axis rotation)
    float y0 = 2.0f*(q.w*q.y - q.z*q.x);
    y0 = y0 > 1.0f ? 1.0f : y0;
    y0 = y0 < -1.0f ? -1.0f : y0;
    result.y = asinf(y0)*RAD2DEG;

    // yaw (z-axis rotation)
    float z0 = 2.0f*(q.w*q.z + q.x*q.y);
    float z1 = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);
    result.z = atan2f(z0, z1)*RAD2DEG;

    return result;
}

// Transform a quaternion given a transformation matrix
inline Quaternion QuaternionTransform(Quaternion q, Matrix mat)
{
    Quaternion result = { 0 };

    result.x = mat.m0*q.x + mat.m4*q.y + mat.m8*q.z + mat.m12*q.w;
    result.y = mat.m1*q.x + mat.m5*q.y + mat.m9*q.z + mat.m13*q.w;
    result.z = mat.m2*q.x + mat.m6*q.y + mat.m10*q.z + mat.m14*q.w;
    result.w = mat.m3*q.x + mat.m7*q.y + mat.m11*q.z + mat.m15*q.w;

    return result;
}

// Projects a Vector3 from screen space into object space
inline Vector3 Vector3Unproject(Vector3 source, Matrix projection, Matrix view)
{
    Vector3 result = { 0.0f, 0.0f, 0.0f };

    // Calculate unproject matrix (multiply view patrix by projection matrix) and invert it
    Matrix matViewProj = MatrixMultiply(view, projection);
    matViewProj = MatrixInvert(matViewProj);

    // Create quaternion from source point
    Quaternion quat = { source.x, source.y, source.z, 1.0f };

    // Multiply quat point by unproject matrix
    quat = QuaternionTransform(quat, matViewProj);

    // Normalized world points in vectors
    result.x = quat.x/quat.w;
    result.y = quat.y/quat.w;
    result.z = quat.z/quat.w;

    return result;
}