#include "proyectil.h"
#include "obstaculodestructible.h"
#include <cmath>

Proyectil::Proyectil(int id, double radio, double masa,
                     const Vector2D& pos, const Vector2D& vel, int idJugador)
    : id(id), radio(radio), masa(masa),
    posicion(pos), velocidad(vel), activo(true), idJugador(idJugador)
{}

// Getters
int Proyectil::getId() const { return id; }
double Proyectil::getRadio() const { return radio; }
double Proyectil::getMasa() const { return masa; }
Vector2D Proyectil::getPosicion() const { return posicion; }
Vector2D Proyectil::getVelocidad() const { return velocidad; }
bool Proyectil::estaActivo() const { return activo; }
int Proyectil::getIdJugador() const { return idJugador; }

// Setters
void Proyectil::setPosicion(const Vector2D& pos) { posicion = pos; }
void Proyectil::setVelocidad(const Vector2D& vel) { velocidad = vel; }
void Proyectil::desactivar() { activo = false; }

void Proyectil::mover(double dt)
{
    if (activo) {
        posicion += velocidad * dt;
    }
}

bool Proyectil::colisionaConPared(double ancho, double alto) const
{
    return (posicion.x - radio <= 0.0) ||
           (posicion.x + radio >= ancho) ||
           (posicion.y - radio <= 0.0) ||
           (posicion.y + radio >= alto);
}

void Proyectil::resolverColisionConPared(double ancho, double alto)
{
    if (posicion.x - radio <= 0.0) {
        posicion.x = radio;
        velocidad.x = -velocidad.x;
    } else if (posicion.x + radio >= ancho) {
        posicion.x = ancho - radio;
        velocidad.x = -velocidad.x;
    }

    if (posicion.y - radio <= 0.0) {
        posicion.y = radio;
        velocidad.y = -velocidad.y;
    } else if (posicion.y + radio >= alto) {
        posicion.y = alto - radio;
        velocidad.y = -velocidad.y;
    }
}

bool Proyectil::colisionaConObstaculo(const ObstaculoDestructible& obs) const
{
    return obs.colisionaCon(*this);
}

void Proyectil::resolverColisionObstaculo(const ObstaculoDestructible& obs, double e)
{
    Vector2D normal = obs.calcularNormalImpacto(*this).normalizar();

    double vN = velocidad.productoPunto(normal);
    Vector2D vPerp = normal * vN;
    Vector2D vPar = velocidad - vPerp;

    Vector2D nuevaVel = vPar - vPerp * e;
    velocidad = nuevaVel;

    posicion += normal * 0.5;
}

double Proyectil::calcularMomentoImpacto() const
{
    return masa * velocidad.magnitud();
}
