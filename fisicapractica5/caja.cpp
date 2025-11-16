#include "caja.h"

Caja::Caja(double ancho, double alto): ancho(ancho), alto(alto){}

double Caja::getAncho() const
{
    return ancho;
}

double Caja::getAlto() const
{
    return alto;
}

bool Caja::dentroDeLimites(const Particula &p) const
{
    Vector2D pos = p.getPosicion();
    double r = p.getRadio();

    return (pos.x - r >= 0 && pos.x + r <= ancho &&
            pos.y - r >= 0 && pos.y + r <= alto);
}

