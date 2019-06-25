#pragma once

#include "util.h"
#include "swizzle.h"
#include <cassert>
#include <cmath>
#include <iostream>

#ifdef WIN32
#undef min
#undef max
#endif

// Defines a thin wrapper around fixed size C-style arrays, using template parameters,
// which is useful for dealing with vectors of different dimensions.
// For example, float[3] is equivalent to Vec<3,float>.
// Entries in the vector are accessed with the overloaded [] operator, so
// for example if x is a Vec<3,float>, then the middle entry is x[1].
// For convenience, there are a number of typedefs for abbreviation:
//   Vec<3,float> -> Vec3f
//   Vec<2,int>   -> Vec2i
// and so on.
// Arithmetic operators are appropriately overloaded, and functions are defined
// for additional operations (such as dot-products, norms, cross-products, etc.)

// Vec3, Vec3 and Vec4 have template specialisations,
// this provides x, y, z and w members union (with r, g, b and a)
// additional xy, xyz members are provided to truncate to a smaller vec
// swizzles can be also performed in shader style xx, yy, yx... etc.

// functions for quickly intialising common values provided:
// one(), zero(), unit_x(), unit_y(), unit_z()
// red(), green(), blue(), magenta(), yellow(), cyan() for colours

template <size_t N, typename T>
struct Vec
{
    T v[N];

    Vec<N, T>(void)
    {
    }

    explicit Vec<N, T>(T value_for_all)
    {
        for (size_t i = 0; i < N; ++i)
            v[i] = value_for_all;
    }

    template <typename S>
    explicit Vec<N, T>(const S* source)
    {
        for (size_t i = 0; i < N; ++i)
            v[i] = (T)source[i];
    }

    template <typename S>
    explicit Vec<N, T>(const Vec<N, S>& source)
    {
        for (size_t i = 0; i < N; ++i)
            v[i] = (T)source[i];
    }

    Vec<N, T>(T v0, T v1)
    {
        assert(N == 2);
        v[0] = v0;
        v[1] = v1;
    }

    Vec<N, T>(T v0, T v1, T v2)
    {
        assert(N == 3);
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }

    Vec<N, T>(T v0, T v1, T v2, T v3)
    {
        assert(N == 4);
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        v[3] = v3;
    }

    Vec<N, T>(T v0, T v1, T v2, T v3, T v4)
    {
        assert(N == 5);
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        v[3] = v3;
        v[4] = v4;
    }

    Vec<N, T>(T v0, T v1, T v2, T v3, T v4, T v5)
    {
        assert(N == 6);
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        v[3] = v3;
        v[4] = v4;
        v[5] = v5;
    }

    T& operator[](size_t index)
    {
        assert(0 <= index && (size_t)index < N);
        return v[index];
    }

    const T& operator[](size_t index) const
    {
        assert(0 <= index && (size_t)index < N);
        return v[index];
    }
};

// vector swizzles.
template <typename T, size_t... SW>
struct Swizzle
{
    T v[sizeof...(SW)];
    
    template <typename T2, size_t... SW2>
    Swizzle<T, SW...>& operator=(const Swizzle<T2, SW2...>& lhs)
    {
        std::vector<size_t> i1 = {SW...};
        std::vector<size_t> i2 = {SW2...};
        
        static_assert(sizeof...(SW) == sizeof...(SW2), "error: assigning swizzle of different dimensions");
        
        size_t ss = i1.size();
        for(size_t x = 0; x < ss; ++x)
            v[i1[x]] = lhs.v[i2[x]];

        return *this;
    }
    
    template <size_t N, typename T2>
    Swizzle<T, SW...>& operator=(const Vec<N, T2>& lhs)
    {
        static_assert(sizeof...(SW) == N, "error: assigning vector to swizzle of different dimensions");
        
        std::vector<size_t> i1 = {SW...};
        for(size_t x = 0; x < N; ++x)
            v[i1[x]] = lhs.v[x];
    }
};

// Template specialisations for 2, 3, 4
template <typename T>
struct Vec<2, T>
{
    union {
        T v[2];
        struct {
            T x, y;
        };
        swizzle_v2;
    };

    Vec<2, T>(void)
    {
    }

    explicit Vec<2, T>(T value_for_all)
    {
        for (size_t i = 0; i < 2; ++i)
            v[i] = value_for_all;
    }

    template <class S>
    explicit Vec<2, T>(const S* source)
    {
        for (size_t i = 0; i < 2; ++i)
            v[i] = (T)source[i];
    }

    template <class S>
    explicit Vec<2, T>(const Vec<2, S>& source)
    {
        for (size_t i = 0; i < 2; ++i)
            v[i] = (T)source[i];
    }
    
    template<typename T2, size_t... SW>
    Vec<2, T>(const Swizzle<T2, SW...>& lhs)
    {
        std::vector<size_t> ii = {SW...};
        for(size_t i = 0; i < ii.size(); ++i)
            v[i] = lhs.v[ii[i]];
    }
    
    template<typename T2, size_t... SW>
    Vec<2, T>& operator=(const Swizzle<T2, SW...>& lhs)
    {
        std::vector<size_t> ii = {SW...};
        for(size_t i = 0; i < ii.size(); ++i)
            v[i] = lhs.v[ii[i]];
        
        return *this;
    }

    Vec<2, T>(T v0, T v1)
    {
        v[0] = v0;
        v[1] = v1;
    }

    T& operator[](size_t index)
    {
        return v[index];
    }

    const T& operator[](size_t index) const
    {
        return v[index];
    }

    inline static Vec<2, T> one()
    {
        return Vec<2, T>(1, 1);
    }

    inline static Vec<2, T> zero()
    {
        return Vec<2, T>(0, 0);
    }

    inline static Vec<2, T> flt_max()
    {
        return Vec<2, T>(FLT_MAX, FLT_MAX);
    }

    inline static Vec<2, T> unit_x()
    {
        return Vec<2, T>(1, 0);
    }

    inline static Vec<2, T> unit_y()
    {
        return Vec<2, T>(0, 1);
    }

    inline static Vec<2, T> vmax(Vec<2, T> a, Vec<2, T> b)
    {
        Vec<2, T> v(max(a.x, b.x), max(a.y, b.y));
        return v;
    }

    inline static Vec<2, T> vmin(Vec<2, T> a, Vec<2, T> b)
    {
        Vec<2, T> v(min(a.x, b.x), min(a.y, b.y));
        return v;
    }
};

template <typename T>
struct Vec<3, T>
{
    union {
        T v[3];
        struct
        {
            T x, y, z;
        };
        struct
        {
            T r, g, b;
        };
        Vec<2, T> xy;
        swizzle_v3;
    };

    Vec<3, T>(void)
    {
    }

    explicit Vec<3, T>(T value_for_all)
    {
        for (size_t i = 0; i < 3; ++i)
            v[i] = value_for_all;
    }

    template <class S>
    explicit Vec<3, T>(const S* source)
    {
        for (size_t i = 0; i < 3; ++i)
            v[i] = (T)source[i];
    }

    template <class S>
    explicit Vec<3, T>(const Vec<3, S>& source)
    {
        for (size_t i = 0; i < 3; ++i)
            v[i] = (T)source[i];
    }

    Vec<3, T>(T v0, T v1, T v2)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }

    Vec<3, T>(const Vec<2, T>& v2, T _z)
    {
        for (size_t i = 0; i < 2; ++i)
            v[i] = (T)v2[i];

        v[2] = _z;
    }
    
    template<typename T2, size_t... SW>
    Vec<3, T>(const Swizzle<T2, SW...>& lhs)
    {
        std::vector<size_t> ii = {SW...};
        for(size_t i = 0; i < ii.size(); ++i)
            v[i] = lhs.v[ii[i]];
    }
    
    template<typename T2, size_t... SW>
    Vec<3, T>& operator=(const Swizzle<T2, SW...>& lhs)
    {
        std::vector<size_t> ii = {SW...};
        for(size_t i = 0; i < ii.size(); ++i)
            v[i] = lhs.v[ii[i]];
        
        return *this;
    }

    T& operator[](size_t index)
    {
        return v[index];
    }

    const T& operator[](size_t index) const
    {
        return v[index];
    }

    inline static Vec<3, T> one()
    {
        return Vec<3, T>(1, 1, 1);
    }

    inline static Vec<3, T> zero()
    {
        return Vec<3, T>(0, 0, 0);
    }

    inline static Vec<3, T> flt_max()
    {
        return Vec<3, T>(FLT_MAX, FLT_MAX, FLT_MAX);
    }

    inline static Vec<3, T> unit_x()
    {
        return Vec<3, T>(1, 0, 0);
    }

    inline static Vec<3, T> unit_y()
    {
        return Vec<3, T>(0, 1, 0);
    }

    inline static Vec<3, T> unit_z()
    {
        return Vec<3, T>(0, 0, 1);
    }

    inline static Vec<3, T> white()
    {
        return Vec<3, T>(1, 1, 1);
    }

    inline static Vec<3, T> black()
    {
        return Vec<3, T>(0, 0, 0);
    }

    inline static Vec<3, T> red()
    {
        return Vec<3, T>(1, 0, 0);
    }

    inline static Vec<3, T> green()
    {
        return Vec<3, T>(0, 1, 0);
    }

    inline static Vec<3, T> blue()
    {
        return Vec<3, T>(0, 0, 1);
    }

    inline static Vec<3, T> yellow()
    {
        return Vec<3, T>(1, 1, 0);
    }

    inline static Vec<3, T> cyan()
    {
        return Vec<3, T>(0, 1, 1);
    }

    inline static Vec<3, T> magenta()
    {
        return Vec<3, T>(1, 0, 1);
    }

    inline static Vec<3, T> vmax(Vec<3, T> a, Vec<3, T> b)
    {
        Vec<3, T> v(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
        return v;
    }

    inline static Vec<3, T> vmin(Vec<3, T> a, Vec<3, T> b)
    {
        Vec<3, T> v(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
        return v;
    }
};

template <typename T>
struct Vec<4, T>
{
    union {
        T v[4];
        struct {
            T x, y, z, w;
        };
        struct {
            T r, g, b, a;
        };
        Vec<2, T> xy;
        Vec<3, T> xyz;
        
        swizzle_v4;
    };

    Vec<4, T>(void)
    {
    }
    
    Vec<4, T>& operator=(const Vec<4, T>& lhs)
    {
        for(size_t i = 0; i < 4; ++i)
            v[i] = lhs.v[i];
        
        return *this;
    }
    
    template<typename T2, size_t... SW>
    Vec<4, T>(const Swizzle<T2, SW...>& lhs)
    {
        std::vector<size_t> ii = {SW...};
        for(size_t i = 0; i < ii.size(); ++i)
            v[i] = lhs.v[ii[i]];
    }
    
    template<typename T2, size_t... SW>
    Vec<4, T>& operator=(const Swizzle<T2, SW...>& lhs)
    {
        std::vector<size_t> ii = {SW...};
        for(size_t i = 0; i < ii.size(); ++i)
            v[i] = lhs.v[ii[i]];
        
        return *this;
    }
    
    explicit Vec<4, T>(T value_for_all)
    {
        for (size_t i = 0; i < 4; ++i)
            v[i] = value_for_all;
    }

    template <class S>
    explicit Vec<4, T>(const S* source)
    {
        for (size_t i = 0; i < 4; ++i)
            v[i] = (T)source[i];
    }

    template <class S>
    explicit Vec<4, T>(const Vec<4, S>& source)
    {
        for (size_t i = 0; i < 4; ++i)
            v[i] = (T)source[i];
    }

    Vec<4, T>(T v0, T v1, T v2, T v3)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        v[3] = v3;
    }

    Vec<4, T>(const Vec<3, T>& v3, T _w)
    {
        for (size_t i = 0; i < 3; ++i)
            v[i] = (T)v3[i];

        v[3] = _w;
    }

    T& operator[](size_t index)
    {
        return v[index];
    }

    const T& operator[](size_t index) const
    {
        return v[index];
    }

    inline static Vec<4, T> one()
    {
        return Vec<4, T>(1, 1, 1, 1);
    }

    inline static Vec<4, T> zero()
    {
        return Vec<4, T>(0, 0, 0, 0);
    }

    inline static Vec<4, T> unit_x()
    {
        return Vec<4, T>(1, 0, 0, 0);
    }

    inline static Vec<4, T> unit_y()
    {
        return Vec<4, T>(0, 1, 0, 0);
    }

    inline static Vec<4, T> unit_z()
    {
        return Vec<4, T>(0, 0, 1, 0);
    }

    inline static Vec<4, T> white()
    {
        return Vec<4, T>(1, 1, 1, 1);
    }

    inline static Vec<4, T> black()
    {
        return Vec<3, T>(0, 0, 0, 1);
    }

    inline static Vec<4, T> red()
    {
        return Vec<4, T>(1, 0, 0, 1);
    }

    inline static Vec<4, T> green()
    {
        return Vec<4, T>(0, 1, 0, 1);
    }

    inline static Vec<4, T> blue()
    {
        return Vec<4, T>(0, 0, 1, 1);
    }

    inline static Vec<4, T> yellow()
    {
        return Vec<4, T>(1, 1, 0, 1);
    }

    inline static Vec<4, T> cyan()
    {
        return Vec<4, T>(0, 1, 1, 1);
    }

    inline static Vec<4, T> magenta()
    {
        return Vec<4, T>(1, 0, 1, 1);
    }
};

// Abbreviations

typedef Vec<2, double>         Vec2d;
typedef Vec<2, float>          Vec2f;
typedef Vec<2, int>            Vec2i;
typedef Vec<2, unsigned int>   Vec2ui;
typedef Vec<2, short>          Vec2s;
typedef Vec<2, unsigned short> Vec2us;
typedef Vec<2, char>           Vec2c;
typedef Vec<2, unsigned char>  Vec2uc;

typedef Vec<3, double>         Vec3d;
typedef Vec<3, float>          Vec3f;
typedef Vec<3, int>            Vec3i;
typedef Vec<3, unsigned int>   Vec3ui;
typedef Vec<3, short>          Vec3s;
typedef Vec<3, unsigned short> Vec3us;
typedef Vec<3, char>           Vec3c;
typedef Vec<3, unsigned char>  Vec3uc;

typedef Vec<4, double>         Vec4d;
typedef Vec<4, float>          Vec4f;
typedef Vec<4, int>            Vec4i;
typedef Vec<4, unsigned int>   Vec4ui;
typedef Vec<4, short>          Vec4s;
typedef Vec<4, unsigned short> Vec4us;
typedef Vec<4, char>           Vec4c;
typedef Vec<4, unsigned char>  Vec4uc;

typedef Vec2i vec2i;
typedef Vec2f vec2f;

typedef Vec3f  vec3f;
typedef Vec3ui vec3ui;
typedef Vec3i  vec3i;

typedef Vec4f vec4f;
typedef Vec4i vec4i;

typedef Vec4f float4;
typedef Vec3f float3;
typedef Vec2f float2;

// For bullet
struct lw_vec3f
{
    float x, y, z;

    inline lw_vec3f& operator=(const vec3f& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;

        return *this;
    }
};

// Operators

template <size_t N, typename T>
Vec<N, T> operator+=(Vec<N, T>& lhs, const Vec<N, T>& rhs)
{
    for (size_t i = 0; i < N; ++i)
        lhs[i] += rhs[i];
    return lhs;
}

template <size_t N, typename T>
Vec<N, T> operator+(const Vec<N, T>& lhs, const Vec<N, T>& rhs)
{
    Vec<N, T> sum(lhs);
    sum += rhs;
    return sum;
}

template <size_t N, typename T>
Vec<N, T> operator-=(Vec<N, T>& lhs, const Vec<N, T>& rhs)
{
    for (size_t i = 0; i < N; ++i)
        lhs[i] -= rhs[i];
    return lhs;
}

template <size_t N, typename T>
Vec<N, T> operator-(const Vec<N, T>& rhs) // unary minus
{
    Vec<N, T> negative;
    for (size_t i = 0; i < N; ++i)
        negative.v[i] = -rhs.v[i];
    return negative;
}

template <size_t N, typename T>
Vec<N, T> operator-(const Vec<N, T>& lhs, const Vec<N, T>& rhs) // (binary) subtraction
{
    Vec<N, T> diff(lhs);
    diff -= rhs;
    return diff;
}

template <size_t N, typename T>
Vec<N, T> operator*=(Vec<N, T>& lhs, T a)
{
    for (size_t i = 0; i < N; ++i)
        lhs.v[i] *= a;
    return lhs;
}

template <size_t N, typename T>
Vec<N, T> operator*(const Vec<N, T>& lhs, T a)
{
    Vec<N, T> w(lhs);
    w *= a;
    return w;
}

template <size_t N, typename T>
Vec<N, T> operator*=(Vec<N, T>& lhs, const Vec<N, T>& rhs)
{
    for (size_t i = 0; i < N; ++i)
        lhs.v[i] *= rhs.v[i];
    return lhs;
}

template <size_t N, typename T>
Vec<N, T> operator*(const Vec<N, T>& lhs, const Vec<N, T>& rhs)
{
    Vec<N, T> componentwise_product;
    for (size_t i = 0; i < N; ++i)
        componentwise_product[i] = lhs.v[i] * rhs.v[i];
    return componentwise_product;
}

template <size_t N, typename T>
Vec<N, T> operator/=(Vec<N, T>& lhs, T a)
{
    for (size_t i = 0; i < N; ++i)
        lhs.v[i] /= a;
    return lhs;
}

template <size_t N, typename T>
Vec<N, T> operator/(const Vec<N, T>& lhs, T a)
{
    Vec<N, T> w(lhs);
    w /= a;
    return w;
}

template <size_t N, typename T>
Vec<N, T> operator/=(Vec<N, T>& lhs, const Vec<N, T>& rhs)
{
    for (size_t i = 0; i < N; ++i)
        lhs.v[i] /= rhs.v[i];
    return lhs;
}

template <size_t N, typename T>
Vec<N, T> operator/(const Vec<N, T>& lhs, const Vec<N, T>& rhs)
{
    Vec<N, T> componentwise_divide;
    for (size_t i = 0; i < N; ++i)
        componentwise_divide[i] = lhs.v[i] / rhs.v[i];
    return componentwise_divide;
}

template <size_t N, typename T>
bool operator==(Vec<N, T>& lhs, const Vec<N, T>& rhs)
{
    return (equals(lhs, rhs));
}

template <size_t N, typename T>
bool operator!=(Vec<N, T>& lhs, const Vec<N, T>& rhs)
{
    return (!equals(lhs, rhs));
}

// Non standard operations with scalars

template <size_t N, typename T>
Vec<N, T> operator+=(Vec<N, T>& lhs, T a)
{
    for (size_t i = 0; i < N; ++i)
        lhs[i] += a;
    return lhs;
}

template <size_t N, typename T>
Vec<N, T> operator+(const Vec<N, T>& lhs, T a)
{
    Vec<N, T> sum(lhs);
    sum += a;
    return sum;
}

template <size_t N, typename T>
Vec<N, T> operator-=(Vec<N, T>& lhs, T a)
{
    for (size_t i = 0; i < N; ++i)
        lhs[i] -= a;
    return lhs;
}

template <size_t N, typename T>
Vec<N, T> operator-(const Vec<N, T>& lhs, T a)
{
    Vec<N, T> sum(lhs);
    sum -= a;
    return sum;
}

// Free Functions
template <size_t N, typename T>
T component_wise_min(const Vec<N, T>& v)
{
    T _min = v.v[0];
    for (size_t i = 1; i < N; ++i)
        _min = v.v[i] < _min ? v.v[i] : _min;

    return _min;
}

template <size_t N, typename T>
T component_wise_max(const Vec<N, T>& v)
{
    T _max = v.v[0];
    for (size_t i = 1; i < N; ++i)
        _max = v.v[i] > _max ? v.v[i] : _max;

    return _max;
}

template <size_t N, typename T>
inline Vec<N, T> lerp(const Vec<N, T>& value0, const Vec<N, T>& value1, T f)
{
    return value0 * (1 - f) + value1 * f;
}

template <size_t N, typename T>
inline Vec<N, T> lerp(const Vec<N, T>& value0, const Vec<N, T>& value1, const Vec<N, T>& f)
{
    return value0 * (1 - f) + value1 * f;
}

template <size_t N, typename T>
inline Vec<N, T> vclamp(const Vec<N, T>& a, T lower, T upper)
{
    Vec<N, T> res = a;
    for (size_t i = 0; i < N; ++i)
    {
        if (a[i] < lower)
            res[i] = lower;
        else if (a[i] > upper)
            res[i] = upper;
    }

    return res;
}

template <size_t N, typename T>
inline Vec<N, T> vclamp(const Vec<N, T>& a, const Vec<N, T>& lower, const Vec<N, T>& upper)
{
    Vec<N, T> res = a;
    for (size_t i = 0; i < N; ++i)
    {
        if (a[i] < lower[i])
            res[i] = lower[i];
        else if (a[i] > upper[i])
            res[i] = upper[i];
    }

    return res;
}

template <size_t N, typename T>
inline Vec<N, T> saturate(Vec<N, T>& a)
{
    Vec<N, T> res = a;
    for (size_t i = 0; i < N; ++i)
    {
        if (a[i] < 0)
            res[i] = 0;
        else if (a[i] > 1)
            res[i] = 1;
    }

    a = res;
    return res;
}

template <size_t N, typename T>
inline bool all(const Vec<N, T>& a)
{
    for (size_t i = 0; i < N; ++i)
        if (a[i] == 0)
            return false;

    return true;
}

template <size_t N, typename T>
inline bool any(const Vec<N, T>& a)
{
    for (size_t i = 0; i < N; ++i)
        if (a[i] != 0)
            return true;

    return false;
}

template <size_t N, typename T>
inline Vec<N, T> smooth_step(T r, const Vec<N, T>& edge0, const Vec<N, T>& edge1)
{
    Vec<N, T> res;
    for (size_t i = 0; i < N; ++i)
        res[i] = smooth_step(r, edge0[i], edge1[i], 0, 1);

    return res;
}

template <size_t N, typename T>
inline Vec<N, T> step(const Vec<N, T>& value0, const Vec<N, T>& value1)
{
    Vec<N, T> res;
    for (size_t i = 0; i < N; ++i)
        res[i] = value0[i] > value1[i] ? 1 : 0;

    return res;
}

template <size_t N, typename T>
bool equals(const Vec<N, T>& lhs, const Vec<N, T>& rhs)
{
    for (size_t i = 0; i < N; ++i)
        if (lhs[i] != rhs[i])
            return false;

    return true;
}

template <size_t N, typename T>
bool almost_equal(const Vec<N, T>& lhs, const Vec<N, T>& rhs, const T& epsilon)
{
    if (dist(lhs, rhs) < epsilon)
        return true;

    return false;
}

template <size_t N, typename T>
bool nonzero(const Vec<N, T>& v)
{
    for (size_t i = 0; i < N; ++i)
        if (v[i])
            return true;
    return false;
}

template <size_t N, typename T>
T mag2(const Vec<N, T>& a)
{
    T l = sqr(a.v[0]);
    for (size_t i = 1; i < N; ++i)
        l += sqr(a.v[i]);
    return l;
}

template <size_t N, typename T>
T mag(const Vec<N, T>& a)
{
    return sqrt(mag2(a));
}

template <size_t N, typename T>
inline T dist2(const Vec<N, T>& a, const Vec<N, T>& b)
{
    T d = sqr(a.v[0] - b.v[0]);
    for (size_t i = 1; i < N; ++i)
        d += sqr(a.v[i] - b.v[i]);
    return d;
}

template <size_t N, typename T>
inline T dist(const Vec<N, T>& a, const Vec<N, T>& b)
{
    return std::sqrt(dist2(a, b));
}

template <size_t N, typename T>
inline void normalize(Vec<N, T>& a)
{
    a /= mag(a);
}

template <size_t N, typename T>
inline Vec<N, T> normalized(const Vec<N, T>& a)
{
    return a / mag(a);
}

template <size_t N, typename T>
inline T infnorm(const Vec<N, T>& a)
{
    T d = std::fabs(a.v[0]);
    for (size_t i = 1; i < N; ++i)
        d = max(std::fabs(a.v[i]), d);
    return d;
}

template <size_t N, typename T>
inline void normalise(Vec<N, T>& a)
{
    a /= mag(a);
}

template <size_t N, typename T>
inline Vec<N, T> normalised(const Vec<N, T>& a)
{
    return a / mag(a);
}

template <size_t N, typename T>
void zero(Vec<N, T>& a)
{
    for (size_t i = 0; i < N; ++i)
        a.v[i] = 0;
}

template <size_t N, typename T>
std::ostream& operator<<(std::ostream& out, const Vec<N, T>& v)
{
    out << v.v[0];
    for (size_t i = 1; i < N; ++i)
        out << ' ' << v.v[i];
    return out;
}

template <size_t N, typename T>
std::istream& operator>>(std::istream& in, Vec<N, T>& v)
{
    in >> v.v[0];
    for (size_t i = 1; i < N; ++i)
        in >> v.v[i];
    return in;
}

template <size_t N, typename T>
inline bool operator==(const Vec<N, T>& a, const Vec<N, T>& b)
{
    bool         t = (a.v[0] == b.v[0]);
    size_t i = 1;
    while (i < N && t)
    {
        t = t && (a.v[i] == b.v[i]);
        ++i;
    }
    return t;
}

template <size_t N, typename T>
inline bool operator!=(const Vec<N, T>& a, const Vec<N, T>& b)
{
    bool         t = (a.v[0] != b.v[0]);
    size_t i = 1;
    while (i < N && !t)
    {
        t = t || (a.v[i] != b.v[i]);
        ++i;
    }
    return t;
}

template <size_t N, typename T>
inline Vec<N, T> operator*(T a, const Vec<N, T>& v)
{
    Vec<N, T> w(v);
    w *= a;
    return w;
}

template <size_t N, typename T>
inline T min(const Vec<N, T>& a)
{
    T m = a.v[0];
    for (size_t i = 1; i < N; ++i)
        if (a.v[i] < m)
            m = a.v[i];
    return m;
}

template <size_t N, typename T>
inline Vec<N, T> min_union(const Vec<N, T>& a, const Vec<N, T>& b)
{
    Vec<N, T> m;
    for (size_t i = 0; i < N; ++i)
        (a.v[i] < b.v[i]) ? m.v[i] = a.v[i] : m.v[i] = b.v[i];
    return m;
}

template <size_t N, typename T>
inline Vec<N, T> max_union(const Vec<N, T>& a, const Vec<N, T>& b)
{
    Vec<N, T> m;
    for (size_t i = 0; i < N; ++i)
        (a.v[i] > b.v[i]) ? m.v[i] = a.v[i] : m.v[i] = b.v[i];
    return m;
}

template <size_t N, typename T>
inline T max(const Vec<N, T>& a)
{
    T m = a.v[0];
    for (size_t i = 1; i < N; ++i)
        if (a.v[i] > m)
            m = a.v[i];
    return m;
}

template <size_t N, typename T>
inline T dot(const Vec<N, T>& a, const Vec<N, T>& b)
{
    T d = a.v[0] * b.v[0];
    for (size_t i = 1; i < N; ++i)
        d += a.v[i] * b.v[i];
    return d;
}

template <typename T>
inline Vec<2, T> rotate(const Vec<2, T>& a, float angle)
{
    T c = cos(angle);
    T s = sin(angle);
    return Vec<2, T>(c * a[0] - s * a[1], s * a[0] + c * a[1]); // counter-clockwise rotation
}

template <typename T>
inline Vec<2, T> perp(const Vec<2, T>& a)
{
    return Vec<2, T>(-a.v[1], a.v[0]);
} // counter-clockwise rotation by 90 degrees

template <typename T>
inline T cross(const Vec<2, T>& a, const Vec<2, T>& b)
{
    return a.v[0] * b.v[1] - a.v[1] * b.v[0];
}

template <typename T>
inline Vec<3, T> cross(const Vec<3, T>& a, const Vec<3, T>& b)
{
    return Vec<3, T>(a.v[1] * b.v[2] - a.v[2] * b.v[1], a.v[2] * b.v[0] - a.v[0] * b.v[2], a.v[0] * b.v[1] - a.v[1] * b.v[0]);
}

template <typename T>
inline T triple(const Vec<3, T>& a, const Vec<3, T>& b, const Vec<3, T>& c)
{
    return a.v[0] * (b.v[1] * c.v[2] - b.v[2] * c.v[1]) + a.v[1] * (b.v[2] * c.v[0] - b.v[0] * c.v[2]) +
           a.v[2] * (b.v[0] * c.v[1] - b.v[1] * c.v[0]);
}

template <size_t N, typename T>
inline size_t hash(const Vec<N, T>& a)
{
    size_t h = a.v[0];
    for (size_t i = 1; i < N; ++i)
        h = hash(h ^ a.v[i]);
    return h;
}

template <size_t N, typename T>
inline void assign(const Vec<N, T>& a, T& a0, T& a1)
{
    assert(N == 2);
    a0 = a.v[0];
    a1 = a.v[1];
}

template <size_t N, typename T>
inline void assign(const Vec<N, T>& a, T& a0, T& a1, T& a2)
{
    assert(N == 3);
    a0 = a.v[0];
    a1 = a.v[1];
    a2 = a.v[2];
}

template <size_t N, typename T>
inline void assign(const Vec<N, T>& a, T& a0, T& a1, T& a2, T& a3)
{
    assert(N == 4);
    a0 = a.v[0];
    a1 = a.v[1];
    a2 = a.v[2];
    a3 = a.v[3];
}

template <size_t N, typename T>
inline void assign(const Vec<N, T>& a, T& a0, T& a1, T& a2, T& a3, T& a4, T& a5)
{
    assert(N == 6);
    a0 = a.v[0];
    a1 = a.v[1];
    a2 = a.v[2];
    a3 = a.v[3];
    a4 = a.v[4];
    a5 = a.v[5];
}

template <size_t N, typename T>
inline Vec<N, T> round(const Vec<N, T>& a)
{
    Vec<N, T> rounded;
    for (size_t i = 0; i < N; ++i)
        rounded.v[i] = lround(a.v[i]);
    return rounded;
}

template <size_t N, typename T>
inline Vec<N, T> floor(const Vec<N, T>& a)
{
    Vec<N, T> rounded;
    for (size_t i = 0; i < N; ++i)
        rounded.v[i] = (int)floor(a.v[i]);
    return rounded;
}

template <size_t N, typename T>
inline Vec<N, int> ceil(const Vec<N, T>& a)
{
    Vec<N, T> rounded;
    for (size_t i = 0; i < N; ++i)
        rounded.v[i] = (int)ceil(a.v[i]);
    return rounded;
}

template <size_t N, typename T>
inline Vec<N, T> fabs(const Vec<N, T>& a)
{
    Vec<N, T> result;
    for (size_t i = 0; i < N; ++i)
        result.v[i] = fabs(a.v[i]);
    return result;
}

template <size_t N, typename T>
inline void minmax(const Vec<N, T>& x0, const Vec<N, T>& x1, Vec<N, T>& xmin, Vec<N, T>& xmax)
{
    for (size_t i = 0; i < N; ++i)
        minmax(x0.v[i], x1.v[i], xmin.v[i], xmax.v[i]);
}

template <size_t N, typename T>
inline void minmax(const Vec<N, T>& x0, const Vec<N, T>& x1, const Vec<N, T>& x2, Vec<N, T>& xmin, Vec<N, T>& xmax)
{
    for (size_t i = 0; i < N; ++i)
        minmax(x0.v[i], x1.v[i], x2.v[i], xmin.v[i], xmax.v[i]);
}

template <size_t N, typename T>
inline void minmax(const Vec<N, T>& x0, const Vec<N, T>& x1, const Vec<N, T>& x2, const Vec<N, T>& x3, Vec<N, T>& xmin,
                   Vec<N, T>& xmax)
{
    for (size_t i = 0; i < N; ++i)
        minmax(x0.v[i], x1.v[i], x2.v[i], x3.v[i], xmin.v[i], xmax.v[i]);
}

template <size_t N, typename T>
inline void minmax(const Vec<N, T>& x0, const Vec<N, T>& x1, const Vec<N, T>& x2, const Vec<N, T>& x3, const Vec<N, T>& x4,
                   Vec<N, T>& xmin, Vec<N, T>& xmax)
{
    for (size_t i = 0; i < N; ++i)
        minmax(x0.v[i], x1.v[i], x2.v[i], x3.v[i], x4.v[i], xmin.v[i], xmax.v[i]);
}

template <size_t N, typename T>
inline void minmax(const Vec<N, T>& x0, const Vec<N, T>& x1, const Vec<N, T>& x2, const Vec<N, T>& x3, const Vec<N, T>& x4,
                   const Vec<N, T>& x5, Vec<N, T>& xmin, Vec<N, T>& xmax)
{
    for (size_t i = 0; i < N; ++i)
        minmax(x0.v[i], x1.v[i], x2.v[i], x3.v[i], x4.v[i], x5.v[i], xmin.v[i], xmax.v[i]);
}

template <size_t N, typename T>
inline void update_minmax(const Vec<N, T>& x, Vec<N, T>& xmin, Vec<N, T>& xmax)
{
    for (size_t i = 0; i < N; ++i)
        update_minmax(x[i], xmin[i], xmax[i]);
}
