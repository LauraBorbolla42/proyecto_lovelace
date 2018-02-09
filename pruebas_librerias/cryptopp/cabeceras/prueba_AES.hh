/*
 * Prueba de AES, declaración, crypto++
 * Proyecto Lovelace.
 */

#ifndef __PRUEBA_AES__
#define __PRUEBA_AES__

#include "../../utilidades/cabeceras/prueba.hh"

/**
 * \brief Prueba de la impementación de AES de cryptopp.
 *
 * Agrupa las funciones de prueba que interactúan con la implementación
 * de AES de cryptopp.
 */

class PruebaAES : public Prueba
{
  public:

    /** \brief Regitro de pruebas en vector de funciones. */
    PruebaAES();
};

#endif