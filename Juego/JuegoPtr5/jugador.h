#ifndef JUGADOR_H
#define JUGADOR_H

#include "vector2d.h"
#include "proyectil.h"
#include <string>
#include <QColor>

class Jugador
{
private:
    int id;
    std::string nombre;
    Vector2D posicionCanion;
    bool vivo;
    QColor color;

public:
    Jugador(int id, const std::string& nombre,
            const Vector2D& posCanion, const QColor& color);

    int getId() const;
    std::string getNombre() const;
    Vector2D getPosicionCanion() const;
    bool estaVivo() const;
    QColor getColor() const;

    void eliminar();

    Proyectil crearProyectil(double angulo, double velocidad,
                             double masa, double radio) const;
};

#endif // JUGADOR_H
