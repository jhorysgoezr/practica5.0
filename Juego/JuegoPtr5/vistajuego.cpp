#include "vistajuego.h"
#include <QBrush>
#include <QResizeEvent>
#include <QPen>


VistaJuego::VistaJuego(ControladorJuego* ctrl, QWidget* parent):
    QGraphicsView(parent), controlador(ctrl), proyectilGrafico(nullptr)
{
    escena = new QGraphicsScene(this);
    setScene(escena);

    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    escala = 1.0;

    connect(controlador, &ControladorJuego::obstaculoDestruido,
            this, &VistaJuego::onObstaculoDestruido);
}

VistaJuego::~VistaJuego()
{
    delete escena;
}

void VistaJuego::inicializarEscena()
{
    escena->clear();
    rectObstaculos.clear();

    dibujarCaja();
    dibujarObstaculos();
    dibujarCanones();

}

void VistaJuego::resizeEvent(QResizeEvent *event) {
    if (escena) {
        fitInView(escena->sceneRect(), Qt::KeepAspectRatio);
    }
    QGraphicsView::resizeEvent(event);
}

void VistaJuego::dibujarCaja()
{
    Caja* caja = controlador->getCaja();
    if (!caja) return;

    double ancho = caja->getAncho();
    double alto = caja->getAlto();

    QPen penCaja(Qt::black, 3);
    QBrush brushCaja(QColor(240, 240, 240));

    rectCaja = escena->addRect(0, 0, ancho * escala, alto * escala,
                               penCaja, brushCaja);

    escena->setSceneRect(-10, -10, ancho * escala + 20, alto * escala + 20);
}

void VistaJuego::dibujarObstaculos()
{
    std::vector<ObstaculoDestructible*> obstaculos = controlador->getObstaculos();

    for (auto obs : obstaculos) {
        Vector2D pos = obs->getPosicion();
        double ancho = obs->getAncho();
        double alto = obs->getAlto();
        int idPropietario = obs->getIdJugadorPropietario();

        double x = (pos.x - ancho/2.0) * escala;
        double y = (pos.y - alto/2.0) * escala;

        QColor colorObstaculo;
        if (idPropietario == controlador->getJugador1()->getId()) {
            colorObstaculo = controlador->getJugador1()->getColor();
        } else {
            colorObstaculo = controlador->getJugador2()->getColor();
        }

        QPen penObs(Qt::black, 2);
        QBrush brushObs(colorObstaculo);

        QGraphicsRectItem* rectObs = escena->addRect(
            x, y, ancho * escala, alto * escala,
            penObs, brushObs
            );

        rectObs->setData(0, obs->getId());
        rectObstaculos.append(rectObs);
    }
}

void VistaJuego::dibujarCanones()
{
    Jugador* j1 = controlador->getJugador1();
    Jugador* j2 = controlador->getJugador2();

    if (!j1 || !j2) return;

    Vector2D posCanon1 = j1->getPosicionCanion();
    double radioCanon = 10.0;

    QPen penCanon1(Qt::black, 2);
    QBrush brushCanon1(j1->getColor().darker(150));

    canon1 = escena->addEllipse(
        (posCanon1.x - radioCanon) * escala,
        (posCanon1.y - radioCanon) * escala,
        radioCanon * 2 * escala,
        radioCanon * 2 * escala,
        penCanon1, brushCanon1
        );

    Vector2D posCanon2 = j2->getPosicionCanion();

    QPen penCanon2(Qt::black, 2);
    QBrush brushCanon2(j2->getColor().darker(150));

    canon2 = escena->addEllipse(
        (posCanon2.x - radioCanon) * escala,
        (posCanon2.y - radioCanon) * escala,
        radioCanon * 2 * escala,
        radioCanon * 2 * escala,
        penCanon2, brushCanon2
        );
}

void VistaJuego::actualizar()
{
    actualizarProyectil();

    std::vector<ObstaculoDestructible*> obstaculos = controlador->getObstaculos();

    for (int i = 0; i < rectObstaculos.size(); ++i) {
        if (i < obstaculos.size()) {
            double porcentajeVida = obstaculos[i]->getPorcentajeVida();
            double opacidad = porcentajeVida / 100.0;
            rectObstaculos[i]->setOpacity(opacidad);
        }
    }
}

void VistaJuego::actualizarProyectil()
{
    Proyectil* proy = controlador->getProyectilActivo();

    if (proy && proy->estaActivo()) {
        Vector2D pos = proy->getPosicion();
        double radio = proy->getRadio();

        if (!proyectilGrafico) {
            QPen penProy(Qt::black, 1);
            QBrush brushProy(Qt::darkGray);

            proyectilGrafico = escena->addEllipse(
                0, 0, radio * 2 * escala, radio * 2 * escala,
                penProy, brushProy
                );
        }

        proyectilGrafico->setPos(
            (pos.x - radio) * escala,
            (pos.y - radio) * escala
            );
        proyectilGrafico->setVisible(true);
    } else {
        if (proyectilGrafico) {
            proyectilGrafico->setVisible(false);
        }
    }
}

void VistaJuego::onObstaculoDestruido(int idObstaculo)
{
    for (int i = 0; i < rectObstaculos.size(); ++i) {
        if (rectObstaculos[i]->data(0).toInt() == idObstaculo) {
            escena->removeItem(rectObstaculos[i]);
            delete rectObstaculos[i];
            rectObstaculos.removeAt(i);
            break;
        }
    }
}
