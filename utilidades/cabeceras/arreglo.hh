/**
 * \file
 * \brief Implementación y definición de contenedor Arreglo.
 *
 * Al tratarse de una clase con plantillas («template»), la declaración y
 * la implementación deben estar juntas, en un mismo archivo; si se
 * intentan ligar por separado, hay problemas. Al parecer es técnica
 * estándar definir a las clases con plantillas dentro de los archivos de
 * cabeceras.
 *
 * Proyecto Lovelace.
 */

#ifndef __ARREGLO__
#define __ARREGLO__

#include "error.hh"
#include "intermediario_de_arreglo.hh"
#include "utilidades_matematicas.hh"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <ostream>
#include <utility>

/* Declaración *************************************************************/

template<typename tipo>
class Arreglo;

/** \brief Impresión de un arreglo. */
template <typename tipo>
std::ostream& operator<<(std::ostream& flujo, const Arreglo<tipo>& arreglo);

/** \brief Concatenación de dos arreglos. */
template <typename tipo>
Arreglo<tipo> operator||(const Arreglo<tipo>& arregloUno,
  const Arreglo<tipo>& arregloDos);

/** \brief Comparación de igualdad entre arreglos. */
template <typename tipo>
bool operator==(const Arreglo<tipo>& arregloUno,
  const Arreglo<tipo>& arregloDos);

/** \brief Comparación de desigualdad entre arreglos. */
template <typename tipo>
bool operator!=(const Arreglo<tipo>& arregloUno,
  const Arreglo<tipo>& arregloDos);

/** \brief Convierte el número dado en un arreglo. */
template <typename tipoDeArreglo, typename tipoDeNumero>
Arreglo<tipoDeArreglo> convertirAArreglo(tipoDeNumero numero,
  int base, int tamanio);

/** \brief Regresa la representación entera del arreglo. */
template<typename tipoDeArreglo, typename tipoDeNumero>
tipoDeNumero convertirANumero(
  const Arreglo<tipoDeArreglo>& arreglo, int base);

/**
 * \brief Alias para ArregloBase.
 *
 * Esta clase es la cara «externa» de ArregloBase. En cualquier lugar
 * en donde se ocupe un ArregloBase se debería poder ocupar un Arreglo.
 *
 * El por qué de este tipo de alias es para poder hacer especializaciones del
 * arreglo que extiendan a la base; esto es, si se quieren hacer funciones
 * especializadas para un tipo específico (por ejemplo, el arreglo de bytes)
 * se puedan seguir usando las demás funcionalidades que no se
 * sobreescriban.
 */

template<typename tipo>
class Arreglo : public ArregloBase<tipo>
{
  public:
    /** \brief Herencia de constructores. */
    using ArregloBase<tipo>::ArregloBase;

  private:
    /** \brief Función de impresión como amiga. */
    friend std::ostream& operator<< <tipo>(
      std::ostream& flujo, const Arreglo<tipo>& arreglo);
};

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
 * \todo Integrar el arreglo con las clases de iteradores de la biblioteca
 * estándar para poder escribir ciclos con la sintaxis de for-each:
 * ```
 * Arreglo<int> elementos {1, 2, 3};
 * for (auto elemento : elementos)
 *   cout << elemento << endl;
 * ```
 *
 * \tparam tipo Tipo de dato del contenedor.
 */

template <typename tipo>
class ArregloBase
{
  public:

    /** \brief Construcción por defecto. */
    explicit ArregloBase();

    /** \brief Construcción de arreglo del tamaño dado. */
    explicit ArregloBase(unsigned int numeroDeElementos);

    /** \brief Construcción mediante lista de inicialización. */
    ArregloBase(std::initializer_list<tipo> elementos);

		/** \brief Construcción por memoria ya existente. */
		ArregloBase(unsigned int numeroDeElementos, tipo* &&memoria);

    /** \brief Constructor de operación de copiado. */
    ArregloBase(const ArregloBase &arreglo);

    /** \brief Operación de asignación por copia. */
    ArregloBase& operator=(const ArregloBase &arreglo);

    /** \brief Constructor de operación de movimiento. */
    ArregloBase(ArregloBase &&arreglo);

    /** \brief Operación de asignación por movimiento */
    ArregloBase& operator=(ArregloBase &&arreglo);

    /** \brief Destructor de arreglo. */
    ~ArregloBase();

    /** \brief Operación de subíndice (no constante). */
    Utilidades::IntermediarioDeArreglo<tipo> operator[](int indice);

    /** \brief Operación de subíndice (constante). */
    virtual tipo operator[](int indice) const;

    /** \brief Operación de escritura. */
    virtual void colocar(int indice, tipo valor);

    /** \brief Colocar una constante en todo el arreglo. */
    void colocarConstante(tipo constante);

    /** \brief Regresa el tamaño del arreglo. */
    inline unsigned int obtenerNumeroDeElementos() const
      { return mNumeroDeElementos; }

    /** \brief Regresa una copia del arreglo interno. */
    tipo *obtenerCopiaDeArreglo() const;

    /** \brief Regresa un apuntador al arreglo interno. */
    inline const tipo *obtenerApuntador() const
      { return mArregloInterno; }

    /** \brief Parte el arreglo según parámetros. */
    ArregloBase<tipo> partir(int numeroDePartes, int parte,
      int desviacion = 0) const;

    /** \brief Operación de división entre arreglo de índices. */
    Arreglo<Arreglo<tipo>> operator/(
      const Arreglo<unsigned int> &marcasDivisorias) const;

    /** \brief División entre arreglo de índices. Función de compatibilidad. */
    Arreglo<Arreglo<tipo>> fragmentar(
      const Arreglo<unsigned int> &marcasDivisorias) const;

    /** \brief Error para representar un acceso ilegal. */
    struct AccesoFueraDeRango : public Utilidades::Error
    { inline AccesoFueraDeRango(std::string mensaje)
      : Utilidades::Error{mensaje} {} };

  protected:

    /** \brief Tamaño del arreglo (entero mayor a cero). */
    unsigned int mNumeroDeElementos;

    /** \brief Apuntador a sección de memoria con el contenido del arreglo.*/
    tipo *mArregloInterno;

    /** \brief Aplica una resolución al índice dado. */
    int resolverIndice(int indice) const;

  private:

    /** \brief Clase intermediario como amiga. */
    friend class Utilidades::IntermediarioDeArreglo<tipo>;
};

/* Arreglo de bytes *******************************************************/

/** \brief Impresión de un arreglo de bytes. */
std::ostream& operator<<(std::ostream& flujo,
  const Arreglo<unsigned char>& arreglo);

/** \brief Sumas entre arreglos de bytes */
Arreglo<unsigned char> operator+(const Arreglo<unsigned char>& arregloUno,
  const Arreglo<unsigned char>& arregloDos);

/** \brief Xor entre arreglos */
Arreglo<unsigned char> operator^(const Arreglo<unsigned char>& arregloUno,
  const Arreglo<unsigned char>& arregloDos);

/**
 * \brief Especialización de Arreglo para bytes.
 *
 * Define operaciones únicas a los arreglos de bytes.
 */

template<>
class Arreglo<unsigned char> : public ArregloBase<unsigned char>
{
  public:

    /** \brief Herencia de constructores. */
    using ArregloBase<unsigned char>::ArregloBase;

    /** \brief Constructor por defecto. */
    explicit Arreglo<unsigned char>();

    /** \brief Descomposición en bytes de entero. */
    Arreglo<unsigned char>(entero numero);

  private:

    /** \brief Función de impresión especializada como amiga. */
    friend std::ostream& operator<<(
      std::ostream& flujo, const Arreglo<unsigned char>& arreglo);
};

/* Definición **************************************************************/

/**
 * Constructor vacío. Inicializa conun nullptr el arreglo y coloca un
 * 0 en el contador de elementos.
 */

template<typename tipo>
ArregloBase<tipo>::ArregloBase()
: mNumeroDeElementos {0u},
  mArregloInterno {nullptr}
{
}

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
 */

template<typename tipo>
ArregloBase<tipo>::ArregloBase(
  unsigned int numeroDeElementos /**< Número de elementos del arreglo. */
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
ArregloBase<tipo>::ArregloBase(
  std::initializer_list<tipo> elementos   /**< Contenido del arreglo. */
)
: ArregloBase<tipo>(elementos.size())         /* Delegación de constructor. */
{
  std::uninitialized_copy(elementos.begin(), elementos.end(),
    mArregloInterno);
}

/**
 * Permite contruir un arreglo a partir de un apuntador a memoria ya reservada.
 * Funciona de forma bastante similar al contructor por copia, excepto que en
 * este caso la memoria de origen es un arreglo de bajo nivel.
 */

template <typename tipo>
ArregloBase<tipo>::ArregloBase(
	unsigned int numeroDeElementos,      /**< Número de elementos del arreglo. */
	tipo* &&memoria                      /**< Apuntador a memoria reservada. */
)
: mNumeroDeElementos {numeroDeElementos},
	mArregloInterno {memoria}
{
	memoria = nullptr;
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
 * \sa http://www.cplusplus.com/reference/memory/uninitialized_copy/
 */

template<typename tipo>
ArregloBase<tipo>::ArregloBase(
  const ArregloBase<tipo> &arreglo  /**< ArregloBase fuente. */
)
: ArregloBase<tipo>(arreglo.obtenerNumeroDeElementos())
{
  std::uninitialized_copy(arreglo.mArregloInterno,
    arreglo.mArregloInterno + mNumeroDeElementos, mArregloInterno);
}

/**
 * Copia el contenido del arreglo fuente en este arreglo. Para esto primero
 * libera la memoria anterior y después reserva la necesaria para la copia.
 *
 * \return Referencia a sí mismo.
 *
 * \sa http://www.cplusplus.com/reference/algorithm/copy/
 */

template<typename tipo>
ArregloBase<tipo>& ArregloBase<tipo>::operator=(
  const ArregloBase<tipo> &arreglo  /**< ArregloBase fuente. */
)
{
  delete[] mArregloInterno;
  mNumeroDeElementos = arreglo.obtenerNumeroDeElementos();
  mArregloInterno = new tipo[mNumeroDeElementos];
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
 */

template<typename tipo>
ArregloBase<tipo>::ArregloBase(
  ArregloBase<tipo> &&arreglo   /**< Arreglo fuente. */
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
ArregloBase<tipo>& ArregloBase<tipo>::operator=(
  ArregloBase<tipo> &&arreglo   /**< ArregloBase fuente. */
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
ArregloBase<tipo>::~ArregloBase()
{
  delete[] mArregloInterno;
}

/**
 * Crea un nuevo intermediario con el arreglo asociado a la posición dada.
 * Esta es la operación que el compilador resuelve en la gran mayoría de
 * los casos; la otra (operator[](int indice) const) solo se utiliza cuando el
 * arreglo es constante.
 *
 * La principal función de esto es poder hacer asignaciones con este operador:
 * ```
 * Arreglo<int> prueba {1, 2, 3};
 * prueba[0] = 9;
 * ```
 *
 * «prueba[0]» regresa una instancia de IntermediarioDeArreglo, la cual tiene
 * una referencia al índice dado. IntermediarioDeArreglo sobreescribe la
 * operación de asignación.
 *
 * \return Instancia de intermediario asociado al índice dado.
 *
 * \throw AccesoFueraDeRango Si el índice dado no se encuentra en el rango del
 * arreglo intenro.
 */

template<typename tipo>
Utilidades::IntermediarioDeArreglo<tipo> ArregloBase<tipo>::operator[](
  int indice                             /**< Índice de elemento. */
)
{
  return Utilidades::IntermediarioDeArreglo<tipo>(*this, resolverIndice(indice));
}

/**
 * Regresa el elemento que hay en el índice dado. A diferencia de
 * operator[](int indice) esta operación solamente se utiliza cuando se trata
 * con arreglos constantes.
 *
 * \return Copia de elemento en índice.
 *
 * \throw AccesoFueraDeRango Si el índice dado no se encuentra en el rango del
 * arreglo intenro.
 */

template<typename tipo>
tipo ArregloBase<tipo>::operator[](
  int indice                             /**< Índice de elemento. */
) const
{
  return mArregloInterno[resolverIndice(indice)];
}

/**
 * Guarda el valor dado en la posición dada.
 *
 * \deprecated A partir de
 * [este commit](https://github.com/RQF7/proyecto_lovelace/commit/0785d61c33f6f8bfa84f5005337c8683894ddebf)
 * el intermediario de arreglo funciona para hacer asignaciones:
 * ```
 * Arreglo<int> prueba {1, 2, 3};
 * prueba[0] = 9;
 * ```
 * «prueba[0]» regresa una instancia de Utilidades::IntermediarioDeArreglo, la
 * cual tiene una referencia al índice dado. IntermediarioDeArreglo
 * sobreescribe la operación de asignación.
 */

template<typename tipo>
[[ deprecated("No es lo que se espera de un arreglo; mejor usar [].") ]]
void ArregloBase<tipo>::colocar(
  int indice,                            /**< Posición del arreglo. */
  tipo valor                             /**< Valor a guardar. */
)
{
  mArregloInterno[indice] = valor;
}

/**
 * Coloca la constante dada en todo el arreglo.
 */

template<typename tipo>
void ArregloBase<tipo>::colocarConstante(
  tipo constante      /**< Valor a colocar en el arreglo. */
)
{
  for (unsigned int i = 0; i < mNumeroDeElementos; i++)
    mArregloInterno[i] = constante;
}

/**
 * Regresa una copia del arreglo interno.
 *
 * \warning El usuario es el encargado de liberar esta memoria. Se debe
 * utilizar solo para interactuar con código viejo; La gestión de la memoria
 * sólo se debería de encontrar en el sistema de contructores y destructores.
 */

template<typename tipo>
tipo *ArregloBase<tipo>::obtenerCopiaDeArreglo() const
{
  tipo *resultado = new tipo [mNumeroDeElementos];
  memcpy(resultado, mArregloInterno, mNumeroDeElementos);
  return resultado;
}

/**
 * Regresa un subarreglo del arreglo actual: primero se parte en
 * `numeroDePartes` distintas, y después se crea un nuevo arreglo con
 * la parte número `parte`.
 *
 * La desviación se aplica sobre el lugar en el que normalmente estaría
 * el corte. Números positivos implican una desviación a la derecha y
 * negativos a la izquierda (suponiendo una representación abstracta del
 * arreglo en el que «derecha» significa más cerca del primer índice e
 * «izquierda» más cerca del final).
 *
 * \warning El tamaño de la subdivisión está determinado por un división
 * entre enteros (truncada); el posible desface en caso de divisiones
 * no enteras coloca los elementos extras en el último fragmento:
 * ```
 * |-------- 7 --------|
 * partido en 3 partes:
 * |- 2 -||- 2 -||- 3 -|
 * ```
 *
 * \return Subarreglo número `parte`.
 *
 * \deprecated A partir de
 * [este commit](https://github.com/RQF7/proyecto_lovelace/commit/b375defcd0070cf6537a793a7d73b7fcb9f38256),
 * se utiliza la operación de división entre arreglos.
 */

template<typename tipo>
[[ deprecated("Se sustituye por operador de división entre arreglo") ]]
ArregloBase<tipo> ArregloBase<tipo>::partir(
  int numeroDePartes, /**< Número de particiones a hacer. **/
  int parte,          /**< Número de partición deseada. **/
  int desviacion      /**< Desviación con respecto al corte (0 por defecto). **/
) const
{
  int tamanioDeDivision = mNumeroDeElementos / numeroDePartes;
  int inicio = (parte == 0)
    ? 0
    : tamanioDeDivision * parte + desviacion;
  int fin = (parte == numeroDePartes - 1)
    ? mNumeroDeElementos
    : tamanioDeDivision * (parte + 1) + desviacion;
  ArregloBase<tipo> subArreglo (fin - inicio);
  for (int i = inicio, j = 0; i < fin; i++, j++)
    subArreglo[j] = mArregloInterno[i];
  return subArreglo;
}

/**
 * Permite crear subarreglos a partir del propio. Parte el arreglo
 * actual según los marcadores del divisor. Por ejemplo:
 * [1, 2, 3, 4] / [1, 3] da como resultado
 * [ [1], [2, 3], 4 ].
 * Las marcas divisorias indican el índice posterior a la partición.
 * Esto es, es un error incluir el 0 o NumeroDeElementos.
 *
 * Aunque con un formato en los argumentos un poco distinto, esta
 * función sustituye casi por completo a Arreglo::partir (marcada como
 * obsoleta). Los únicos casos en los que podría seguir siendo válido
 * seguir utilizando Arreglo::partir es cuando el arreglo es muy grando
 y solo se necesita una fracción pequeña de él (que sea ridículo calcular
 * todas las particiones cuando solo es una la que se ocupa). De momento no
 * existe el caso arriba mencionado, por lo que se quedará como obsoleta.
 *
 * \tparam marcasDivisorias Arreglo con marcas divisorias.
 *
 * \sa fragmentar.
 *
 * \return Arreglo de arreglos con subpartes del original.
 */

template<typename tipo>
Arreglo<Arreglo<tipo>> ArregloBase<tipo>::operator/(
  const Arreglo<unsigned int> &marcasDivisorias
)
const
{
  return fragmentar(marcasDivisorias);
}

/**
 * Función espejo de ArregloBase<tipo>::operator/
 * Para compiladores que mueren con la otra función (GCC 6).
 *
 * \return Arreglo de arreglos con subpartes del original.
 */

template <typename tipo>
Arreglo<Arreglo<tipo>> ArregloBase<tipo>::fragmentar(
  const Arreglo<unsigned int> &marcasDivisorias
)
const
{
  Arreglo<Arreglo<tipo>> resultado(
    marcasDivisorias.obtenerNumeroDeElementos() + 1);
  unsigned int acumulado = 0;
  unsigned int marcaActual = marcasDivisorias[0];
  for (unsigned int i = 0; i < resultado.obtenerNumeroDeElementos(); i++)
  {
    Arreglo<tipo> subArreglo(marcaActual - acumulado);
    for (unsigned int j = 0; j < subArreglo.obtenerNumeroDeElementos(); j++)
      subArreglo[j] = mArregloInterno[acumulado + j];
    resultado[i] = subArreglo;
    if (i == resultado.obtenerNumeroDeElementos() - 1)
      break;
    acumulado += subArreglo.obtenerNumeroDeElementos();
    marcaActual = (i != resultado.obtenerNumeroDeElementos() - 2) ?
      marcasDivisorias[i + 1] :
      mNumeroDeElementos;
  }
  return resultado;
}

/**
 * Verifica que el índice dado se encuentre en el rango de la memoria interna.
 * Los índices negativos se resuelven de atrás hacia adelante (-1 representa
 * al último elemento, -2 al penúltimo, etcétera).
 *
 * \return El índice después de la resolución.
 *
 * \throw AccesoFueraDeRango Si el índice dado no se encuentra en el rango del
 * arreglo intenro.
 */

template<typename tipo>
int ArregloBase<tipo>::resolverIndice(
  int indice                        /**< Índice a resolver. */
) const
{
  if (indice < 0)
    indice = mNumeroDeElementos + indice;
  if (indice < 0 || static_cast<unsigned int>(indice) >= mNumeroDeElementos)
    throw AccesoFueraDeRango{
      "El índice no se encuentra en el rango del arreglo"};
  return indice;
}

/**
 * Coloca en el flujo los elementos del arreglo separados por un
 * espacio. La disponibilidad de esta operación depende de la existencia
 * de una operación equivalente para un elemento del arreglo (error en
 * compilación).
 *
 * \return Referencia a flujo dado (para concatenar operaciones).
 */

template <typename tipo>
std::ostream& operator<<(
  std::ostream &flujo,          /**< Flujo de impresión. */
  const Arreglo<tipo> &arreglo  /**< Arreglo a imprimir. */
)
{
  for (unsigned int i = 0; i < arreglo.obtenerNumeroDeElementos(); i++)
    flujo << arreglo.mArregloInterno[i] << " ";
  return flujo;
}

/**
 * Operación de concatenación de arreglo: crea un nuevo objeto con
 * espacio para concatenar a los dos arreglos dados y los coloca en el
 * orden dado (la operación no es conmutativa).
 *
 * \return Arreglo con los dos parámetros concatenados.
 */

template <typename tipo>
Arreglo<tipo> operator||(
  const Arreglo<tipo> &arregloUno,  /**< Primer arreglo de concatenación. */
  const Arreglo<tipo> &arregloDos   /**< Segundo arreglo de concatenación. */
)
{
  unsigned int mitad = arregloUno.obtenerNumeroDeElementos();
  unsigned int total = mitad + arregloDos.obtenerNumeroDeElementos();
  Arreglo<tipo> resultado (total);
  for (unsigned int i = 0; i < total; i++)
    resultado[i] = (i < mitad) ? arregloUno[i] : arregloDos[i - mitad];
  return resultado;
}

/**
 * Primero compara número de elemetos y después compara elemento a
 * elemento.
 *
 * \return Validación de igualdad.
 */

template <typename tipo>
bool operator==(
  const Arreglo<tipo> &arregloUno,  /**< Primer arreglo. */
  const Arreglo<tipo> &arregloDos   /**< Segundo arreglo. */
)
{
  if (arregloUno.obtenerNumeroDeElementos() !=
    arregloDos.obtenerNumeroDeElementos())
    return false;
  for (unsigned int i = 0; i < arregloUno.obtenerNumeroDeElementos(); i++)
    if (arregloUno[i] != arregloDos[i])
      return false;
  return true;
}

/**
 * Simplemente hace el complemento de la validación de igualdad.
 *
 * \return Validación de desigualdad.
 */

template <typename tipo>
bool operator!=(
  const Arreglo<tipo> &arregloUno,  /**< Primer arreglo. */
  const Arreglo<tipo> &arregloDos   /**< Segundo arreglo. */
)
{
  return !(arregloUno == arregloDos);
}

/**
 * Crea un arreglo a partir de los argumentos dados: interpreta el número
 * como dígitos de la base dada. El tamaño es para especificar la longitud
 * del arreglo; en caso de un número menor, se colocan ceros a la izquierda.
 *
 * El formato dado es en *little endian*: los números menos significativos se
 * encuentran al inicio del arreglo.
 *
 * \sa http://www.cplusplus.com/reference/cmath/pow/
 *     http://www.cplusplus.com/reference/cmath/floor/
 *
 * \deprecated A partir de
 * [este commit](https://github.com/RQF7/proyecto_lovelace/commit/1413ef905357af7dd8159f44c9938e259be00f84)
 * esta función queda sustituida por la operación del contructor de un
 * \ref ArregloDeDigitos.
 */

template <typename tipoDeArreglo, typename tipoDeNumero>
[[ deprecated("Mejor usar arreglo de dígitos; tiene más sentido.") ]]
Arreglo<tipoDeArreglo> convertirAArreglo(
  tipoDeNumero numero,        /**< Número a convertir. */
  int base,                   /**< Base de la conversión. */
  int tamanio                 /**< Número de dígitos. */
)
{
  Arreglo<tipoDeArreglo> resultado (tamanio);
  for (int i = tamanio - 1; i >= 0; i--)
  {
    tipoDeNumero equivalentePotencia = potencia<tipoDeNumero>(base, i);
    tipoDeArreglo digito = floor(numero / equivalentePotencia);
    resultado[i] = digito;
    numero -= digito * equivalentePotencia;
  }
  return resultado;
}

/**
 * Interpreta el contenido del arreglo como un número escrito en dígitos
 * de la base dada.
 *
 * El formato esperado es en *little endian*: los números menos significativos
 * se encuentran al inicio del arreglo.

 * \return Número equivalente.
 *
 * \sa http://www.cplusplus.com/reference/cmath/pow/
 *
 * \deprecated A partir de
 * [este commit](https://github.com/RQF7/proyecto_lovelace/commit/1413ef905357af7dd8159f44c9938e259be00f84)
 * esta función queda sustituida por la operación del contructor de un
 * \ref ArregloDeDigitos.
 */

template<typename tipoDeArreglo, typename tipoDeNumero>
[[ deprecated("Mejor usar arreglo de dígitos; tiene más sentido.") ]]
tipoDeNumero convertirANumero(
  const Arreglo<tipoDeArreglo> &arreglo,    /**< Referencia a arreglo. */
  int base                                  /**< Base de la conversión. */
)
{
  tipoDeNumero resultado {0};
  for (unsigned int i = 0; i < arreglo.obtenerNumeroDeElementos(); i++)
    resultado += arreglo[i] * potencia<tipoDeNumero>(base, i);
  return resultado;
}

#endif
