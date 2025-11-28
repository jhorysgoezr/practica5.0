#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "vector2d.h"

class ObstaculoDestructible;

class Proyectil
{
private:
    int id;
    double radio;
    double masa;
    Vector2D posicion;
    Vector2D velocidad;
    bool activo;
    int idJugador;

public:
    Proyectil(int id, double radio, double masa,
              const Vector2D& pos, const Vector2D& vel, int idJugador);

    int getId() const;
    double getRadio() const;
    double getMasa() const;
    Vector2D getPosicion() const;
    Vector2D getVelocidad() const;
    bool estaActivo() const;
    int getIdJugador() const;

    void setPosicion(const Vector2D& pos);
    void setVelocidad(const Vector2D& vel);
    void desactivar();

    void mover(double dt);

    bool colisionaConPared(double ancho, double alto) const;
    void resolverColisionConPared(double ancho, double alto);

    bool colisionaConObstaculo(const ObstaculoDestructible& obs) const;
    void resolverColisionObstaculo(const ObstaculoDestructible& obs, double e);

    double calcularMomentoImpacto() const;

    void aplicarGravedad(double dt, const Vector2D& gravedad);
};

#endif // PROYECTIL_H
