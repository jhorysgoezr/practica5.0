#include "obstaculo.h"
#include <cmath>

using namespace std;

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

bool Obstaculo::colisionaCon(const Particula &p) const
{
    Vector2D centro = p.getPosicion();
    double radio = p.getRadio();

    double xIzq = posicion.x;
    double xDer = posicion.x + lado;
    double ySup = posicion.y;
    double yInf = posicion.y + lado;

    double xCercano = max(xIzq, min(centro.x, xDer));
    double yCercano = max(ySup, min(centro.y, yInf));

    double dx = centro.x - xCercano;
    double dy = centro.y - yCercano;
    double distancia = sqrt(dx * dx + dy * dy);

    return distancia <= radio;
}

Vector2D Obstaculo::calcularNormalImpacto(const Particula &p) const
{
    Vector2D centro = p.getPosicion();

    double xIzq = posicion.x;
    double xDer = posicion.x + lado;
    double ySup = posicion.y;
    double yInf = posicion.y + lado;

    double distIzq = abs(centro.x - xIzq);
    double distDer = abs(centro.x - xDer);
    double distSup = abs(centro.y - ySup);
    double distInf = abs(centro.y - yInf);

    double minDist = min(min(distIzq, distDer), min(distSup, distInf));

    if (minDist == distIzq) return Vector2D(-1, 0);  // lado izquierdo
    if (minDist == distDer) return Vector2D(1, 0);   // lado derecho
    if (minDist == distSup) return Vector2D(0, -1);  // lado superior
    return Vector2D(0, 1);


}

Obstaculo::Obstaculo(int id, const Vector2D &pos, double lado):id(id), posicion(pos), lado(lado){}
