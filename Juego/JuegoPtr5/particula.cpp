#include "particula.h"
#include "obstaculo.h"
#include <cmath>

Particula::Particula(int id, double radio, double masa,
                     const Vector2D &pos, const Vector2D &vel)
    : id(id), radio(radio), masa(masa),
    posicion(pos), velocidad(vel), activa(true)
{}

int Particula::getId() const            { return id; }
double Particula::getRadio() const      { return radio; }
double Particula::getMasa() const       { return masa; }
Vector2D Particula::getPosicion() const { return posicion; }
Vector2D Particula::getVelocidad() const{ return velocidad; }
bool Particula::estaActiva() const      { return activa; }

void Particula::setPosicion(const Vector2D &pos) { posicion = pos; }
void Particula::setVelocidad(const Vector2D &vel){ velocidad = vel; }
void Particula::desactivar()                     { activa = false; }

void Particula::mover(double dt)
{
    posicion += velocidad * dt;
}

bool Particula::colisionaConPared(double anchoCaja, double altoCaja) const
{
    return (posicion.x - radio <= 0.0) ||
           (posicion.x + radio >= anchoCaja) ||
           (posicion.y - radio <= 0.0) ||
           (posicion.y + radio >= altoCaja);
}

void Particula::resolverColisionConPared(double anchoCaja, double altoCaja)
{
    if (posicion.x - radio <= 0.0) {
        posicion.x = radio;
        velocidad.x = -velocidad.x;
    } else if (posicion.x + radio >= anchoCaja) {
        posicion.x = anchoCaja - radio;
        velocidad.x = -velocidad.x;
    }

    if (posicion.y - radio <= 0.0) {
        posicion.y = radio;
        velocidad.y = -velocidad.y;
    } else if (posicion.y + radio >= altoCaja) {
        posicion.y = altoCaja - radio;
        velocidad.y = -velocidad.y;
    }
}

bool Particula::colisionacon(const Particula &otra) const
{
    double dist = posicion.distancia(otra.posicion);
    return dist <= (radio + otra.radio);
}

Particula Particula::fusionarcon(const Particula &otra) const
{
    double nuevaMasa = masa + otra.masa;

    Vector2D nuevaVelocidad =
        (velocidad * masa + otra.velocidad * otra.masa) / nuevaMasa;

    Vector2D nuevaPosicion =
        (posicion * masa + otra.posicion * otra.masa) / nuevaMasa;

    double nuevoRadio = std::sqrt(radio * radio + otra.radio * otra.radio);

    return Particula(id, nuevoRadio, nuevaMasa, nuevaPosicion, nuevaVelocidad);
}

bool Particula::colisionaConObstaculo(const Obstaculo &obs) const
{
    return obs.colisionaCon(*this);
}

void Particula::resolverColisionObstaculo(const Obstaculo &obs, double e)
{
    Vector2D normal = obs.calcularNormalImpacto(*this).normalizar();

    double vN = velocidad.productoPunto(normal);
    Vector2D vPerp = normal * vN;
    Vector2D vPar  = velocidad - vPerp;

    Vector2D nuevaVel = vPar - vPerp * e;
    velocidad = nuevaVel;

    posicion += normal * 0.1;
}




