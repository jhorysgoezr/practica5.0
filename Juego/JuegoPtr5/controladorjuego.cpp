#include "controladorjuego.h"
#include <algorithm>

ControladorJuego::ControladorJuego(Caja* caja, double dt, double e, double factorDanio)
    : caja(caja), dt(dt), coeficienteRestitucion(e), factorDanio(factorDanio),
    jugador1(nullptr), jugador2(nullptr), jugadorActual(nullptr),
    proyectilActivo(nullptr), timer(nullptr)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ControladorJuego::actualizar);
}

ControladorJuego::~ControladorJuego()
{
    delete jugador1;
    delete jugador2;
    delete proyectilActivo;
    for (auto obs : obstaculos) {
        delete obs;
    }
    delete caja;
    delete timer;
}

void ControladorJuego::agregarJugador(Jugador* j)
{
    if (!jugador1) {
        jugador1 = j;
        jugadorActual = jugador1;
    } else if (!jugador2) {
        jugador2 = j;
    }
}

void ControladorJuego::agregarObstaculo(ObstaculoDestructible* obs)
{
    obstaculos.push_back(obs);
}

void ControladorJuego::lanzarProyectil(double angulo, double velocidad)
{
    if (proyectilActivo && proyectilActivo->estaActivo()) {
        return;
    }

    double masa = 5.0;
    double radio = 5.0;

    delete proyectilActivo;
    proyectilActivo = new Proyectil(
        jugadorActual->crearProyectil(angulo, velocidad, masa, radio)
        );

    timer->start(static_cast<int>(dt * 1000));
}

void ControladorJuego::actualizar()
{
    if (!proyectilActivo || !proyectilActivo->estaActivo()) {
        timer->stop();
        cambiarTurno();
        return;
    }

    proyectilActivo->mover(dt);

    procesarColisiones();

    if (proyectilActivo->getVelocidad().magnitud() < 0.5) {
        proyectilActivo->desactivar();
    }
}

void ControladorJuego::procesarColisiones()
{
    if (!proyectilActivo || !proyectilActivo->estaActivo()) return;

    if (proyectilActivo->colisionaConPared(caja->getAncho(), caja->getAlto())) {
        proyectilActivo->resolverColisionConPared(caja->getAncho(), caja->getAlto());
    }

    for (auto it = obstaculos.begin(); it != obstaculos.end(); ) {
        ObstaculoDestructible* obs = *it;

        if (obs->getIdJugadorPropietario() != proyectilActivo->getIdJugador()) {
            if (proyectilActivo->colisionaConObstaculo(*obs)) {
                double danio = calcularDanio(proyectilActivo);
                obs->recibirDanio(danio);

                proyectilActivo->resolverColisionObstaculo(*obs, coeficienteRestitucion);

                if (obs->estaDestruido()) {
                    emit obstaculoDestruido(obs->getId());
                    delete obs;
                    it = obstaculos.erase(it);
                    continue;
                }
            }
        }
        ++it;
    }
}

void ControladorJuego::cambiarTurno()
{
    if (verificarVictoria()) {
        emit juegoTerminado(getJugadorGanador());
        return;
    }

    if (jugadorActual == jugador1) {
        jugadorActual = jugador2;
    } else {
        jugadorActual = jugador1;
    }

    emit turnoFinalizado();
}

bool ControladorJuego::verificarVictoria()
{
    // Contar obstáculos de cada jugador
    int obstaculosJ1 = 0;
    int obstaculosJ2 = 0;

    for (const auto& obs : obstaculos) {
        if (obs->getIdJugadorPropietario() == jugador1->getId()) {
            obstaculosJ1++;
        } else if (obs->getIdJugadorPropietario() == jugador2->getId()) {
            obstaculosJ2++;
        }
    }

    if (obstaculosJ2 == 0 && jugador1) {
        jugador1->eliminar();
        return true;
    }
    if (obstaculosJ1 == 0 && jugador2) {
        jugador2->eliminar();
        return true;
    }

    return false;
}

Jugador* ControladorJuego::getJugadorGanador()
{
    if (jugador1 && !jugador1->estaVivo()) {
        return jugador2;
    }
    if (jugador2 && !jugador2->estaVivo()) {
        return jugador1;
    }
    return nullptr;
}

Jugador* ControladorJuego::getJugadorActual() const { return jugadorActual; }
Proyectil* ControladorJuego::getProyectilActivo() const { return proyectilActivo; }
std::vector<ObstaculoDestructible*> ControladorJuego::getObstaculos() const { return obstaculos; }
Caja* ControladorJuego::getCaja() const { return caja; }
Jugador* ControladorJuego::getJugador1() const { return jugador1; }
Jugador* ControladorJuego::getJugador2() const { return jugador2; }

double ControladorJuego::calcularDanio(Proyectil* p)
{
    double momentoImpacto = p->calcularMomentoImpacto();
    return factorDanio * momentoImpacto;
}
