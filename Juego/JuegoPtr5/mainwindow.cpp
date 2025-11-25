#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Juego de Estrategia - Destrucción de Infraestructura");
    resize(1000, 700);

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

    controlador = new ControladorJuego(caja, 0.016, 0.7, 0.5);


    double anchoPilar = 40;
    double altoPilar = 150;
    double anchoTecho = 140;
    double altoTecho = 30;

    double sueloY = altoCaja - 10;

    double yCentroPilar = sueloY - (altoPilar / 2);
    double yCentroTecho = sueloY - altoPilar - (altoTecho / 2);

    double xBaseJ1 = 150;

    controlador->agregarObstaculo(new ObstaculoDestructible(
        1, Vector2D(xBaseJ1, yCentroPilar), anchoPilar, altoPilar, 200.0, 1));

    controlador->agregarObstaculo(new ObstaculoDestructible(
        2, Vector2D(xBaseJ1 + 100, yCentroPilar), anchoPilar, altoPilar, 200.0, 1));

    controlador->agregarObstaculo(new ObstaculoDestructible(
        3, Vector2D(xBaseJ1 + 50, yCentroTecho), anchoTecho, altoTecho, 100.0, 1));


    double xBaseJ2 = 650;

    controlador->agregarObstaculo(new ObstaculoDestructible(
        4, Vector2D(xBaseJ2 - 100, yCentroPilar), anchoPilar, altoPilar, 200.0, 2));

    controlador->agregarObstaculo(new ObstaculoDestructible(
        5, Vector2D(xBaseJ2, yCentroPilar), anchoPilar, altoPilar, 200.0, 2));

    controlador->agregarObstaculo(new ObstaculoDestructible(
        6, Vector2D(xBaseJ2 - 50, yCentroTecho), anchoTecho, altoTecho, 100.0, 2));



    Vector2D posCanon1(50, yCentroTecho);
    Vector2D posCanon2(anchoCaja - 50, yCentroTecho);

    Jugador* jugador1 = new Jugador(1, "Jugador 1", posCanon1, QColor(70, 130, 180));
    Jugador* jugador2 = new Jugador(2, "Jugador 2", posCanon2, QColor(220, 20, 60));

    controlador->agregarJugador(jugador1);
    controlador->agregarJugador(jugador2);

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

    labelTurno = new QLabel("Turno: Jugador 1", this);
    labelTurno->setStyleSheet("font-size: 16px; font-weight: bold;");

    labelInfo = new QLabel("Presiona 'Lanzar' para disparar el proyectil", this);
    labelInfo->setStyleSheet("font-size: 12px;");

    layoutPrincipal->addWidget(labelTurno);
    layoutPrincipal->addWidget(labelInfo);

    vistaJuego = new VistaJuego(controlador, this);
    vistaJuego->inicializarEscena();
    layoutPrincipal->addWidget(vistaJuego);

    timerActualizacion = new QTimer(this);
    connect(timerActualizacion, &QTimer::timeout, this, &MainWindow::actualizarVista);
    timerActualizacion->start(16); // ~60 FPS
}

void MainWindow::actualizarVista()
{
    vistaJuego->actualizar();
}

void MainWindow::onTurnoFinalizado()
{
    Jugador* jugadorActual = controlador->getJugadorActual();
    if (jugadorActual) {
        labelTurno->setText(QString("Turno: %1")
                                .arg(QString::fromStdString(jugadorActual->getNombre())));
    }
}

void MainWindow::onJuegoTerminado(Jugador* ganador)
{
    timerActualizacion->stop();

    if (ganador) {
        QMessageBox::information(this, "¡Juego Terminado!",
                                 QString("¡%1 ha ganado el juego!")
                                     .arg(QString::fromStdString(ganador->getNombre())));
    }
}
