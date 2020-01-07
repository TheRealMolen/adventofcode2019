#pragma once

#include <ostream>


template<typename T>
T gcd(T a, T b)
{
    while (b != 0)
    {
        T t = b;
        b = a % b;
        a = t;
    }
    return a;
}


template<typename T>
T gcd(T a, T b, T c)
{
    return max(gcd(a, b), max(gcd(b, c), gcd(c, a)));
}

template<typename T>
T lcm(T a, T b)
{
    return (a * b) / gcd(a, b);
}



template<typename T>
struct pt2d
{
    T x, y;

    pt2d() {/**/ }
    pt2d(T x, T y) : x(x), y(y) {/**/ }
    pt2d(initializer_list<T> l) {
        auto it = l.begin();
        x = *(it++);
        y = *it;
    }

    bool operator==(const pt2d<T>& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const pt2d<T>& rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }
    bool operator<(const pt2d<T>& rhs) const
    {
        if (x < rhs.x)
            return true;
        if (x > rhs.x)
            return false;
        if (y < rhs.y)
            return true;
        return false;
    }
    pt2d<T>& operator+=(const pt2d<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    pt2d<T> operator+(const pt2d<T>& rhs) const
    {
        return pt2d<T>(x + rhs.x, y + rhs.y);
    }
    pt2d<T> operator-(const pt2d<T>& rhs) const
    {
        return pt2d<T>(x - rhs.x, y - rhs.y);
    }

    void simplify()
    {
        T div = (T)abs(gcd(x, y));
        x /= div;
        y /= div;
    }
};
template<typename T>
ostream& operator<<(ostream& os, const pt2d<T>& p)
{
    os << '<' << p.x << ", " << p.x << '>';
    return os;
}


struct d12_pt
{
    int x, y, z;

    d12_pt(const initializer_list<int>& l)
    {
        auto it = l.begin();
        x = *(it++);
        y = *(it++);
        z = *it;
    }
    d12_pt(int x, int y, int z) : x(x), y(y), z(z) {/**/ }

    d12_pt& operator+=(const d12_pt& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};