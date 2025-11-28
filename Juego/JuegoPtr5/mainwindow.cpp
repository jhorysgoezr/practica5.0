#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Juego de Estrategia - Destrucción de Infraestructura");
    resize(1200, 800);

    configurarJuego();
    configurarInterfaz();
}

MainWindow::~MainWindow()
{
    delete controlador;
    delete ui;
}

void MainWindow::configurarJuego()
{
    double anchoCaja = 800.0;
    double altoCaja = 600.0;
    Caja* caja = new Caja(anchoCaja, altoCaja);

    // Parámetros del juego
    double dt = 0.016;  // 60 FPS aproximadamente
    double coefRestitucion = 0.6;  // Colisiones inelásticas (pérdida de energía)
    double factorDanio = 1.0;  // Ajusta este valor según el balance deseado

    controlador = new ControladorJuego(caja, dt, coefRestitucion, factorDanio);

    // Dimensiones de los obstáculos
    double anchoPilar = 40;
    double altoPilar = 150;
    double anchoTecho = 140;
    double altoTecho = 30;

    double sueloY = altoCaja - 10;

    // Posiciones centradas de los obstáculos
    double yCentroPilar = sueloY - (altoPilar / 2);
    double yCentroTecho = sueloY - altoPilar - (altoTecho / 2);

    // Resistencias: Ajustadas para que se destruyan en aproximadamente 3 golpes
    // Con velocidad promedio de 80 m/s, masa 5kg y factorDanio 1.0
    // Daño por golpe ≈ 1.0 * 5 * 80 = 400
    // Para 3 golpes: resistencia ≈ 1200
    double resistenciaPilar = 3000.0;  // ~3 golpes directos
    double resistenciaTecho = 1000.0;   // ~2-3 golpes

    // Infraestructura del Jugador 1 (izquierda)
    double xBaseJ1 = 150;

    controlador->agregarObstaculo(new ObstaculoDestructible(
        1, Vector2D(xBaseJ1, yCentroPilar), anchoPilar, altoPilar, resistenciaPilar, 1));

    controlador->agregarObstaculo(new ObstaculoDestructible(
        2, Vector2D(xBaseJ1 + 100, yCentroPilar), anchoPilar, altoPilar, resistenciaPilar, 1));

    controlador->agregarObstaculo(new ObstaculoDestructible(
        3, Vector2D(xBaseJ1 + 50, yCentroTecho), anchoTecho, altoTecho, resistenciaTecho, 1));

    // Infraestructura del Jugador 2 (derecha)
    double xBaseJ2 = 650;

    controlador->agregarObstaculo(new ObstaculoDestructible(
        4, Vector2D(xBaseJ2 - 100, yCentroPilar), anchoPilar, altoPilar, resistenciaPilar, 2));

    controlador->agregarObstaculo(new ObstaculoDestructible(
        5, Vector2D(xBaseJ2, yCentroPilar), anchoPilar, altoPilar, resistenciaPilar, 2));

    controlador->agregarObstaculo(new ObstaculoDestructible(
        6, Vector2D(xBaseJ2 - 50, yCentroTecho), anchoTecho, altoTecho, resistenciaTecho, 2));

    // Posiciones de los cañones
    Vector2D posCanon1(30, 280);
    Vector2D posCanon2(anchoCaja - 30, 280);

    Jugador* jugador1 = new Jugador(1, "Jugador 1", posCanon1, QColor(70, 130, 180));
    Jugador* jugador2 = new Jugador(2, "Jugador 2", posCanon2, QColor(220, 20, 60));

    controlador->agregarJugador(jugador1);
    controlador->agregarJugador(jugador2);

    // Conectar señales del controlador
    connect(controlador, &ControladorJuego::turnoFinalizado,
            this, &MainWindow::onTurnoFinalizado);
    connect(controlador, &ControladorJuego::juegoTerminado,
            this, &MainWindow::onJuegoTerminado);
}

void MainWindow::configurarInterfaz()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layoutPrincipal = new QVBoxLayout(centralWidget);

    // ===== Panel de Control =====
    QGroupBox* panelControl = new QGroupBox("Panel de Control", this);
    QVBoxLayout* layoutPanel = new QVBoxLayout(panelControl);

    // Etiqueta de turno
    labelTurno = new QLabel("Turno: Jugador 1", this);
    labelTurno->setStyleSheet("font-size: 18px; font-weight: bold; color: #2E86AB;");
    labelTurno->setAlignment(Qt::AlignCenter);
    layoutPanel->addWidget(labelTurno);

    // Controles de lanzamiento
    QHBoxLayout* layoutControles = new QHBoxLayout();

    QLabel* labelAngulo = new QLabel("Ángulo (°):", this);
    labelAngulo->setStyleSheet("font-size: 14px;");

    anguloInput = new QLineEdit(this);
    anguloInput->setPlaceholderText("0 - 180");
    anguloInput->setFixedWidth(100);
    anguloInput->setStyleSheet("font-size: 14px; padding: 5px;");

    QLabel* labelVelocidad = new QLabel("Velocidad (m/s):", this);
    labelVelocidad->setStyleSheet("font-size: 14px;");

    velocidadInput = new QLineEdit(this);
    velocidadInput->setPlaceholderText("10 - 200");
    velocidadInput->setFixedWidth(100);
    velocidadInput->setStyleSheet("font-size: 14px; padding: 5px;");

    lanzarButton = new QPushButton("Lanzar Proyectil", this);
    lanzarButton->setStyleSheet(
        "font-size: 14px; font-weight: bold; padding: 8px 15px; "
        "background-color: #4CAF50; color: white; border-radius: 5px;"
        );

    layoutControles->addWidget(labelAngulo);
    layoutControles->addWidget(anguloInput);
    layoutControles->addSpacing(20);
    layoutControles->addWidget(labelVelocidad);
    layoutControles->addWidget(velocidadInput);
    layoutControles->addSpacing(20);
    layoutControles->addWidget(lanzarButton);
    layoutControles->addStretch();

    layoutPanel->addLayout(layoutControles);

    // Etiqueta de información
    labelInfo = new QLabel("Ingresa el ángulo y velocidad, luego presiona 'Lanzar'", this);
    labelInfo->setStyleSheet("font-size: 13px; color: #555; font-style: italic;");
    labelInfo->setAlignment(Qt::AlignCenter);
    layoutPanel->addWidget(labelInfo);

    // Instrucciones
    QLabel* labelInstrucciones = new QLabel(
        "📋 Instrucciones: Destruye toda la infraestructura del rival para ganar. "
        "Las colisiones con paredes son elásticas, con obstáculos son inelásticas. "
        "Cada obstáculo requiere aproximadamente 3 golpes para ser destruido.",
        this
        );
    labelInstrucciones->setStyleSheet("font-size: 11px; color: #777;");
    labelInstrucciones->setWordWrap(true);
    layoutPanel->addWidget(labelInstrucciones);

    // Información de turno
    labelTiempoTurno = new QLabel("", this);
    labelTiempoTurno->setStyleSheet("font-size: 12px; color: #999; font-style: italic;");
    labelTiempoTurno->setAlignment(Qt::AlignCenter);
    layoutPanel->addWidget(labelTiempoTurno);

    layoutPrincipal->addWidget(panelControl);

    // ===== Vista del Juego =====
    vistaJuego = new VistaJuego(controlador, this);
    vistaJuego->inicializarEscena();
    layoutPrincipal->addWidget(vistaJuego, 1); // Factor de estiramiento 1

    // ===== Conectar Señales =====
    connect(lanzarButton, &QPushButton::clicked, this, &MainWindow::on_lanzarButton_clicked);

    // Timer para actualizar la vista
    timerActualizacion = new QTimer(this);
    connect(timerActualizacion, &QTimer::timeout, this, &MainWindow::actualizarVista);
    timerActualizacion->start(16); // ~60 FPS
}

void MainWindow::actualizarVista()
{
    vistaJuego->actualizar();

    // Actualizar información de tiempo si hay un proyectil activo
    if (controlador->getProyectilActivo() && controlador->getProyectilActivo()->estaActivo()) {
        // Mostrar que el turno está en progreso
        labelTiempoTurno->setText("🚀 Proyectil en movimiento... Observa la trayectoria");
    } else {
        labelTiempoTurno->setText("");
    }
}

void MainWindow::onTurnoFinalizado()
{
    // Re-habilitar el botón de lanzar
    lanzarButton->setEnabled(true);
    lanzarButton->setStyleSheet(
        "font-size: 14px; font-weight: bold; padding: 8px 15px; "
        "background-color: #4CAF50; color: white; border-radius: 5px;"
        );

    Jugador* jugadorActual = controlador->getJugadorActual();
    if (jugadorActual) {
        QString nombreJugador = QString::fromStdString(jugadorActual->getNombre());
        labelTurno->setText(QString("Turno: %1").arg(nombreJugador));

        // Cambiar color según el jugador
        if (jugadorActual->getId() == 1) {
            labelTurno->setStyleSheet("font-size: 18px; font-weight: bold; color: #2E86AB;");
        } else {
            labelTurno->setStyleSheet("font-size: 18px; font-weight: bold; color: #DC143C;");
        }

        labelInfo->setText("Ingresa los parámetros y presiona 'Lanzar'");
        labelInfo->setStyleSheet("font-size: 13px; color: #555; font-style: italic;");
    }

    // Limpiar los campos de entrada
    anguloInput->clear();
    velocidadInput->clear();
    anguloInput->setFocus();
}

void MainWindow::onJuegoTerminado(Jugador* ganador)
{
    timerActualizacion->stop();
    lanzarButton->setEnabled(false);

    if (ganador) {
        QString nombreGanador = QString::fromStdString(ganador->getNombre());

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Juego Terminado");
        msgBox.setText(QString("Felicidades 1").arg(nombreGanador));
        msgBox.setInformativeText("Has destruido toda la infraestructura del rival y ganado el juego.");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        labelTurno->setText(QString("Ganador: %1").arg(nombreGanador));
        labelInfo->setText("El juego ha terminado. Cierra la aplicación para salir.");
    }
}

void MainWindow::on_lanzarButton_clicked()
{
    // 1. Verificar si hay un proyectil activo
    if (controlador->getProyectilActivo() && controlador->getProyectilActivo()->estaActivo()) {
        labelInfo->setText("⚠️ Espera a que el proyectil actual termine su trayectoria!");
        labelInfo->setStyleSheet("font-size: 13px; color: #D32F2F; font-weight: bold;");
        return;
    }

    // 2. Leer y validar las entradas
    bool anguloOk, velocidadOk;
    double angulo = anguloInput->text().toDouble(&anguloOk);
    double velocidad = velocidadInput->text().toDouble(&velocidadOk);

    if (!anguloOk || !velocidadOk) {
        labelInfo->setText("Error: Debes ingresar valores numéricos válidos.");
        labelInfo->setStyleSheet("font-size: 13px; color: #D32F2F; font-weight: bold;");
        return;
    }

    if (velocidad <= 0.0 || velocidad > 300.0) {
        labelInfo->setText("Error: La velocidad debe estar entre 1 y 300 m/s.");
        labelInfo->setStyleSheet("font-size: 13px; color: #D32F2F; font-weight: bold;");
        return;
    }

    if (angulo < 0.0 || angulo > 180.0) {
        labelInfo->setText(" Error: El ángulo debe estar entre 0° y 180°.");
        labelInfo->setStyleSheet("font-size: 13px; color: #D32F2F; font-weight: bold;");
        return;
    }

    // 3. Llamar al método de lanzamiento del controlador
    controlador->lanzarProyectil(angulo, velocidad);

    // 4. Actualizar la interfaz
    lanzarButton->setEnabled(false);
    lanzarButton->setStyleSheet(
        "font-size: 14px; font-weight: bold; padding: 8px 15px; "
        "background-color: #999; color: white; border-radius: 5px;"
        );

    labelInfo->setText(QString("Lanzado: Ángulo= f°, Velocidad= m/s")
                           .arg(angulo).arg(velocidad));
    labelInfo->setStyleSheet("font-size: 13px; color: #1976D2; font-weight: bold;");
}
