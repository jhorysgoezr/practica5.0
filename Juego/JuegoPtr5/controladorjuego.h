#ifndef CONTROLADORJUEGO_H
#define CONTROLADORJUEGO_H

#include "jugador.h"
#include "proyectil.h"
#include "obstaculodestructible.h"
#include "caja.h"
#include <vector>
#include <QTimer>
#include <QObject>

class ControladorJuego : public QObject
{
    Q_OBJECT

private:
    Jugador* jugador1;
    Jugador* jugador2;
    Jugador* jugadorActual;
    Proyectil* proyectilActivo;
    std::vector<ObstaculoDestructible*> obstaculos;
    Caja* caja;

    double coeficienteRestitucion;
    double factorDanio;
    double dt;
    QTimer* timer;

    double calcularDanio(Proyectil* p);
    void procesarColisiones();

public:
    ControladorJuego(Caja* caja, double dt = 0.016,
                     double e = 0.7, double factorDanio = 0.5);
    ~ControladorJuego();

    void agregarJugador(Jugador* j);
    void agregarObstaculo(ObstaculoDestructible* obs);

    void lanzarProyectil(double angulo, double velocidad);
    void actualizar();
    void cambiarTurno();
    bool verificarVictoria();

    Jugador* getJugadorGanador();
    Jugador* getJugadorActual() const;
    Proyectil* getProyectilActivo() const;
    std::vector<ObstaculoDestructible*> getObstaculos() const;
    Caja* getCaja() const;
    Jugador* getJugador1() const;
    Jugador* getJugador2() const;

signals:
    void turnoFinalizado();
    void juegoTerminado(Jugador* ganador);
    void obstaculoDestruido(int idObstaculo);
};

#endif // CONTROLADORJUEGO_H
