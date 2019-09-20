#pragma once

#include <algorithm>
#include <cmath>
#include <float.h>
#include <iostream>
#include <vector>

#define MATHS_ALWAYS_INLINE

#ifndef MATHS_ALWAYS_INLINE
#define maths_inline inline
#else
#ifdef _MSC_VER
#define maths_inline __forceinline
#else
#define maths_inline inline __attribute__((always_inline))
#endif
#endif

#ifndef M_PI
const double M_PI = 3.1415926535897932384626433832795;
#endif

#ifdef WIN32
#undef min
#undef max
#elif __linux__
#include <limits.h>
#endif

typedef uint32_t u32;
typedef float    f32;
typedef double   f64;

using std::max;
using std::min;
using std::swap;

template <class T>
inline T sqr(const T& x)
{
    return x * x;
}

template <class T>
inline T cube(const T& x)
{
    return x * x * x;
}

template <class T>
inline T min(T a1, T a2, T a3)
{
    return min(a1, min(a2, a3));
}

template <class T>
inline T min(T a1, T a2, T a3, T a4)
{
    return min(min(a1, a2), min(a3, a4));
}

template <class T>
inline T min(T a1, T a2, T a3, T a4, T a5)
{
    return min(min(a1, a2), min(a3, a4), a5);
}

template <class T>
inline T min(T a1, T a2, T a3, T a4, T a5, T a6)
{
    return min(min(a1, a2), min(a3, a4), min(a5, a6));
}

template <class T>
inline T max(T a1, T a2, T a3)
{
    return max(a1, max(a2, a3));
}

template <class T>
inline T max(T a1, T a2, T a3, T a4)
{
    return max(max(a1, a2), max(a3, a4));
}

template <class T>
inline T max(T a1, T a2, T a3, T a4, T a5)
{
    return max(max(a1, a2), max(a3, a4), a5);
}

template <class T>
inline T max(T a1, T a2, T a3, T a4, T a5, T a6)
{
    return max(max(a1, a2), max(a3, a4), max(a5, a6));
}

template <class T>
inline void minmax(T a1, T a2, T& amin, T& amax)
{
    if (a1 < a2)
    {
        amin = a1;
        amax = a2;
    }
    else
    {
        amin = a2;
        amax = a1;
    }
}

template <class T>
inline void minmax(T a1, T a2, T a3, T& amin, T& amax)
{
    if (a1 < a2)
    {
        if (a1 < a3)
        {
            amin = a1;
            if (a2 < a3)
                amax = a3;
            else
                amax = a2;
        }
        else
        {
            amin = a3;
            if (a1 < a2)
                amax = a2;
            else
                amax = a1;
        }
    }
    else
    {
        if (a2 < a3)
        {
            amin = a2;
            if (a1 < a3)
                amax = a3;
            else
                amax = a1;
        }
        else
        {
            amin = a3;
            amax = a1;
        }
    }
}

template <class T>
inline void minmax(T a1, T a2, T a3, T a4, T& amin, T& amax)
{
    if (a1 < a2)
    {
        if (a3 < a4)
        {
            amin = min(a1, a3);
            amax = max(a2, a4);
        }
        else
        {
            amin = min(a1, a4);
            amax = max(a2, a3);
        }
    }
    else
    {
        if (a3 < a4)
        {
            amin = min(a2, a3);
            amax = max(a1, a4);
        }
        else
        {
            amin = min(a2, a4);
            amax = max(a1, a3);
        }
    }
}

template <class T>
inline void minmax(T a1, T a2, T a3, T a4, T a5, T& amin, T& amax)
{
    amin = min(a1, a2, a3, a4, a5);
    amax = max(a1, a2, a3, a4, a5);
}

template <class T>
inline void minmax(T a1, T a2, T a3, T a4, T a5, T a6, T& amin, T& amax)
{
    amin = min(a1, a2, a3, a4, a5, a6);
    amax = max(a1, a2, a3, a4, a5, a6);
}

template <class T>
inline void update_minmax(T a1, T& amin, T& amax)
{
    if (a1 < amin)
        amin = a1;
    else if (a1 > amax)
        amax = a1;
}

template <class T>
inline void sort(T& a, T& b, T& c)
{
    T temp;
    if (a < b)
    {
        if (a < c)
        {
            if (c < b)
            { // a<c<b
                temp = c;
                c    = b;
                b    = temp;
            } // else: a<b<c
        }
        else
        { // c<a<b
            temp = c;
            c    = b;
            b    = a;
            a    = temp;
        }
    }
    else
    {
        if (b < c)
        {
            if (a < c)
            { // b<a<c
                temp = b;
                b    = a;
                a    = temp;
            }
            else
            { // b<c<a
                temp = b;
                b    = c;
                c    = a;
                a    = temp;
            }
        }
        else
        { // c<b<a
            temp = c;
            c    = a;
            a    = temp;
        }
    }
}

template <class T>
inline T clamp(T a, T lower, T upper)
{
    if (a < lower)
        return lower;
    else if (a > upper)
        return upper;
    else
        return a;
}

// only makes sense with T=float or double
template <class T>
inline T smooth_step(T r)
{
    if (r < 0)
        return 0;
    else if (r > 1)
        return 1;
    return r * r * r * (10 + r * (-15 + r * 6));
}

// only makes sense with T=float or double
template <class T>
inline T smooth_step(T r, T r_lower, T r_upper, T value_lower, T value_upper)
{
    return value_lower + smooth_step((r - r_lower) / (r_upper - r_lower)) * (value_upper - value_lower);
}

// its like smooth step but with linear interpolation
template <class T>
inline T linear_step(T l, T r, T v)
{
    if (v <= l)
        return 0.0;
    
    if (v >= r)
        return 1.0;

    return (v - l) / (r - l);
}

// only makes sense with T=float or double
template <class T>
inline T ramp(T r)
{
    return smooth_step((r + 1) / 2) * 2 - 1;
}

#ifdef WIN32__
inline int lround(double x)
{
    if (x > 0)
        return (x - floor(x) < 0.5) ? (int)floor(x) : (int)ceil(x);
    else
        return (x - floor(x) <= 0.5) ? (int)floor(x) : (int)ceil(x);
}

inline double remainder(double x, double y)
{
    return x - std::floor(x / y + 0.5) * y;
}
#endif

inline unsigned int round_up_to_power_of_two(unsigned int n)
{
    int exponent = 0;
    --n;
    while (n)
    {
        ++exponent;
        n >>= 1;
    }
    return 1 << exponent;
}

inline unsigned int round_down_to_power_of_two(unsigned int n)
{
    int exponent = 0;
    while (n > 1)
    {
        ++exponent;
        n >>= 1;
    }
    return 1 << exponent;
}

inline int intlog2(int x)
{
    int exp = -1;
    while (x)
    {
        x >>= 1;
        ++exp;
    }
    return exp;
}

template <class T>
inline void get_barycentric(T x, int& i, T& f, int i_low, int i_high)
{
    T s = std::floor(x);
    i   = (int)s;
    if (i < i_low)
    {
        i = i_low;
        f = 0;
    }
    else if (i > i_high - 2)
    {
        i = i_high - 2;
        f = 1;
    }
    else
        f = (T)(x - s);
}

template <class S, class T>
inline S lerp(const S& value0, const S& value1, T f)
{
    return (1 - f) * value0 + f * value1;
}

template <class S, class T>
inline S bilerp(const S& v00, const S& v10, const S& v01, const S& v11, T fx, T fy)
{
    return lerp(lerp(v00, v10, fx), lerp(v01, v11, fx), fy);
}

template <class S, class T>
inline S trilerp(const S& v000, const S& v100, const S& v010, const S& v110, const S& v001, const S& v101, const S& v011,
                 const S& v111, T fx, T fy, T fz)
{
    return lerp(bilerp(v000, v100, v010, v110, fx, fy), bilerp(v001, v101, v011, v111, fx, fy), fz);
}

template <class S, class T>
inline S quadlerp(const S& v0000, const S& v1000, const S& v0100, const S& v1100, const S& v0010, const S& v1010,
                  const S& v0110, const S& v1110, const S& v0001, const S& v1001, const S& v0101, const S& v1101,
                  const S& v0011, const S& v1011, const S& v0111, const S& v1111, T fx, T fy, T fz, T ft)
{
    return lerp(trilerp(v0000, v1000, v0100, v1100, v0010, v1010, v0110, v1110, fx, fy, fz),
                trilerp(v0001, v1001, v0101, v1101, v0011, v1011, v0111, v1111, fx, fy, fz), ft);
}

// f should be between 0 and 1, with f=0.5 corresponding to balanced weighting between w0 and w2
template <class T>
inline void quadratic_bspline_weights(T f, T& w0, T& w1, T& w2)
{
    w0 = T(0.5) * sqr(f - 1);
    w1 = T(0.75) - sqr(f - T(0.5));
    w2 = T(0.5) * sqr(f);
}

// f should be between 0 and 1
template <class T>
inline void cubic_interp_weights(T f, T& wneg1, T& w0, T& w1, T& w2)
{
    T f2(f * f), f3(f2 * f);
    wneg1 = -T(1. / 3) * f + T(1. / 2) * f2 - T(1. / 6) * f3;
    w0    = 1 - f2 + T(1. / 2) * (f3 - f);
    w1    = f + T(1. / 2) * (f2 - f3);
    w2    = T(1. / 6) * (f3 - f);
}

template <class S, class T>
inline S cubic_interp(const S& value_neg1, const S& value0, const S& value1, const S& value2, T f)
{
    T wneg1, w0, w1, w2;
    cubic_interp_weights(f, wneg1, w0, w1, w2);
    return wneg1 * value_neg1 + w0 * value0 + w1 * value1 + w2 * value2;
}

template<class T>
inline T map_to_range(T in_range_start, T in_range_end, T out_range_start, T out_range_end, T v)
{
    T slope = 1.0f * (out_range_end - out_range_start) / (in_range_end - in_range_start);
    return out_range_start + slope * (v - in_range_start);
}

template<class T>
inline T catmul_rom_spline(float t, T p0, T p1, T p2, T p3)
{
    return 0.5f *  ((2.0f * p1) +
        (p2 - p0) * t +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * (t * t) +
        (-1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3) * (t * t * t));
}

template<class T>
inline T impulse(T k, T x)
{
    const T h = k * x;
    return h * exp(1.0 - h);
}

template<class T>
inline T cubic_pulse(T c, T w, T x)
{
    x = fabs(x - c);
    if (x > w) return 0.0;
    x /= w;
    return 1.0 - x * x*(3.0 - 2.0*x);
}

template<class T>
inline T exp_step(T x, T k, T n)
{
    return exp(-k * pow(x, n));
}

template<class T>
inline T parabola(T x, T k)
{
    return pow(4.0 * x * (1.0 - x), k);

}

template<class T>
inline T pcurve(T x, T a, T b)
{
    T k = pow(a + b, a + b) / (pow(a, a)*pow(b, b));
    return k * pow(x, a) * pow(1.0 - x, b);
}

template<class T>
inline T smooth_start2(T t, T b = 0.0, T c = 1.0, T d = 1.0)
{
    t /= d;
    return c * t*t + b;
}

template<class T>
inline T smooth_start3(T t, T b = 0.0, T c = 1.0, T d = 1.0)
{
    t /= d;
    return c * t*t*t + b;
}

template<class T>
inline T smooth_start4(T t, T b = 0.0, T c = 1.0, T d = 1.0)
{
    t /= d;
    return c * t*t*t*t + b;
}

template<class T>
inline T smooth_start5(T t, T b = 0.0, T c = 1.0, T d = 1.0)
{
    t /= d;
    return c * t*t*t*t*t + b;
}

template<class T>
inline T smooth_stop2(T t, T b = 0.0, T c = 1.0, T d = 1.0)
{
    t /= d;
    return -c * t*t(t - 2) + b;
}

template<class T>
inline T smooth_stop3(T t, T b = 0.0, T c = 1.0, T d = 1.0)
{
    t = t / d - 1;
    return c * (t*t*t + 1) + b;
}

template<class T>
inline T smooth_stop4(T t, T b = 0.0, T c = 1.0, T d = 1.0)
{
    t = t / d - 1;
    return -c * (t*t*t*t - 1) + b;
}

template<class T>
inline T smooth_stop5(T t, T b = 0.0, T c = 1.0, T d = 1.0)
{
    t = t / d - 1;
    return -c * (t*t*t*t*t + 1) + b;
}