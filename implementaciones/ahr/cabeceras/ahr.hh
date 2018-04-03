#ifndef __AHRHH__
#define __AHRHH__

#include "../../acceso_a_datos/cabeceras/cdv.hh"
#include "../../acceso_a_datos/cabeceras/acceso_mysql.hh"

/** \brief  Tamaño del bloque del cifrador en bytes: 16 bytes = 256 bits*/
#define M 16

//#define L 16

namespace Implementaciones
{
  class AHR{
  private:
    /** \brief  Entero con la entrada a tokenizar. */
    unsigned long long int entradaX;

    /** \brief  Entero con la entrada adicional (tweak). */
    unsigned long long int entradaU;

    /** \brief  Entero que almacena el token generado. */
    unsigned long long int token;

    /** \brief  Bloque que concatena la salida del SHA256 y la entradaX en binario. */
    unsigned char *bloqueT;

    /** \brief  Bloque que contiene al bloque cifrado. */
    unsigned char *bloqueC;

    /** \brief  Número de bytes necesarios para almacenar la entradaX:
      * N = log2(10^L) <- en bits, hay que pasarlo a bytes.
      */
    int N;

    /** \brief  Número de digitos de la entrada X:  */
    int L;

    /** \brief Pasa a binario entradaX y la almacena en los últimos N bits de bloqueT.*/
    void obtenerBitsX();

    /** \brief Concatena en T la salida truncada del SHA y la entradaX en binario.*/
    void crearBloqueT();

    /** \brief Guarda en token los últimos N bits de bloqueC en base 10. */
    void obtenerNumeroC();

    /** \brief  Verifica si existen el token creado en la base de datos. */
    bool existeToken();

    /** \brief Apuntador a la clase de acceso a la base de datos. */
    CDV* accesoADatos;

  public:
    /** \brief Constructor: recibe el número de bits para guardar a entradaX en base 2.*/
    AHR(CDV*);

    /** C\brief Constructor por copia. */
    AHR(AHR const&);

    /** \brief Operador de asignación. */
    AHR& operator= (AHR const&);

    /** \brief Destructor.*/
    ~AHR();

    /** \brief Método que obtiene el token dada una llave, una entradaX y una entradaU.*/
    void tokenizarHibridamente(unsigned char*, unsigned long int,
      unsigned long int);

    /** \brief Método que permite acceder a la variable token.*/
    unsigned long long int obtenerToken();

    /** \brief Obtener del PAN, el IIN y el número de cuenta */
    void separarPAN(char*);
  };
}

#endif
