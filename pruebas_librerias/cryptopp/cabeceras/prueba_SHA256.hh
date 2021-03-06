/**
 * \file
 * \brief Prueba de SHA256, declaración, crypto++
 * Proyecto Lovelace
 */

#ifndef __PRUEBA_SHA256_CRYPTOPP__
#define __PRUEBA_SHA256_CRYPTOPP__

#include "../../../utilidades/cabeceras/prueba.hh"

namespace PruebasCryptopp
{
  /**
   * \brief Prueba de la implementación SHA-256 de Crypto++
   *
   * Pequeña prueba para obtener el valor HASH de la entrada abc en SHA-256.
   * Se prueban también las constantes.
   */

  class PruebaSHA256 : public Utilidades::Prueba
  {
    public:
      /** \brief Registro de funciones estáticas. */
      PruebaSHA256();

      /** \brief Prueba funcionamiento de SHA256. */
      static bool probarDigestion();
  };
}

#endif
