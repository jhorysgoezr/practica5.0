#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <vector>
#include <fstream>
#include <string>
#include "particula.h"
#include "obstaculo.h"
#include "caja.h"

class Simulador
{
private:
    std::vector<Particula> particulas;
    std::vector<Obstaculo> obstaculos;
    Caja caja;
    double dt;
    double e;
    double tiempo;

    void detectarColisionesEntreParticulas(std::ofstream& archivo);

public:
    Simulador(const Caja& caja, double dt, double e);

    void agregarParticula(const Particula& p);
    void agregarObstaculo(const Obstaculo& o);

    void actualizar(std::ofstream& archivo);

    void guardarDatos(std::ofstream& archivo);

    void ejecutarSimulacion(double tiempoTotal, const std::string& nombreArchivo);
};

#endif // SIMULADOR_H
