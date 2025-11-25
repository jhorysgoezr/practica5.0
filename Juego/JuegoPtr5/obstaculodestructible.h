#ifndef OBSTACULODESTRUCTIBLE_H
#define OBSTACULODESTRUCTIBLE_H

#include "vector2d.h"

class Proyectil;

class ObstaculoDestructible
{
private:
    int id;
    Vector2D posicion;
    double ancho;
    double alto;
    double resistenciaActual;
    double resistenciaMaxima;
    int idJugadorPropietario;
public:
    ObstaculoDestructible(int id, const Vector2D& pos,
                          double ancho, double alto,
                          double resistencia, int idJugador);

    int getId() const;
    Vector2D getPosicion() const;
    double getAncho() const;
    double getAlto() const;
    double getResistenciaActual() const;
    double getResistenciaMaxima() const;
    int getIdJugadorPropietario() const;

    bool colisionaCon(const Proyectil& p) const;
    Vector2D calcularNormalImpacto(const Proyectil& p) const;

    void recibirDanio(double danio);
    bool estaDestruido() const;
    double getPorcentajeVida() const;
};

#endif // OBSTACULODESTRUCTIBLE_H
