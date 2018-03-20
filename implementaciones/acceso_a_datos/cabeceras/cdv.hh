/**
 * \file
 * \brief Declaración de interfaz de acceso a dao.
 */

#ifndef __CDV__
#define __CDV__

#include "registro.hh"
#include "../../../utilidades/cabeceras/arreglo_de_digitos.hh"

namespace Implementaciones
{
  /**
   * \brief Interfaz de acceso a datos.
   *
   * Definición de comunicación con una fuente de datos. Define las operaciones
   * que cualquier clase concreta de acceso a datos debe de cumplir. Todo el
   * código del espacio de implementaciones debe utilizar esta interfaz.
   */

  class CDV
  {
    public:
      /** \brief Destructor virtual. */
      virtual ~CDV()
      {
      }

      /** \brief Busca el PAN dado en la base de datos. */
      virtual Registro buscarPorPan(const ArregloDeDigitos& PAN) = 0;

      /** \brief Busca el token dado en la base de datos. */
      virtual Registro buscarPorToken(const ArregloDeDigitos& token) = 0;

      /** \brief Guarda el registro dado en la base de datos. */
      virtual void guardar(const Registro& registro) = 0;

      /** \brief Elimina el registro con el identificador dado. */
      virtual void eliminar(int identificador) = 0;
  };
}

#endif
