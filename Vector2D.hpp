#include <iostream>

class Vector2D
{
public:
    float x, y;
    
    Vector2D();
    Vector2D(float x, float y);
    ~Vector2D();

    float norm();
    double dot(const Vector2D &other) const;
    Vector2D project(const Vector2D &other) const;

    Vector2D &Add(const Vector2D &vec);
    Vector2D &Substract(const Vector2D &vec);
    Vector2D &Multiply(const float &i);
    Vector2D &Divide(const float &i);

    friend Vector2D &operator+(Vector2D &v1, const Vector2D &v2);
    friend Vector2D &operator-(Vector2D &v1, const Vector2D &v2);
    friend Vector2D &operator*(Vector2D &v1, const float &i);
    friend Vector2D &operator/(Vector2D &v1, const float &i);

    Vector2D &operator+=(const Vector2D &v1);
    Vector2D &operator-=(const Vector2D &v1);
    Vector2D &operator*=(const int &i);
    Vector2D &operator/=(const int &i);
    Vector2D &operator=(const Vector2D &v1);

    Vector2D &Zero();
    Vector2D &Normalize();
    

    friend std::ostream &operator<<(std::ostream &stream, const Vector2D &vec);
};