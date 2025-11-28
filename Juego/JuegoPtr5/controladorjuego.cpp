#include "controladorjuego.h"
#include <algorithm>

ControladorJuego::ControladorJuego(Caja* caja, double dt, double e, double factorDanio)
    : caja(caja), dt(dt), coeficienteRestitucion(e), factorDanio(factorDanio),
    jugador1(nullptr), jugador2(nullptr), jugadorActual(nullptr),
    proyectilActivo(nullptr), timer(nullptr), tiempoMaximoTurno(7.0), tiempoTranscurrido(0.0)
{
    gravedad = Vector2D(0.0, 98.0);

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

    obstaculosImpactados.clear();

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
    if (proyectilActivo && proyectilActivo->estaActivo()) {
        tiempoTranscurrido += dt;

        if (tiempoTranscurrido >= tiempoMaximoTurno) {
            proyectilActivo->desactivar();
        }

        proyectilActivo->aplicarGravedad(dt, gravedad);

        proyectilActivo->mover(dt);

        procesarColisiones();

        emit actualizarVista();

    } else {
        timer->stop();

        tiempoTranscurrido = 0.0;

        if (verificarVictoria()) {
            emit juegoTerminado(getJugadorGanador());
            return;
        }

        cambiarTurno();

        delete proyectilActivo;
        proyectilActivo = nullptr;
    }
}

void ControladorJuego::procesarColisiones()
{
    if (!proyectilActivo || !proyectilActivo->estaActivo()) {
        return;
    }

    // 1. Colisión Elástica con Paredes de la Caja
    if (proyectilActivo->colisionaConPared(caja->getAncho(), caja->getAlto())) {
        proyectilActivo->resolverColisionConPared(caja->getAncho(), caja->getAlto());
    }

    // 2. Colisión Inelástica y Daño con Obstáculos Destructibles
    for (auto it = obstaculos.begin(); it != obstaculos.end(); ) {
        ObstaculoDestructible* obs = *it;

        // Colisión solo si es infraestructura del jugador rival
        if (obs->getIdJugadorPropietario() != proyectilActivo->getIdJugador() &&
            proyectilActivo->colisionaConObstaculo(*obs))
        {
            // Verificar si ya impactó este obstáculo en este turno
            int idObs = obs->getId();
            bool yaImpactado = false;
            for (int id : obstaculosImpactados) {
                if (id == idObs) {
                    yaImpactado = true;
                    break;
                }
            }

            // Solo aplicar daño si NO ha impactado antes este obstáculo
            if (!yaImpactado) {
                // A. Cálculo de Daño y Aplicación
                double danio = calcularDanio(proyectilActivo);
                obs->recibirDanio(danio);

                // Registrar que ya impactó este obstáculo
                obstaculosImpactados.push_back(idObs);

                // B. Si el obstáculo está destruido, eliminarlo
                if (obs->estaDestruido()) {
                    emit obstaculoDestruido(obs->getId());
                    delete obs;
                    it = obstaculos.erase(it);

                    // Verificar victoria después de destruir un obstáculo
                    if (verificarVictoria()) {
                        proyectilActivo->desactivar();
                        return;
                    }
                } else {
                    ++it;
                }
            } else {
                ++it;
            }

            // C. Colisión Inelástica con coeficiente de restitución
            proyectilActivo->resolverColisionObstaculo(*obs, coeficienteRestitucion);

            return;
        } else {
            ++it;
        }
    }

    // 3. Condición de Detención (velocidad muy baja y cerca del suelo)
    if (proyectilActivo->getVelocidad().magnitud() < 1.0 &&
        proyectilActivo->getPosicion().y >= caja->getAlto() - proyectilActivo->getRadio() - 5.0)
    {
        proyectilActivo->desactivar();
    }

    // 4. Desactivar si sale de los límites (seguridad adicional)
    Vector2D pos = proyectilActivo->getPosicion();
    if (pos.x < -50 || pos.x > caja->getAncho() + 50 ||
        pos.y < -50 || pos.y > caja->getAlto() + 50) {
        proyectilActivo->desactivar();
    }
}

void ControladorJuego::cambiarTurno()
{
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

    // Un jugador gana cuando destruye toda la infraestructura del rival
    if (obstaculosJ2 == 0 && jugador1) {
        jugador2->eliminar();
        return true;
    }
    if (obstaculosJ1 == 0 && jugador2) {
        jugador1->eliminar();
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
    // Velocidad de impacto (magnitud del vector velocidad)
    double V_impacto = p->getVelocidad().magnitud();
    double Masa_proyectil = p->getMasa();

    // Daño = factor_constante × masa × velocidad
    double danio = factorDanio * Masa_proyectil * V_impacto;

    return danio;
}
