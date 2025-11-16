#ifndef PARTICULA_H
#define PARTICULA_H

#include "vector2d.h"

class Obstaculo;

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
    Particula(int id, double radio, double masa,
              const Vector2D& pos, const Vector2D& vel);

    // Getters / setters básicos
    int getId() const;
    double getRadio() const;
    double getMasa() const;
    Vector2D getPosicion() const;
    Vector2D getVelocidad() const;
    bool estaActiva() const;

    void setPosicion(const Vector2D& pos);
    void setVelocidad(const Vector2D& vel);
    void desactivar();

    // Movimiento
    void mover(double dt);

    // Colisión con paredes (perfectamente elástica)
    bool colisionaConPared(double anchoCaja, double altoCaja) const;
    void resolverColisionConPared(double anchoCaja, double altoCaja);

    // Colisión entre partículas (completamente inelástica)
    bool colisionacon(const Particula& otra) const;
    Particula fusionarcon (const Particula& otra) const;

    // Colisión con obstáculo (inelástica con coeficiente e)
    bool colisionaConObstaculo(const Obstaculo& obs) const;
    void resolverColisionObstaculo(const Obstaculo& obs, double e);
};

#endif // PARTICULA_H
