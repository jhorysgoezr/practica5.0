#include "obstaculodestructible.h"
#include "proyectil.h"
#include <algorithm>
#include <cmath>

ObstaculoDestructible::ObstaculoDestructible(int id, const Vector2D& pos,
                                             double ancho, double alto,
                                             double resistencia, int idJugador)
    : id(id), posicion(pos), ancho(ancho), alto(alto),
    resistenciaActual(resistencia), resistenciaMaxima(resistencia),
    idJugadorPropietario(idJugador)
{}

int ObstaculoDestructible::getId() const { return id; }
Vector2D ObstaculoDestructible::getPosicion() const { return posicion; }
double ObstaculoDestructible::getAncho() const { return ancho; }
double ObstaculoDestructible::getAlto() const { return alto; }
double ObstaculoDestructible::getResistenciaActual() const { return resistenciaActual; }
double ObstaculoDestructible::getResistenciaMaxima() const { return resistenciaMaxima; }
int ObstaculoDestructible::getIdJugadorPropietario() const { return idJugadorPropietario; }

bool ObstaculoDestructible::colisionaCon(const Proyectil& p) const
{
    Vector2D centro = p.getPosicion();
    double r = p.getRadio();

    double halfW = ancho / 2.0;
    double halfH = alto / 2.0;

    double xMin = posicion.x - halfW;
    double xMax = posicion.x + halfW;
    double yMin = posicion.y - halfH;
    double yMax = posicion.y + halfH;

    double closestX = std::max(xMin, std::min(centro.x, xMax));
    double closestY = std::max(yMin, std::min(centro.y, yMax));

    double dx = centro.x - closestX;
    double dy = centro.y - closestY;

    return (dx * dx + dy * dy) <= (r * r);
}

Vector2D ObstaculoDestructible::calcularNormalImpacto(const Proyectil& p) const
{
    Vector2D centro = p.getPosicion();

    double halfW = ancho / 2.0;
    double halfH = alto / 2.0;

    double xMin = posicion.x - halfW;
    double xMax = posicion.x + halfW;
    double yMin = posicion.y - halfH;
    double yMax = posicion.y + halfH;

    double distIzq = std::abs(centro.x - xMin);
    double distDer = std::abs(centro.x - xMax);
    double distSup = std::abs(centro.y - yMin);
    double distInf = std::abs(centro.y - yMax);

    double minDist = std::min({distIzq, distDer, distSup, distInf});

    if (minDist == distIzq)  return Vector2D(-1.0, 0.0);
    if (minDist == distDer)  return Vector2D( 1.0, 0.0);
    if (minDist == distSup)  return Vector2D( 0.0,-1.0);
    return Vector2D(0.0, 1.0);
}

void ObstaculoDestructible::recibirDanio(double danio)
{
    resistenciaActual -= danio;
    if (resistenciaActual < 0.0) {
        resistenciaActual = 0.0;
    }
}

bool ObstaculoDestructible::estaDestruido() const
{
    return resistenciaActual <= 0.0;
}

double ObstaculoDestructible::getPorcentajeVida() const
{
    if (resistenciaMaxima <= 0.0) return 0.0;
    return (resistenciaActual / resistenciaMaxima) * 100.0;
}
