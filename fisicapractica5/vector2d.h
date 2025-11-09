#ifndef VECTOR2D_H
#define VECTOR2D_H

using namespace std;

class Vector2D
{
private:
    double x;
    double y;
public:
    Vector2D();
    Vector2D(double x = 0.0, double y = 0.0);

    double magnitud() const;
    Vector2D normalizar() const;
    double productoPunto(const Vector2D& otro)const;

    Vector2D operator+(const Vector2D& otro) const;
    Vector2D operator-(const Vector2D& otro) const;
    Vector2D operator*(double escalar)const;
    Vector2D operator/(double escalar)const;

    Vector2D& operator+=(const Vector2D& otro);
    Vector2D& operator-=(const Vector2D& otro);

    double distancia(const Vector2D& otro) const;
};

#endif // VECTOR2D_H
