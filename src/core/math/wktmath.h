#ifndef _WKT_MATH_H
#define _WKT_MATH_H

#include <math.h>
#include <ostream>

namespace wkt {
namespace math
{

class vec2
{
public:
    float x, y;

    inline vec2& operator+=(const vec2&);
    inline vec2& operator-=(const vec2&);
    inline vec2& operator*=(float s);

    float lengthSquared() const { return this->x * this->x + this->y * this->y; }
    float length() const { return sqrtf(lengthSquared()); }

    bool operator==(const vec2& v) const
    {
        return this->x == v.x && this->y == v.y;
    }

    bool operator!=(const vec2& v) const { return !(*this == v); }
};

inline vec2& vec2::operator+=(const vec2& v)
{
    this->x += v.x;
    this->y += v.y;
    return *this;
}

inline vec2& vec2::operator-=(const vec2& v)
{
    this->x -= v.x;
    this->y -= v.y;
    return *this;
}

inline vec2& vec2::operator*=(float s)
{
    this->x *= s;
    this->y *= s;
    return *this;
}

inline vec2 operator+(const vec2& a, const vec2& b)
{
    return vec2(a) += b;
}

inline vec2 operator-(const vec2& a, const vec2& b)
{
    return vec2(a) -= b;
}

inline vec2 operator*(const vec2& v, float s)
{
    return vec2(v) *= s;
}

inline vec2 operator*(float s, const vec2& v)
{
    return v * s;
}

inline float operator*(const vec2& a, const vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

class mat2
{
public:
    mat2() = default;
    mat2(const vec2 col1, const vec2& col2) : col1(col1), col2(col2) { }
    mat2(float i11, float i21, float i12, float i22) 
    {
        this->col1 = { i11, i21 };
        this->col2 = { i12, i22 };
    }

    const vec2& firstColumn() const { return this->col1; }
    const vec2& secondColumn() const { return this->col2; }
    vec2 firstRow() const { return { col1.x, col2.x }; }
    vec2 secondRow() const { return { col1.y, col2.y }; }

    inline float& operator()(int i, int j);
    const float& operator()(int i, int j) const { return (*this)(i, j); }
 
    inline mat2& operator+=(const mat2& other);
    inline mat2& operator-=(const mat2& other);
    inline mat2& operator*=(float x);
    inline mat2 operator*(const mat2& other);

    vec2 transform(const vec2& v)
    {
        vec2 res;
        res.x = col1.x * v.x + col2.x * v.y;
        res.y = col1.y * v.x + col2.y * v.y;

        return res;
    }

private:
    vec2 col1;
    vec2 col2;
};

inline float& mat2::operator()(int i, int j)
{
    return j == 1 ? 
        (i == 1 ? col1.x : col1.y)
        : (i == 1) ? col2.x : col2.y;
}

inline mat2& mat2::operator+=(const mat2& other)
{
    this->col1 += other.col1;
    this->col2 += other.col2;

    return *this;
}

inline mat2& mat2::operator-=(const mat2& other)
{
    this->col1 -= other.col1;
    this->col2 -= other.col2;

    return *this;
}

inline mat2& mat2::operator*=(float x)
{
    this->col1 *= x;
    this->col2 *= x;

    return *this;
}

inline mat2 operator+(const mat2& a, const mat2& b)
{
    mat2 c = a;
    c += b;
    return c;
}

inline mat2 operator-(const mat2& a, const mat2& b)
{
    mat2 c = a;
    c -= b;
    return c;
}

inline mat2 operator*(const mat2& a, float x)
{
    mat2 c = a;
    c *= x;
    return c;
}

inline mat2 operator*(float x, const mat2& a)
{
    return a * x;
}

inline mat2 mat2::operator*(const mat2& m)
{
    mat2 res;
    auto row1 = firstRow();
    auto row2 = secondRow();

    res.col1.x = row1 * m.firstColumn();
    res.col2.x = row1 * m.secondColumn();
    res.col1.y = row2 * m.firstColumn();
    res.col2.y = row2 * m.secondColumn();

    return res;
}


struct Size
{
    Size() = default;
    Size(float width, float height) : width(width), height(height) {}
    explicit Size(const vec2 &v) : width(v.x), height(v.y) {}
    explicit operator vec2() const { return vec2{this->width, this->height}; }

    float width = 0;
    float height = 0;
};

inline Size operator*(const Size& a, const Size& b)
{
    return Size(std::max(a.width, b.width), std::max(a.height, b.height));
}

inline Size operator/(const Size& a, const Size& b)
{
    return Size(std::min(a.width, b.width), std::min(a.height, b.height));
}

inline bool operator<(const Size& a, const Size& b)
{
    return a.width * a.height < b.width * b.height;
}

inline bool operator==(const Size& a, const Size& b)
{
    return a.width == b.width && a.height == b.height;
}

inline bool operator!=(const Size& a, const Size& b)
{
    return !(a == b);
}

struct Rect
{
    Rect() = default;
    Rect(const vec2& origin, const Size& size)
        : origin(origin), size(size) {}
    Rect(float x, float y, float width, float height) : origin{x, y}, size(width, height) {}

    vec2 origin;
    Size size;

    inline bool intersect(const Rect& rect);
    inline bool intersect(const vec2& v);
};

inline bool Rect::intersect(const Rect& rect)
{
    const vec2& upperLeft = rect.origin;
    vec2 upperRight{rect.origin.x + rect.size.width, rect.origin.y};
    vec2 lowerLeft{rect.origin.x, rect.origin.y + rect.size.height};
    vec2 lowerRight{rect.origin.x + rect.size.width, rect.origin.y + rect.size.height};

    return intersect(upperLeft) || intersect(upperRight) || intersect(lowerLeft) || intersect(lowerRight);
}

inline bool Rect::intersect(const vec2& v)
{
    return this->origin.x <= v.x && v.x <= this->origin.x + this->size.width 
            && this->origin.y <= v.y && v.y <= this->origin.y + this->size.height;
}

inline bool operator==(const Rect& a, const Rect& b)
{
    return a.origin == b.origin && a.size == b.size;
}

inline bool operator!=(const Rect& a, const Rect& b)
{
    return !(a == b);
}

}}

#endif