#include "jugador.h"
#include <cmath>

Jugador::Jugador(int id, const std::string& nombre,
                 const Vector2D& posCanion, const QColor& color)
    : id(id), nombre(nombre), posicionCanion(posCanion),
    vivo(true), color(color)
{}

int Jugador::getId() const { return id; }
std::string Jugador::getNombre() const { return nombre; }
Vector2D Jugador::getPosicionCanion() const { return posicionCanion; }
bool Jugador::estaVivo() const { return vivo; }
QColor Jugador::getColor() const { return color; }

void Jugador::eliminar()
{
    vivo = false;
}

Proyectil Jugador::crearProyectil(double angulo, double velocidad,
                                  double masa, double radio) const
{
    double anguloRad = angulo * M_PI / 180.0;

    double vx = velocidad * std::cos(anguloRad);
    double vy = -velocidad * std::sin(anguloRad);

    if (id == 2) {
        vx = -vx;
    }

    Vector2D velocidadInicial(vx, vy);

    static int contadorProyectiles = 0;
    return Proyectil(contadorProyectiles++, radio, masa,
                     posicionCanion, velocidadInicial, id);
}
