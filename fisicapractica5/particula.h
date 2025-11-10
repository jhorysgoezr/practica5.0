#ifndef PARTICULA_H
#define PARTICULA_H

#include <cmath>

#include "vector2d.h"

class Particula
{
private:
    int id;
    double radio;
    double masa;
    Vector2D posicion;
    Vector2D velocidad;
    bool activa;

public:
    Particula(int id, double radio, double masa, const Vector2D& pos, const Vector2D& vel);

    int getId() const;
    double getRadio() const;
    double getMasa() const;
    Vector2D getPosicion() const;
    Vector2D getVelocidad() const;
    bool estaActiva()const;

    void setPosicion(const Vector2D& pos);
    void setVelocidad(const Vector2D& vel);
    void desactivar();

    void mover(double dt);

    bool colisionaConPared(double anchoCaja, double altoCaja)const;
    void resolverColisionConPared(double anchoCaja, double altoCaja);

    bool colisionacon(const Particula& otra) const;
    Particula fusionarcon (const Particula& otra) const;

};

#endif // PARTICULA_H
