#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "vector2d.h"

class Particula;

class Obstaculo
{
private:
    int id;
    Vector2D posicion;
    double lado;

public:
    Obstaculo(int id, const Vector2D& pos, double lado);

    int getId() const;
    Vector2D getPosicion() const;
    double getLado() const;

    bool colisionaCon(const Particula& p) const;
    Vector2D calcularNormalImpacto(const Particula& p) const;
};

#endif // OBSTACULO_H
