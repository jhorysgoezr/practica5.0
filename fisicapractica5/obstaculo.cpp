#include "obstaculo.h"
#include "particula.h"
#include <algorithm>
#include <cmath>

Obstaculo::Obstaculo(int id, const Vector2D &pos, double lado)
    : id(id), posicion(pos), lado(lado) {}

int Obstaculo::getId() const
{
    return id;
}

Vector2D Obstaculo::getPosicion() const
{
    return posicion;
}

double Obstaculo::getLado() const
{
    return lado;
}

// Tratamos el obstáculo como un cuadrado alineado a los ejes.
bool Obstaculo::colisionaCon(const Particula &p) const
{
    Vector2D centro = p.getPosicion();
    double r = p.getRadio();
    double half = lado / 2.0;

    double xMin = posicion.x - half;
    double xMax = posicion.x + half;
    double yMin = posicion.y - half;
    double yMax = posicion.y + half;

    double closestX = std::max(xMin, std::min(centro.x, xMax));
    double closestY = std::max(yMin, std::min(centro.y, yMax));

    double dx = centro.x - closestX;
    double dy = centro.y - closestY;

    return (dx * dx + dy * dy) <= (r * r);
}

Vector2D Obstaculo::calcularNormalImpacto(const Particula &p) const
{
    Vector2D centro = p.getPosicion();
    double half = lado / 2.0;

    double xMin = posicion.x - half;
    double xMax = posicion.x + half;
    double yMin = posicion.y - half;
    double yMax = posicion.y + half;

    double distIzq = std::abs(centro.x - xMin);
    double distDer = std::abs(centro.x - xMax);
    double distSup = std::abs(centro.y - yMin);
    double distInf = std::abs(centro.y - yMax);

    double minDist = std::min(std::min(distIzq, distDer),
                              std::min(distSup, distInf));

    if (minDist == distIzq)  return Vector2D(-1.0, 0.0); // lado izquierdo
    if (minDist == distDer)  return Vector2D( 1.0, 0.0); // lado derecho
    if (minDist == distSup)  return Vector2D( 0.0,-1.0); // lado superior
    return Vector2D(0.0, 1.0);                           // lado inferior
}
