#include "simulador.h"
#include <iostream>

int main() {
    Caja caja(200, 100);

    Simulador sim(caja, 0.1, 0.7);

    sim.agregarObstaculo(Obstaculo(1, Vector2D(30, 30), 15));
    sim.agregarObstaculo(Obstaculo(2, Vector2D(170, 30), 15));
    sim.agregarObstaculo(Obstaculo(3, Vector2D(30, 70), 15));
    sim.agregarObstaculo(Obstaculo(4, Vector2D(170, 70), 15));

    sim.agregarParticula(Particula(1, 5, 2, Vector2D(100, 10), Vector2D(0, 4)));
    sim.agregarParticula(Particula(2, 5, 2, Vector2D(60, 50), Vector2D(-3, -2)));
    sim.agregarParticula(Particula(3, 5, 2, Vector2D(80, 80), Vector2D(2, -2)));
    sim.agregarParticula(Particula(4, 5, 2, Vector2D(90, 70), Vector2D(0, 0)));
    sim.agregarParticula(Particula(5, 5, 2, Vector2D(150, 50), Vector2D(5, 3)));

    sim.ejecutarSimulacion(15.0, "resultados.txt");

    return 0;
}
