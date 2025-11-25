#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include "controladorjuego.h"
#include "vistajuego.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    ControladorJuego* controlador;
    VistaJuego* vistaJuego;
    QTimer* timerActualizacion;

    QLabel* labelTurno;
    QLabel* labelInfo;

    void configurarJuego();
    void configurarInterfaz();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void actualizarVista();
    void onTurnoFinalizado();
    void onJuegoTerminado(Jugador* ganador);
};

#endif // MAINWINDOW_H
