/**
 * \file
 * \brief Prueba de IDEA, declaración, openssl
 * Proyecto Lovelace.
 */

#ifndef __PRUEBA_IDEA_OPENSSL__
#define __PRUEBA_IDEA_OPENSSL__

#include "../../../utilidades/cabeceras/prueba.hh"

namespace PruebasOpenssl
{
  /**
   * \brief Prueba de la impementación de IDEA de openssl.
   *
   * Agrupa las funciones de prueba que interactúan con la implementación
   * de IDEA de openssl.
   */

  class PruebaIDEA : public Prueba
  {
    public:
      /** \brief Regitro de pruebas en vector de funciones. */
      PruebaIDEA();

      /** \brief Prueba el funcionamiento de IDEA. */
      static bool probarCifradoDescifrado();
  };
}

#endif
