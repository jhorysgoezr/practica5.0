#include "vistajuego.h"
#include <QBrush>
#include <QResizeEvent>
#include <QPen>
#include <QGraphicsTextItem>

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
    textosVida.clear();

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
    QBrush brushCaja(QColor(245, 245, 245));

    rectCaja = escena->addRect(0, 0, ancho * escala, alto * escala,
                               penCaja, brushCaja);

    escena->setSceneRect(-10, -10, ancho * escala + 20, alto * escala + 20);

    // Agregar línea de suelo para referencia visual
    QPen penSuelo(QColor(139, 69, 19), 2, Qt::DashLine);
    escena->addLine(0, (alto - 10) * escala, ancho * escala, (alto - 10) * escala, penSuelo);
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
        rectObs->setRotation(0.785298);

        rectObs->setData(0, obs->getId());
        rectObstaculos.append(rectObs);

        // Agregar texto con porcentaje de vida
        QGraphicsTextItem* textoVida = escena->addText(
            QString("%1%").arg(static_cast<int>(obs->getPorcentajeVida()))
            );
        textoVida->setDefaultTextColor(Qt::white);
        textoVida->setFont(QFont("Arial", 10, QFont::Bold));

        // Centrar el texto en el obstáculo
        QRectF boundsTexto = textoVida->boundingRect();
        textoVida->setPos(
            x + (ancho * escala - boundsTexto.width()) / 2.0,
            y + (alto * escala - boundsTexto.height()) / 2.0
            );

        textoVida->setData(0, obs->getId());
        textosVida.append(textoVida);
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
    QBrush brushCanon1(j1->getColor().darker(120));

    canon1 = escena->addEllipse(
        (posCanon1.x - radioCanon) * escala,
        (posCanon1.y - radioCanon) * escala,
        radioCanon * 2 * escala,
        radioCanon * 2 * escala,
        penCanon1, brushCanon1
        );

    // Agregar etiqueta del jugador 1
    QGraphicsTextItem* labelJ1 = escena->addText("J1");
    labelJ1->setDefaultTextColor(j1->getColor().darker(150));
    labelJ1->setFont(QFont("Arial", 8, QFont::Bold));
    labelJ1->setPos((posCanon1.x - 8) * escala, (posCanon1.y - 25) * escala);

    Vector2D posCanon2 = j2->getPosicionCanion();

    QPen penCanon2(Qt::black, 2);
    QBrush brushCanon2(j2->getColor().darker(120));

    canon2 = escena->addEllipse(
        (posCanon2.x - radioCanon) * escala,
        (posCanon2.y - radioCanon) * escala,
        radioCanon * 2 * escala,
        radioCanon * 2 * escala,
        penCanon2, brushCanon2
        );

    // Agregar etiqueta del jugador 2
    QGraphicsTextItem* labelJ2 = escena->addText("J2");
    labelJ2->setDefaultTextColor(j2->getColor().darker(150));
    labelJ2->setFont(QFont("Arial", 8, QFont::Bold));
    labelJ2->setPos((posCanon2.x - 8) * escala, (posCanon2.y - 25) * escala);
}

void VistaJuego::actualizar()
{
    actualizarProyectil();
    actualizarObstaculos();
}

void VistaJuego::actualizarObstaculos()
{
    std::vector<ObstaculoDestructible*> obstaculos = controlador->getObstaculos();

    for (int i = 0; i < rectObstaculos.size(); ++i) {
        // Buscar el obstáculo correspondiente por ID
        int idRect = rectObstaculos[i]->data(0).toInt();

        for (auto obs : obstaculos) {
            if (obs->getId() == idRect) {
                // Actualizar opacidad basada en la vida
                double porcentajeVida = obs->getPorcentajeVida();
                double opacidad = 0.3 + (porcentajeVida / 100.0) * 0.7; // Opacidad entre 0.3 y 1.0
                rectObstaculos[i]->setOpacity(opacidad);

                // Actualizar texto de vida
                if (i < textosVida.size()) {
                    textosVida[i]->setPlainText(QString("%1%").arg(static_cast<int>(porcentajeVida)));

                    // Cambiar color del texto según la vida
                    if (porcentajeVida > 50) {
                        textosVida[i]->setDefaultTextColor(Qt::white);
                    } else if (porcentajeVida > 25) {
                        textosVida[i]->setDefaultTextColor(QColor(255, 200, 0));
                    } else {
                        textosVida[i]->setDefaultTextColor(QColor(255, 100, 100));
                    }
                }
                break;
            }
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
            QPen penProy(Qt::black, 2);
            QBrush brushProy(QColor(50, 50, 50));

            proyectilGrafico = escena->addEllipse(
                0, 0, radio * 2 * escala, radio * 2 * escala,
                penProy, brushProy
                );
            proyectilGrafico->setZValue(10);
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

    for (int i = 0; i < textosVida.size(); ++i) {
        if (textosVida[i]->data(0).toInt() == idObstaculo) {
            escena->removeItem(textosVida[i]);
            delete textosVida[i];
            textosVida.removeAt(i);
            break;
        }
    }
}
