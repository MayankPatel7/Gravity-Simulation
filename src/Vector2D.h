#pragma once
#include <math.h>

struct Vector2D
{
    float x;
    float y;
    
    Vector2D() : x(0.0f), y(0.0f) {}
    Vector2D(float x, float y) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& other){
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other){
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator*(const float scalar){
        return Vector2D(x*scalar, y*scalar); 
    }

    Vector2D operator/(const float scalar){
        return Vector2D(x/scalar, y/scalar); 
    }

    void operator+=(const Vector2D& other){
        x += other.x;
        y += other.y;
    }

    void operator-=(const Vector2D& other){
        x -= other.x;
        y -= other.y;
    }

    float mag(){
        return sqrt(x*x+y*y);
    }

    Vector2D normalize(){
        float m = mag();
        if(m == 0) return Vector2D(0, 0);
        return *this / m;
    }
};