#ifndef CAJA_H
#define CAJA_H

#include "particula.h"

class Caja
{
private:
    double ancho;
    double alto;
public:
    Caja(double ancho, double alto);

    double getAncho() const;
    double getAlto() const;

    bool dentroDeLimites(const Particula& p) const;

};

#endif // CAJA_H
