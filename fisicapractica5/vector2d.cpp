#include "vector2d.h"
#include <cmath>

Vector2D::Vector2D() : x(0.0), y(0.0) {}

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

double Vector2D::magnitud() const
{
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::normalizar() const
{
    double mag = magnitud();
    if (mag == 0.0) return Vector2D(0.0, 0.0);
    return Vector2D(x / mag, y / mag);
}

double Vector2D::productoPunto(const Vector2D &otro) const
{
    return x * otro.x + y * otro.y;
}

Vector2D Vector2D::operator+(const Vector2D &otro) const
{
    return Vector2D(x + otro.x, y + otro.y);
}

Vector2D Vector2D::operator-(const Vector2D &otro) const
{
    return Vector2D(x - otro.x, y - otro.y);
}

Vector2D Vector2D::operator*(double escalar) const
{
    return Vector2D(x * escalar, y * escalar);
}

Vector2D Vector2D::operator/(double escalar) const
{
    return Vector2D(x / escalar, y / escalar);
}

Vector2D &Vector2D::operator+=(const Vector2D &otro)
{
    x += otro.x;
    y += otro.y;
    return *this;
}

Vector2D &Vector2D::operator-=(const Vector2D &otro)
{
    x -= otro.x;
    y -= otro.y;
    return *this;
}

double Vector2D::distancia(const Vector2D &otro) const
{
    double dx = x - otro.x;
    double dy = y - otro.y;
    return std::sqrt(dx * dx + dy * dy);
}
