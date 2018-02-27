/**
 * \file
 * \brief Implementación y definición de contenedor Arreglo.
 *
 * Al tratarse de una clase con plantillas («template»), la declaración y
 * la implementación deben estar juntas, en un mismo archivo; si se
 * intentan ligar por separado, hay problemas. Al parecer es técnica
 * estándar definir a las clases con plantillas dentro de los archivos de
 * cabeceras.
 */

#ifndef __ARREGLO__
#define __ARREGLO__

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <ostream>
#include <utility>

namespace Implementaciones
{
  /* Declaración *************************************************************/

  /**
   * \brief Contenedor de datos secuencial.
   *
   * Clase contenedor para datos secuenciales. El funcionamiento es muy similar
   * al «array» de la librería estándar (la ventaja es que con una
   * implementación propia podemos controlar operaciones de bajo nivel).
   *
   * Esta clase encapsula las operaciones de bajo nivel (con apuntadores y de
   * manejo de memoria) para ofrecer una interfaz clara y de alto nivel.
   *
   * \todo ¿En qué parte se refleja la documentación del template?
   *
   * \tparam tipo Tipo de dato del contenedor.
   */

  template <typename tipo>
  class Arreglo
  {
    public:

      /** \brief Construcción de arreglo del tamaño dado. */
      explicit Arreglo(int numeroDeElementos);

      /** \brief Construcción mediante lista de inicialización. */
      Arreglo(std::initializer_list<tipo> elementos);

      /** \brief Constructor de operación de copiado. */
      Arreglo(const Arreglo &arreglo);

      /** \brief Operación de asignación por copia */
      Arreglo& operator=(const Arreglo &arreglo);

      /** \brief Constructor de operación de movimiento. */
      Arreglo(Arreglo &&arreglo);

      /** \brief Operación de asignación por movimiento */
      Arreglo& operator=(Arreglo &&arreglo);

      /** \brief Destructor de arreglo. */
      ~Arreglo();

      /** \brief Operación de lectura. */
      tipo operator[] (int indice) const;

      /** \brief Operación de escritura. */
      void colocar(int indice, tipo valor);

      /** \brief Regresa el tamaño del arreglo. */
      inline int obtenerNumeroDeElementos() const { return mNumeroDeElementos; }

      /** \brief Parte el arreglo según parámetros. */
      Arreglo<tipo> partir(int numeroDePartes, int parte);

    private:

      /** \brief Tamaño del arreglo (entero mayor a cero). */
      int mNumeroDeElementos;

      /** \brief Apuntador a sección de memoria con el contenido del arreglo.*/
      tipo *mArregloInterno;
  };

  /** \brief Impresión de un arreglo. */
  template <typename tipo>
  std::ostream& operator<<(std::ostream &flujo, const Arreglo<tipo> &arreglo);

  /** \brief Concatenación de dos arreglos. */
  template <typename tipo>
  Arreglo<tipo> operator+(const Arreglo<tipo> &arregloUno,
    const Arreglo<tipo> &arregloDos);

  /* Definición **************************************************************/

  /**
   * Reserva la memoria necesaria para el arreglo. El tamaño reservado equivale
   * al tamaño del tipo de dato multiplicado por el número de elementos.
   *
   * \note Este constructor es explícito, mientras que el que recibe la lista
   * de inicialización, no. Esto implica (entre otras cosas) que:
   * ```
   * Arreglo<int> prueba = 5    // Error: no se puede hacer la conversión.
   * Arreglo<int> prueba(5)     // Bien: arreglo de tamaño 5.
   * Arreglo<int> prueba = {5}  // Bien: arreglo con un elemento (5).
   * Arreglo<int> prueba{5}     // Bien: mismo caso.
   * ```
   * La diferencia entre constructores explícitos y no explícitos también
   * se ve reflejada en las operaciones de copiado al llamar a una función:
   * ```
   * void funcionPrueba(Array<int> arreglo);
   * ...
   * funcionPrueba(6);          // Error: no se puede hacer la conversión.
   * funcionPrueba({6});        // Bien: pasa un arreglo con un seis.
   * ```
   *
   * \todo comprobación de tamaño (máximo, mínimo).
   */

  template<typename tipo>
  Arreglo<tipo>::Arreglo(
    int numeroDeElementos /**< Número de elementos del arreglo. */
  )
  : mNumeroDeElementos {numeroDeElementos},
    mArregloInterno {new tipo[mNumeroDeElementos]}
  {
  }

  /**
   * Construye un arreglo con los elementos dados en la lista de
   * inicialización.
   *
   * \warning La resolución de ambigüedades con respecto al constructor que
   * recibe el número de elementos le da prioridad a este constructor:
   * ```
   * Arreglo<int> arreglo {7};   // Arreglo con un solo elemento (7).
   * Arreglo<int> arreglo (7);   // Arreglo de tamaño 7.
   * ```
   * Estos casos (cuando hay constructores con listas de inicialización) son
   * los únicos en los que se permite (reglas de estilo) el uso de la notación
   * de paréntesis para operaciones de inicialización.
   *
   * \note Sobre la delegación de constructor en la lista de inicialización:
   * este lugar (la lista de inicialización) es el único en el que se puede
   * llamar a otro constructor de la misma clase con éxito; hacerlo en el
   * cuerpo de la función es probablemente un error (lógico, dado que en
   * la sintaxis no hay ningun problema; solo es un nuevo objeto sin
   * asignación).
   *
   * \sa http://www.cplusplus.com/reference/memory/uninitialized_copy/
   */

  template<typename tipo>
  Arreglo<tipo>::Arreglo(
    std::initializer_list<tipo> elementos /**< Contenido del arreglo. */
  )
  : Arreglo<tipo>(elementos.size())   /* Delegación de constructor. */
  {
    std::uninitialized_copy(elementos.begin(), elementos.end(),
      mArregloInterno);
  }

  /**
   * Contruye un nuevo arreglo a partir del dado. A diferencia de la
   * asignación por copia, en esta se reserva memoria para el nuevo objeto.
   *
   * Tanto para las operaciones de copia como para el movimiento, el compilador
   * crea comportamientos por defecto (en la mayoría de los casos no es
   * necesario escribir funciones propias); lo que estas hacen es hacer una
   * copia (o movimiento) miembro a miembro, lo cuál no sirve para esta clase:
   * después de una copia, ambos arreglos internos apuntarían a la misma
   * dirección de memoria (no es lo que se espera de una operación de copiado).
   *
   * \note Después de una operación de copia, ambos objetos (la fuente y el
   * destino), quedan con la misma información. Algunos casos en los que
   * se ocupa este constructor:
   * ```
   * void funcionPrueba(Arreglo<int> arreglo);
   * ...
   * Arreglo<int> a {7};    // Constructor de lista de inicialización.
   * Arreglo<int> b = a;    // Constructor por copia.
   * Arreglo<int> c {b};    // Constructor por copia.
   * funcionPrueba(b);      // Constructor por copia: b a «arreglo».
   * ```
   *
   * \todo :O Estoy accediendo a un objeto privado del otro arreglo (el fuente)
   * ¿Esto se puede hacer siempre que la variable sea de tipo «const»? ¿O es
   * una característica especial de los constructores por copia?
   *
   * \sa http://www.cplusplus.com/reference/memory/uninitialized_copy/
   */

  template<typename tipo>
  Arreglo<tipo>::Arreglo(
    const Arreglo<tipo> &arreglo  /**< Arreglo fuente. */
  )
  : Arreglo<tipo>(arreglo.obtenerNumeroDeElementos())
  {
    std::uninitialized_copy(arreglo.mArregloInterno,
      arreglo.mArregloInterno + mNumeroDeElementos, mArregloInterno);
  }

  /**
   * Copia el contenido del arreglo fuente en este arreglo; en este caso
   * la memoria ya debe de estar reservada.
   *
   * \return Referencia a sí mismo.
   *
   * \todo Comprobaciones de tamaño: si la fuente es más grande, lanzar
   * excepción, si es más pequeña, rellenar con valores por defecto.
   *
   * \sa http://www.cplusplus.com/reference/algorithm/copy/
   */

  template<typename tipo>
  Arreglo<tipo>& Arreglo<tipo>::operator=(
    const Arreglo<tipo> &arreglo  /**< Arreglo fuente. */
  )
  {
    std::copy(arreglo.mArregloInterno,
      arreglo.mArregloInterno + mNumeroDeElementos, mArregloInterno);
    return *this;
  }

  /**
   * Contruye un nuevo arreglo a partir del dado; en este caso el arreglo
   * fuente queda en estado inutilizable. No se reserva más memoria,
   * sino que solamente se actualizan los apuntadores.
   *
   * \note Después de una operación de movimiento solamente el objeto
   * destino queda en estado utilizable. El único caso (normal) en el que
   * se utiliza esta operación es al retorno de una función:
   * ```
   * Array<int> funcionPrueba()
   * {
   *   return Array<int>(5);                // Contructor de tamaño.
   * }
   * ...
   * Array<int> arreglo = funcionPrueba();  // Constructor por movimiento en
   *                                        // variable temporal y después
   *                                        // constructor por copia.
   * ```
   *
   * \note *&&* es una referencia a un *rvalue* (valor de lado derecho). Las
   * operaciones de movimiento están hechas para manejar este tipo de
   * referencias, mientras que las operaciones de copia permiten manejar
   * referencias a *lvalue* (valor de lado izquierdo).
   *
   * \todo Hmmm... aquí el argumento no es constante y de todos modos se
   * accede al apuntador privado.
   * Creo que tiene que ver con el hecho de que es un rvalue (un objeto
   * que ya no se va a utilizar más).
   */

  template<typename tipo>
  Arreglo<tipo>::Arreglo(
    Arreglo<tipo> &&arreglo   /**< Arreglo fuente. */
  )
  : mNumeroDeElementos {arreglo.mNumeroDeElementos},
    mArregloInterno {arreglo.mArregloInterno}
  {
    arreglo.mNumeroDeElementos = 0;
    arreglo.mArregloInterno = nullptr;
  }

  /**
   * Coloca el contenido del arreglo fuente en el destino, dejando a la
   * fuente lista para su destrucción. Para ocupar esta función es
   * necesario obtener un *rvalue* de la fuente:
   * ```
   * Arreglo<int> destino = std::move(origen)
   * ```
   * La función *move* regresa un *rvalue* del argumento dado.
   *
   * \return Referencia a sí mismo.
   *
   * \sa http://www.cplusplus.com/reference/utility/swap/
   */

  template<typename tipo>
  Arreglo<tipo>& Arreglo<tipo>::operator=(
    Arreglo<tipo> &&arreglo   /**< Arreglo fuente. */
  )
  {
    mNumeroDeElementos = arreglo.mNumeroDeElementos;
    mArregloInterno = arreglo.mArregloInterno;
    arreglo.mNumeroDeElementos = 0;
    arreglo.mArregloInterno = nullptr;
    return *this;
  }

  /**
   * Libera la memoria reservada para un arreglo.
   */

  template<typename tipo>
  Arreglo<tipo>::~Arreglo()
  {
    delete[] mArregloInterno;
  }

  /**
   * Regresa el elemento del arreglo en la posición dada.
   *
   * \return Elemento en la posición solicitada.
   *
   * \todo Comprobación de índice válido.
   */

  template<typename tipo>
  tipo Arreglo<tipo>::operator[](
    int indice /**< Índice de lectura. */
  ) const
  {
    return mArregloInterno[indice];
  }

  /**
   * Guarda el valor dado en la posición dada.
   *
   * \todo comprobación de índice válido.
   */

  template<typename tipo>
  void Arreglo<tipo>::colocar(
    int indice, /**< Posición del arreglo. */
    tipo valor  /**< Valor a guardar. */
  )
  {
    mArregloInterno[indice] = valor;
  }

  /**
   * Regresa un subarreglo del arreglo actual: primero se parte en
   * `numeroDePartes` distintas, y después se crea un nuevo arreglo con
   * la parte número `parte`.
   *
   * \warning El tamaño de la subdivisión está determinado por un división
   * entre enteros (truncada); en caso de división no entera y de petición
   * de último fragmento, los últimos elementos no van.
   *
   * \return Subarreglo número `parte`.
   *
   * \todo Soporte en los argumentos para solicitar varias partes a la vez.
   * \todo Validar coherencia en argumentos.
   */

  template<typename tipo>
  Arreglo<tipo> Arreglo<tipo>::partir(
    int numeroDePartes, /**< Número de particiones a hacer. **/
    int parte           /**< Número de partición deseada. **/
  )
  {
    int tamanioDeDivision = mNumeroDeElementos / numeroDePartes;
    Arreglo<tipo> subArreglo (tamanioDeDivision);
    for (int i = tamanioDeDivision * parte, j = 0;
      i < tamanioDeDivision * (parte + 1); i++, j++)
      subArreglo.colocar(j, mArregloInterno[i]);
    return subArreglo;
  }

  /**
   * Coloca en el flujo los elementos del arreglo separados por un
   * espacio. La disponibilidad de esta operación depende de la existencia
   * de una operación equivalente para un elemento del arreglo (error en
   * compilación).
   *
   * \return Referencia a flujo dado (para concatenar operaciones).
   *
   * \todo ¿Cómo hacer que el separador sea un parámetro más?
   */

  template <typename tipo>
  std::ostream& operator<<(
    std::ostream &flujo,          /**< Flujo de impresión. */
    const Arreglo<tipo> &arreglo  /**< Arreglo a imprimir. */
  )
  {
    for (int i = 0; i < arreglo.obtenerNumeroDeElementos(); i++)
      flujo << arreglo[i] << " ";
    return flujo;
  }

  /**
   * Operación de concatenación de arreglo: crea un nuevo objeto con
   * espacio para concatenar a los dos arreglos dados y los coloca en el
   * orden dado (la operación no es conmutativa).
   *
   * \todo Validación de coherencia de tipos.
   */

  template <typename tipo>
  Arreglo<tipo> operator+(
    const Arreglo<tipo> &arregloUno,  /**< Primer arreglo de concatenación. */
    const Arreglo<tipo> &arregloDos   /**< Segundo arreglo de concatenación. */
  )
  {
    int mitad = arregloUno.obtenerNumeroDeElementos();
    int total = mitad + arregloDos.obtenerNumeroDeElementos();
    Arreglo<tipo> resultado (total);
    for (int i = 0; i < total; i++)
      resultado.colocar(i, (i < mitad) ? arregloUno[i] : arregloDos[i - mitad]);
    return resultado;
  }
}

#endif
