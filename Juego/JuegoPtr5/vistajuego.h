#ifndef VISTAJUEGO_H
#define VISTAJUEGO_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include "controladorjuego.h"

class VistaJuego : public QGraphicsView
{
    Q_OBJECT

private:
    QGraphicsScene* escena;
    ControladorJuego* controlador;

    QGraphicsRectItem* rectCaja;
    QList<QGraphicsRectItem*> rectObstaculos;
    QList<QGraphicsTextItem*> textosVida;
    QGraphicsEllipseItem* canon1;
    QGraphicsEllipseItem* canon2;
    QGraphicsEllipseItem* proyectilGrafico;

    double escala;

    void dibujarCaja();
    void dibujarObstaculos();
    void dibujarCanones();
    void actualizarProyectil();
    void actualizarObstaculos();

public:
    VistaJuego(ControladorJuego* ctrl, QWidget* parent = nullptr);
    ~VistaJuego();
    void inicializarEscena();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void actualizar();
    void onObstaculoDestruido(int idObstaculo);
};

#endif // VISTAJUEGO_H
