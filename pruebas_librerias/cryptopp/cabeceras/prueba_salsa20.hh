/*
 * Prueba de AES, declaración, crypto++
 * Proyecto Lovelace.
 */

#ifndef __PRUEBA_SALSA20__
#define __PRUEBA_SALSA20__

#include "../../utilidades/cabeceras/prueba.hh"

/**
 * \brief Prueba de la impementación de Salsa20 de cryptopp.
 *
 * Agrupa las funciones de prueba que interactúan con la implementación
 * de Salsa20 de cryptopp.
 */

class PruebaSalsa20 : public Prueba
{
  public:
    /** \brief Regitro de pruebas en vector de funciones. */
    PruebaSalsa20();

    /** \brief Prueba el proceso de cifrado y descifrado de Salsa20. */
    static bool probarCifradoDescifrado();
};

#endif