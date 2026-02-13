#ifndef MATH_H
#define MATH_H

namespace Kazia {

namespace math {

// 2D 向量
struct float2 {
    float x, y;
    
    float2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    
    float2 operator+(const float2& other) const {
        return float2(x + other.x, y + other.y);
    }
    
    float2 operator-(const float2& other) const {
        return float2(x - other.x, y - other.y);
    }
    
    float2 operator*(float scalar) const {
        return float2(x * scalar, y * scalar);
    }
};

// 3D 向量
struct float3 {
    float x, y, z;
    
    float3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
    
    float3 operator+(const float3& other) const {
        return float3(x + other.x, y + other.y, z + other.z);
    }
    
    float3 operator-(const float3& other) const {
        return float3(x - other.x, y - other.y, z - other.z);
    }
    
    float3 operator*(float scalar) const {
        return float3(x * scalar, y * scalar, z * scalar);
    }
    
    float3 operator/(float scalar) const {
        return float3(x / scalar, y / scalar, z / scalar);
    }
};

// 4x4 矩阵
struct mat4f {
    float m[16];
    
    mat4f() {
        for (int i = 0; i < 16; i++) {
            m[i] = 0.0f;
        }
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }
};

} // namespace math

} // namespace Kazia

#endif // MATH_H