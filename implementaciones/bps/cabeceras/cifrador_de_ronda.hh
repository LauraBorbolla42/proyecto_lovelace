/**
 * \file
 * \brief Definición de la clase del cifrado de ronda.
 *
 * Proyecto Lovelace.
 */

#ifndef __CIFRADOR_DE_RONDA__
#define __CIFRADOR_DE_RONDA__

#include <iostream>
#include <iomanip>

#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>

namespace Implementaciones
{
  class CifradorDeRonda
  {
    private:
      unsigned int tipo;

    public:
      /* No se usó el estilo de las reglas por que son banderas no variables. */
      /** \brief Bandera para usar AES. */
      static const unsigned int BANDERA_AES  = 1;

      /** \brief Bandera para usar TDES. */
      static const unsigned int BANDERA_TDES = 2;

      /** \brief Constructor de la clase sin argumentos. */
      CifradorDeRonda();

      /** \brief Constructor de la clase con argumentos. */
      CifradorDeRonda(unsigned int cifrador);

      /** \brief Método para cambiar el tipo de cifrado que se usa. */
      void colocarCifrador(unsigned int cifrador);

      /** \brief Método para obtener el tipo de cifrado que se usa. */
      unsigned int obtenerCifrador();

      /** \brief Método para obtener el tamaño de bloque del cifrado. */
      unsigned int obtenerTamBloque();

      /** \brief Método para cifrar un mensaje con la llave dada. */
      std::string cifrar(std::string mensaje, CryptoPP::byte llave[]);

      /** \brief Método para descifrar un mensaje con la llave dada. */
      std::string descifrar(std::string mensaje, CryptoPP::byte llave[]);
  };
}

#endif
