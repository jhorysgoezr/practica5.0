#include "particula.h"

Particula::Particula(int id, double radio, double masa, const Vector2D &pos, const Vector2D &vel):
    id(id), radio(radio), masa(masa), posicion(pos), velocidad(vel), activa(true)
{

}

int Particula::getId() const
{
    return id;
}

double Particula::getRadio() const
{
    return radio;
}

double Particula::getMasa() const
{
    return masa;
}

Vector2D Particula::getPosicion() const
{
    return posicion;
}

Vector2D Particula::getVelocidad() const
{
    return velocidad;
}

void Particula::setPosicion(const Vector2D& pos)
{
    posicion = pos;
}

void Particula::setVelocidad(const Vector2D& vel)
{
    velocidad = vel;
}

void Particula::desactivar()
{
    activa = false;
}

void Particula::mover(double dt)
{
    if (!activa) return;
    posicion += velocidad * dt;
}

bool Particula::colisionaConPared(double anchoCaja, double altoCaja) const
{
    return{
    posicion.x - radio <= 0 || posicion.x + radio >= anchoCaja ||
        posicion.y - radio <= 0 || posicion.y + radio >= altoCaja
    };
}

void Particula::resolverColisionConPared(double anchoCaja, double altoCaja)
{
    if (posicion.x - radio <= 0 || posicion.x + radio >= anchoCaja){
        velocidad.x = -velocidad.x;
    }
    if (posicion.y - radio <= 0 || posicion.y + radio >= altoCaja){
        velocidad.y = -velocidad.y;
    }
}




