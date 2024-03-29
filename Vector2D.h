#pragma once

//------------------------------------------------------------------------
//  Vector2d - Author: Mat Buckland
//------------------------------------------------------------------------
#include <iostream>
#include <windows.h>
#include <limits>

const double  MinDouble = (std::numeric_limits<double>::min)();
//compares two real numbers. Returns true if they are equal
inline bool isEqual(float a, float b)
{
    if (fabs(a - b) < 1E-12)
        return true;

    return false;
}

inline bool isEqual(double a, double b)
{
    if (fabs(a - b) < 1E-12)
        return true;

    return false;
}

struct Vector2D
{
    double x;
    double y;

    Vector2D() :x(0.0), y(0.0) {}
    Vector2D(double a, double b) :x(a), y(b) {}

    //sets x and y to zero
    void Zero() { x = 0.0; y = 0.0; }

    //returns true if both x and y are zero
    bool isZero()const { return (x * x + y * y) < MinDouble; }

    //returns the length of the vector
    inline double    Length()const;

    //returns the squared length of the vector (thereby avoiding the sqrt)
    inline double    LengthSq()const;

    inline void      Normalize();

    inline double    Dot(const Vector2D& v2)const;

    //returns positive if v2 is clockwise of this vector,
    //negative if anticlockwise (assuming the Y axis is pointing down,
    //X axis to right like a Window app)
    inline int       Sign(const Vector2D& v2)const;

    //returns the vector that is perpendicular to this one.
    inline Vector2D  Perp()const;

    //adjusts x and y so that the length of the vector does not exceed max
    inline void      Truncate(double max);

    //returns the distance between this vector and the one passed as a parameter
    inline double    Distance(const Vector2D& v2)const;

    //squared version of above.
    inline double    DistanceSq(const Vector2D& v2)const;

    inline void      Reflect(const Vector2D& norm);

    //returns the vector that is the reverse of this vector
    inline Vector2D  GetReverse()const;

    //we need some overloaded operators
    const Vector2D& operator+=(const Vector2D& rhs)
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    const Vector2D& operator-=(const Vector2D& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    const Vector2D& operator*(const Vector2D& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;

        return *this;
    }

    const Vector2D& operator*=(const Vector2D& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;

        return *this;
    }

    const Vector2D& operator*=(const double& rhs)
    {
        x *= rhs;
        y *= rhs;

        return *this;
    }

    const Vector2D& operator/=(const double& rhs)
    {
        x /= rhs;
        y /= rhs;

        return *this;
    }

    bool operator==(const Vector2D& rhs)const
    {
        return (isEqual(x, rhs.x) && isEqual(y, rhs.y));
    }

    bool operator!=(const Vector2D& rhs)const
    {
        return (x != rhs.x) || (y != rhs.y);
    }

};

//-----------------------------------------------------------------------some more operator overloads
inline Vector2D operator*(const Vector2D& lhs, double rhs);
inline Vector2D operator*(double lhs, const Vector2D& rhs);
inline Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs);
inline Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs);
inline Vector2D operator/(const Vector2D& lhs, double val);
std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs);


//------------------------------------------------------------------------member functions

//------------------------- Length ---------------------------------------
//
//  returns the length of a 2D vector
//------------------------------------------------------------------------
inline double Vector2D::Length()const
{
    return sqrt(x * x + y * y);
}


//------------------------- LengthSq -------------------------------------
//
//  returns the squared length of a 2D vector
//------------------------------------------------------------------------
inline double Vector2D::LengthSq()const
{
    return (x * x + y * y);
}


//------------------------- Vec2DDot -------------------------------------
//
//  calculates the dot product
//------------------------------------------------------------------------
inline double Vector2D::Dot(const Vector2D& v2)const
{
    return x * v2.x + y * v2.y;
}

//------------------------ Sign ------------------------------------------
//
//  returns positive if v2 is clockwise of this vector,
//  minus if anticlockwise (Y axis pointing down, X axis to right)
//------------------------------------------------------------------------
enum { clockwise = 1, anticlockwise = -1 };

inline int Vector2D::Sign(const Vector2D& v2)const
{
    if (y * v2.x > x * v2.y)
    {
        return clockwise;
    }
    else
    {
        return anticlockwise;
    }
}

//------------------------------ Perp ------------------------------------
//
//  Returns a vector perpendicular to this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::Perp()const
{
    return Vector2D(-y, x);
}

//------------------------------ Distance --------------------------------
//
//  calculates the euclidean distance between two vectors
//------------------------------------------------------------------------
inline double Vector2D::Distance(const Vector2D& v2)const
{
    double ySeparation = v2.y - y;
    double xSeparation = v2.x - x;

    return sqrt(ySeparation * ySeparation + xSeparation * xSeparation);
}


//------------------------------ DistanceSq ------------------------------
//
//  calculates the euclidean distance squared between two vectors 
//------------------------------------------------------------------------
inline double Vector2D::DistanceSq(const Vector2D& v2)const
{
    double ySeparation = v2.y - y;
    double xSeparation = v2.x - x;

    return ySeparation * ySeparation + xSeparation * xSeparation;
}

//----------------------------- Truncate ---------------------------------
//
//  truncates a vector so that its length does not exceed max
//------------------------------------------------------------------------
inline void Vector2D::Truncate(double max)
{
    if (this->Length() > max)
    {
        this->Normalize();

        *this *= max;
    }
}

//--------------------------- Reflect ------------------------------------
//
//  given a normalized vector this method reflects the vector it
//  is operating upon. (like the path of a ball bouncing off a wall)
//------------------------------------------------------------------------
inline void Vector2D::Reflect(const Vector2D& norm)
{
    *this += 2.0 * this->Dot(norm) * norm.GetReverse();
}

//----------------------- GetReverse ----------------------------------------
//
//  returns the vector that is the reverse of this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::GetReverse()const
{
    return Vector2D(-this->x, -this->y);
}


//------------------------- Normalize ------------------------------------
//
//  normalizes a 2D Vector
//------------------------------------------------------------------------
inline void Vector2D::Normalize()
{
    double vector_length = this->Length();

    if (vector_length > std::numeric_limits<double>::epsilon())
    {
        this->x /= vector_length;
        this->y /= vector_length;
    }
}


//------------------------------------------------------------------------non member functions

inline Vector2D Vec2DNormalize(const Vector2D& v)
{
    Vector2D vec = v;

    double vector_length = vec.Length();

    if (vector_length > std::numeric_limits<double>::epsilon())
    {
        vec.x /= vector_length;
        vec.y /= vector_length;
    }

    return vec;
}

inline double Vec2DDistance(const Vector2D& v1, const Vector2D& v2)
{

    double ySeparation = v2.y - v1.y;
    double xSeparation = v2.x - v1.x;

    return sqrt(ySeparation * ySeparation + xSeparation * xSeparation);
}

inline double Vec2DDistanceSq(const Vector2D& v1, const Vector2D& v2)
{

    double ySeparation = v2.y - v1.y;
    double xSeparation = v2.x - v1.x;

    return ySeparation * ySeparation + xSeparation * xSeparation;
}

inline Vector2D& Vec2DMin(Vector2D& v1, Vector2D& v2)
{

    if (v1.LengthSq() < v2.LengthSq())
    {
        return v1;
    }
    else
    {
        return v2;
    }
}

inline Vector2D& Vec2DMax(Vector2D& v1, Vector2D& v2)
{

    if (v1.LengthSq() > v2.LengthSq())
    {
        return v1;
    }
    else
    {
        return v2;
    }
}

inline double Vec2DLength(const Vector2D& v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

inline double Vec2DLengthSq(const Vector2D& v)
{
    return (v.x * v.x + v.y * v.y);
}

inline Vector2D Vec2DfromAngle(const double theta)
{
    Vector2D d = Vector2D(1, 0);
    if (theta != 0)							
    {
        d.x = std::cos(theta) - std::sin(theta);
        d.y = std::cos(theta) + std::sin(theta);
    }
    return d;
}

inline Vector2D POINTStoVector(const POINTS& p)
{
    return Vector2D(p.x, p.y);
}


inline Vector2D POINTtoVector(const POINT& p)
{
    return Vector2D((double)p.x, (double)p.y);
}

inline POINTS VectorToPOINTS(const Vector2D& v)
{
    POINTS p;
    p.x = (short)v.x;
    p.y = (short)v.y;

    return p;
}

inline POINT VectorToPOINT(const Vector2D& v)
{
    POINT p;
    p.x = (long)v.x;
    p.y = (long)v.y;

    return p;
}



//------------------------------------------------------------------------operator overloads
inline Vector2D operator*(const Vector2D& lhs, double rhs)
{
    Vector2D result(lhs);
    result *= rhs;
    return result;
}

inline Vector2D operator*(double lhs, const Vector2D& rhs)
{
    Vector2D result(rhs);
    result *= lhs;
    return result;
}

//overload the - operator
inline Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs)
{
    Vector2D result(lhs);
    result.x -= rhs.x;
    result.y -= rhs.y;

    return result;
}

//overload the + operator
inline Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs)
{
    Vector2D result(lhs);
    result.x += rhs.x;
    result.y += rhs.y;

    return result;
}

//overload the / operator
inline Vector2D operator/(const Vector2D& lhs, double val)
{
    Vector2D result(lhs);
    result.x /= val;
    result.y /= val;

    return result;
}