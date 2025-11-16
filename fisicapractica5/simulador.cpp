#include "simulador.h"
#include <iostream>
#include <iomanip>

Simulador::Simulador(const Caja& caja, double dt, double e)
    : caja(caja), dt(dt), e(e), tiempo(0.0) {}

void Simulador::agregarParticula(const Particula& p)
{
    particulas.push_back(p);
}

void Simulador::agregarObstaculo(const Obstaculo& o)
{
    obstaculos.push_back(o);
}

void Simulador::detectarColisionesEntreParticulas(std::ofstream& archivo)
{
    std::vector<Particula> nuevas;

    for (std::size_t i = 0; i < particulas.size(); ++i) {
        if (!particulas[i].estaActiva()) continue;

        for (std::size_t j = i + 1; j < particulas.size(); ++j) {
            if (!particulas[j].estaActiva()) continue;

            if (particulas[i].colisionacon(particulas[j])) {
                Particula fusionada = particulas[i].fusionarcon(particulas[j]);

                archivo << "C PP " << std::fixed << std::setprecision(3)
                        << tiempo << " "
                        << particulas[i].getId() << " "
                        << particulas[j].getId() << "\n";

                particulas[i].desactivar();
                particulas[j].desactivar();

                nuevas.push_back(fusionada);

            }
        }
    }

    for (const auto& p : nuevas) {
        particulas.push_back(p);
    }
}

void Simulador::actualizar(std::ofstream& archivo)
{
    tiempo += dt;

    for (auto& p : particulas) {
        if (!p.estaActiva()) continue;

        p.mover(dt);

        if (p.colisionaConPared(caja.getAncho(), caja.getAlto())) {
            p.resolverColisionConPared(caja.getAncho(), caja.getAlto());

            archivo << "C PW " << std::fixed << std::setprecision(3)
                    << tiempo << " "
                    << p.getId() << "\n";
        }

        for (const auto& obs : obstaculos) {
            if (p.colisionaConObstaculo(obs)) {
                p.resolverColisionObstaculo(obs, e);

                archivo << "C PO " << std::fixed << std::setprecision(3)
                        << tiempo << " "
                        << p.getId() << " "
                        << obs.getId() << "\n";
            }
        }
    }

    detectarColisionesEntreParticulas(archivo);
}

void Simulador::guardarDatos(std::ofstream &archivo)
{
    archivo << std::fixed << std::setprecision(3);
    archivo << "P " << tiempo;

    for (const auto& p : particulas) {
        if (!p.estaActiva()) continue;
        Vector2D pos = p.getPosicion();
        archivo << " " << p.getId()
                << " " << pos.x
                << " " << pos.y;
    }
    archivo << "\n";
}

void Simulador::ejecutarSimulacion(double tiempoTotal, const std::string &nombreArchivo)
{
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo de salida.\n";
        return;
    }

    archivo << "# Archivo de resultados de la simulacion\n";
    archivo << "# Formato:\n";
    archivo << "#   Lineas que comienzan con 'P': posiciones de las particulas\n";
    archivo << "#     P t id1 x1 y1 id2 x2 y2 ...\n";
    archivo << "#   Lineas que comienzan con 'C': colisiones detectadas\n";
    archivo << "#     C PP t id1 id2          (colision particula-particula)\n";
    archivo << "#     C PO t idPart idObs     (colision particula-obstaculo)\n";
    archivo << "#     C PW t idPart           (colision particula-pared)\n";
    archivo << "\n";

    while (tiempo < tiempoTotal) {
        actualizar(archivo);
        guardarDatos(archivo);
    }

    archivo.close();
    std::cout << "Simulacion completada. Resultados guardados en "
              << nombreArchivo << "\n";
}
