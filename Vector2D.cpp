#include "Vector2D.hpp"
#include <cmath>

Vector2D::Vector2D() { x = y = 0.0f; }
Vector2D::~Vector2D(){}

float Vector2D::norm()
{
    return sqrt(pow(x, 2) + pow(y, 2));
}
double Vector2D::dot(const Vector2D &other) const
{
    return x*other.x + y*other.y;
};

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2D &Vector2D::Add(const Vector2D &vec)
{
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}
Vector2D &Vector2D::Substract(const Vector2D &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Vector2D &Vector2D::Multiply(const float &i)
{
    this->x *= i;
    this->y *= i;
    return *this;
}

Vector2D &Vector2D::Divide(const float &i)
{
    this->x /= i;
    this->y /= i;
    return *this;
}

Vector2D &Vector2D::operator=(const Vector2D &v1)
{
    this->x = v1.x;
    this->y = v1.y;
    return *this;
}

Vector2D &operator+(Vector2D &v1, const Vector2D &v2)
{
    return v1.Add(v2);
}
Vector2D &operator-(Vector2D &v1, const Vector2D &v2)
{
    return v1.Substract(v2);
}
Vector2D &operator*(Vector2D &v1, const float &i)
{
    return v1.Multiply(i);
}
Vector2D &operator/(Vector2D &v1, const float &i)
{
    return v1.Divide(i);
}

Vector2D &Vector2D::operator+=(const Vector2D &v1)
{
    return this->Add(v1);
}
Vector2D &Vector2D::operator-=(const Vector2D &v1)
{
    return this->Substract(v1);
}
Vector2D &Vector2D::operator*=(const int &i)
{
    return this->Multiply(i);
}
Vector2D &Vector2D::operator/=(const int &i)
{
    return this->Divide(i);
}

Vector2D &Vector2D::Zero()
{
    this->x = 0;
    this->y = 0;
    return *this;
}

Vector2D &Vector2D::Normalize()
{
    return this->Divide(this->norm());
}

std::ostream &operator<<(std::ostream &stream, const Vector2D &vec)
{
    stream << "(" << (float)vec.x << "," << (float)vec.y << ")";
    return stream;
}