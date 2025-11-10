#include <iostream>
#include <iomanip>
#include "vector2d.h"
#include "particula.h"

using namespace std;

void testVector2D() {
    cout << "=== TESTING Vector2D ===" << endl;

    // Test constructores
    Vector2D v2(3.0, 4.0);
    Vector2D v3(-1.0, 2.0);

    cout << "v2: (" << v2.x << ", " << v2.y << ")" << endl;
    cout << "v3: (" << v3.x << ", " << v3.y << ")" << endl;

    // Test magnitud
    cout << "\nMagnitud v2: " << v2.magnitud() << " (esperado: 5.0)" << endl;

    // Test normalizar
    Vector2D v2_norm = v2.normalizar();
    cout << "v2 normalizado: (" << v2_norm.x << ", " << v2_norm.y << ")" << endl;
    cout << "Magnitud del normalizado: " << v2_norm.magnitud() << " (esperado: 1.0)" << endl;

    // Test producto punto
    double producto = v2.productoPunto(v3);
    cout << "v2 · v3: " << producto << " (esperado: 5.0)" << endl;

    // Test operadores aritméticos
    Vector2D suma = v2 + v3;
    Vector2D resta = v2 - v3;
    Vector2D mult = v2 * 2.0;
    Vector2D div = v2 / 2.0;

    cout << "\nv2 + v3: (" << suma.x << ", " << suma.y << ")" << endl;
    cout << "v2 - v3: (" << resta.x << ", " << resta.y << ")" << endl;
    cout << "v2 * 2: (" << mult.x << ", " << mult.y << ")" << endl;
    cout << "v2 / 2: (" << div.x << ", " << div.y << ")" << endl;

    // Test operadores compuestos
    Vector2D v4(1.0, 1.0);
    v4 += v2;
    cout << "\nv4 += v2: (" << v4.x << ", " << v4.y << ")" << endl;

    // Test distancia
    double dist = v2.distancia(v3);
    cout << "Distancia v2-v3: " << dist << endl;

    cout << "=== FIN TEST Vector2D ===\n" << endl;
}

void testParticula() {
    cout << "=== TESTING Particula ===" << endl;

    // Crear partículas
    Particula p1(1, 1.0, 2.0, Vector2D(0.0, 0.0), Vector2D(1.0, 1.0));
    Particula p2(2, 1.0, 1.0, Vector2D(3.0, 0.0), Vector2D(-1.0, 0.0));
    Particula p3(3, 0.5, 0.5, Vector2D(10.0, 10.0), Vector2D(0.0, 0.0));

    // Test getters
    cout << "Particula 1 - ID: " << p1.getId()
         << ", Radio: " << p1.getRadio()
         << ", Masa: " << p1.getMasa() << endl;

    cout << "Posicion p1: (" << p1.getPosicion().x << ", " << p1.getPosicion().y << ")" << endl;
    cout << "Velocidad p1: (" << p1.getVelocidad().x << ", " << p1.getVelocidad().y << ")" << endl;

    // Test movimiento
    cout << "\nAntes de mover - Posicion p1: (" << p1.getPosicion().x << ", " << p1.getPosicion().y << ")" << endl;
    p1.mover(1.0); // Mover 1 segundo
    cout << "Despues de mover (dt=1.0) - Posicion p1: (" << p1.getPosicion().x << ", " << p1.getPosicion().y << ")" << endl;

    // Test colisión con pared
    double anchoCaja = 10.0, altoCaja = 10.0;
    Particula p4(4, 1.0, 1.0, Vector2D(9.5, 5.0), Vector2D(1.0, 0.0));

    cout << "\nTest colision con pared:" << endl;
    cout << "p4 en (9.5, 5.0) - Colisiona con pared? " << p4.colisionaConPared(anchoCaja, altoCaja) << endl;

    p4.resolverColisionConPared(anchoCaja, altoCaja);
    cout << "Velocidad despues de colision: (" << p4.getVelocidad().x << ", " << p4.getVelocidad().y << ")" << endl;

    // Test colisión entre partículas
    Particula p5(5, 1.0, 1.0, Vector2D(2.0, 0.0), Vector2D(0.0, 0.0));
    Particula p6(6, 1.0, 1.0, Vector2D(3.5, 0.0), Vector2D(0.0, 0.0));

    cout << "\nTest colision entre particulas:" << endl;
    cout << "Distancia entre centros: " << p5.getPosicion().distancia(p6.getPosicion()) << endl;
    cout << "Suma de radios: " << (p5.getRadio() + p6.getRadio()) << endl;
    cout << "Colisionan? " << p5.colisionacon(p6) << endl;

    // Test fusión
    cout << "\nTest fusion:" << endl;
    Particula fusionada = p5.fusionarcon(p6);
    cout << "Particula fusionada - ID: " << fusionada.getId()
         << ", Radio: " << fusionada.getRadio()
         << ", Masa: " << fusionada.getMasa() << endl;
    cout << "Posicion fusionada: (" << fusionada.getPosicion().x << ", " << fusionada.getPosicion().y << ")" << endl;

    cout << "=== FIN TEST Particula ===\n" << endl;
}

void testSimulacionSimple() {
    cout << "=== SIMULACION SIMPLE ===" << endl;

    // Crear algunas partículas
    Particula particulas[] = {
        Particula(1, 0.5, 1.0, Vector2D(1.0, 1.0), Vector2D(2.0, 1.0)),
        Particula(2, 0.5, 1.0, Vector2D(8.0, 8.0), Vector2D(-1.0, -1.0)),
        Particula(3, 0.3, 0.5, Vector2D(5.0, 5.0), Vector2D(0.5, -0.5))
    };

    double anchoCaja = 10.0;
    double altoCaja = 10.0;
    double dt = 0.1;
    int pasos = 5;

    for (int paso = 0; paso < pasos; paso++) {
        cout << "\n--- Paso " << paso << " ---" << endl;

        for (int i = 0; i < 3; i++) {
            if (particulas[i].estaActiva()) {
                cout << "Particula " << particulas[i].getId()
                << " - Pos: (" << fixed << setprecision(2)
                << particulas[i].getPosicion().x << ", "
                << particulas[i].getPosicion().y << ")"
                << " - Vel: (" << particulas[i].getVelocidad().x << ", "
                << particulas[i].getVelocidad().y << ")" << endl;

                // Mover partícula
                particulas[i].mover(dt);

                // Verificar colisiones con pared
                if (particulas[i].colisionaConPared(anchoCaja, altoCaja)) {
                    cout << "  -> Colision con pared!" << endl;
                    particulas[i].resolverColisionConPared(anchoCaja, altoCaja);
                }
            }
        }
    }

    cout << "=== FIN SIMULACION ===\n" << endl;
}

int main() {
    cout << "SISTEMA DE PRUEBAS PARA PARTICULAS 2D\n" << endl;

    // Ejecutar tests
    testVector2D();
    testParticula();
    testSimulacionSimple();

    cout << "Todos los tests completados!" << endl;

    return 0;
}
