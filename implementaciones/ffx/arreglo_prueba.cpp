/**
 * \file
 * \brief Definición de funciones de prueba del contenedor tipo arreglo.
 * Proyecto Lovelace.
 */

#include "cabeceras/arreglo_prueba.hh"
#include "cabeceras/arreglo.hh"
#include <iostream>
#include <string>
#include <vector>

using namespace Implementaciones;
using namespace ImplementacionesPruebas;
using namespace std;

ArregloPrueba::ArregloPrueba()
{
  mListaDePruebas.push_back(FuncionDePrueba{
    "funcionalidad básica",
    ArregloPrueba::probarFuncionalidadBasica
  });
  mListaDePruebas.push_back(FuncionDePrueba{
    "generación de particiones",
    ArregloPrueba::probarParticiones
  });
  mListaDePruebas.push_back(FuncionDePrueba{
    "concatenación",
    ArregloPrueba::probarConcatenacion
  });
  mListaDePruebas.push_back(FuncionDePrueba{
    "constructores",
    ArregloPrueba::probarConstructores
  });
  mListaDePruebas.push_back(FuncionDePrueba{
    "operaciones de comparación",
    ArregloPrueba::probarOperadoresComparacion
  });
  mListaDePruebas.push_back(FuncionDePrueba{
    "conversiones numéricas",
    ArregloPrueba::probarConversionesNumericas
  });
}

/**
 * Funcionalidad inicial (de la primera implementación) del contenedor tipo
 * arreglo.
 *
 * Se prueba la reserva de memoria; las operaciones de lectura y escritura;
 * y la impresión en un flujo. Se prueba con distintos tipos (tanto básicos
 * como compuestos).
 *
 * \return Estado de la prueba.
 */

bool ArregloPrueba::probarFuncionalidadBasica()
{
  Arreglo<int> arreglo (10);
  for (int i = 0; i < 10; i++)
    arreglo.colocar(i, i + 1);
  cout << arreglo << endl;
  if (arreglo[3] != 4 || arreglo[7] != 8 || arreglo[9] != 10)
    return false;

  Arreglo<char> arreglo2 (3);
  arreglo2.colocar(0, 'A');
  arreglo2.colocar(1, 'B');
  arreglo2.colocar(2, 'C');
  cout << arreglo2 << endl;
  if (arreglo2[0] != 'A' || arreglo2[1] != 'B' || arreglo2[2] != 'C')
    return false;

  Arreglo<vector<int>> arreglo3 (2);
  arreglo3.colocar(0, {0, 1, 2, 3, 4});
  arreglo3.colocar(1, {5, 6, 7, 8, 9});
  if (arreglo3[0][0] != 0 || arreglo3[0][3] != 3 || arreglo3[1][4] != 9)
    return false;

  Arreglo<string> arreglo4 (3);
  arreglo4.colocar(0, "Ricardo");
  arreglo4.colocar(1, "Quezada");
  arreglo4.colocar(2, "Figueroa");
  cout << arreglo4 << endl;
  if (arreglo4[0] != "Ricardo" || arreglo4[2] != "Figueroa")
    return false;

  return true;
}

/**
 * Prueba de particiones de un arreglo. Se prueban dos particiones exactas,
 * una con residuo y dos con una desviación.
 *
 * \return Estado de la prueba.
 */

bool ArregloPrueba::probarParticiones()
{
  Arreglo<int> arreglo (10);
  for (int i = 0; i < 10; i++)
    arreglo.colocar(i, i + 1);

  /* Partición exacta: 5 */
  auto primerMedio = arreglo.partir(2, 0);
  auto segundoMedio = arreglo.partir(2, 1);
  cout << "Medios: " << endl
       << primerMedio << endl
       << segundoMedio << endl;
  if (primerMedio[2] != 3 || segundoMedio[4] != 10)
    return false;

  /* Partición exacta: 2 */
  auto segundoQuinto = arreglo.partir(5, 1);
  auto quintoQuinto = arreglo.partir(5, 4);
  cout << "Quintos: " << endl
       << segundoQuinto << endl
       << quintoQuinto << endl;
  if (segundoQuinto[1] != 4 || quintoQuinto[0] != 9)
    return false;

  /* Partición con residuo: 2, 2, 2, 4 */
  auto primerCuarto = arreglo.partir(4, 0);
  auto cuartoCuarto = arreglo.partir(4, 3);
  cout << "Cuartos: " << endl
       << primerCuarto << endl
       << cuartoCuarto << endl;
  if (primerCuarto[0] != 1 || cuartoCuarto[1] != 8 || cuartoCuarto[3] != 10)
    return false;

  /* Partición par con desviación: 7, 3 y 3, 7 */
  auto primerMedioCompuesto = arreglo.partir(2, 0, 2);
  auto segundoMedioCompuesto = arreglo.partir(2, 1, 2);
  cout << "Medio compuesto de 10 (+2): " << endl
       << primerMedioCompuesto << endl
       << segundoMedioCompuesto << endl;
  if (primerMedioCompuesto != Arreglo<int>{1, 2, 3, 4, 5, 6, 7} ||
    segundoMedioCompuesto != Arreglo<int>{8, 9, 10})
    return false;
  auto primerMedioCompuestoDos = arreglo.partir(2, 0, -2);
  auto segundoMedioCompuestoDos = arreglo.partir(2, 1, -2);
  cout << "Medio compuesto de 10 (-2): " << endl
       << primerMedioCompuestoDos << endl
       << segundoMedioCompuestoDos << endl;
  if (primerMedioCompuestoDos != Arreglo<int>{1, 2, 3} ||
    segundoMedioCompuestoDos != Arreglo<int>{4, 5, 6, 7, 8, 9, 10})
    return false;

  /* Partición impar con desviación: 5, 2 y 1, 6 */
  Arreglo<int> arregloDos (7);
  for (int i = 0; i < 7; i++)
    arregloDos.colocar(i, i + 1);

  auto primerMedioCompuestoTres = arregloDos.partir(2, 0, 2);
  auto segundoMedioCompuestoTres = arregloDos.partir(2, 1, 2);
  cout << "Medio compuesto de 7 (+2): " << endl
       << primerMedioCompuestoTres << endl
       << segundoMedioCompuestoTres << endl;
  if (primerMedioCompuestoTres != Arreglo<int>{1, 2, 3, 4, 5} ||
    segundoMedioCompuestoTres != Arreglo<int>{6, 7})
    return false;
  auto primerMedioCompuestoCuatro = arregloDos.partir(2, 0, -2);
  auto segundoMedioCompuestoCuatro = arregloDos.partir(2, 1, -2);
  cout << "Medio compuesto de 7 (-2): " << endl
       << primerMedioCompuestoCuatro << endl
       << segundoMedioCompuestoCuatro << endl;
  if (primerMedioCompuestoCuatro != Arreglo<int>{1} ||
    segundoMedioCompuestoCuatro != Arreglo<int>{2, 3, 4, 5, 6, 7})
    return false;

  return true;
}

/**
 * Prueba la operación de concatenación de los arreglos.
 *
 * \return Estado de la prueba.
 */

bool ArregloPrueba::probarConcatenacion()
{
  Arreglo<int> arregloUno (5), arregloDos (5), arregloTres (5);
  for (int i = 0; i < 5; i++)
  {
    arregloUno.colocar(i, i + 1);
    arregloDos.colocar(i, (i + 1) + 5);
    arregloTres.colocar(i, (i + 1) + 10);
  }
  auto compuestoUno = arregloUno + arregloDos;
  auto compuestoDos = arregloUno + arregloDos + arregloTres;
  cout << compuestoUno << endl;
  cout << compuestoDos << endl;
  if (compuestoUno[1] != 2 || compuestoUno[7] != 8 || compuestoDos[13] != 14)
    return false;
  return true;
}

/**
 * Prueba el funcionamiento de todos los constructores (por tamaño, por lista
 * de inicialización, por copia, por movimiento) y de las operaciones
 * de asignación (de copia y de movimiento).
 *
 * \return Estado de la prueba.
 *
 * \todo Probar los dos criterios de copiado (17.5.1.3).
 */

bool ArregloPrueba::probarConstructores()
{
  /* Lista de inicialización. */
  Arreglo<int> arreglo {1, 2, 3, 4};
  cout << "Construcción por lista de inicialización:" << endl
       << arreglo << endl;
  if (arreglo[0] != 1 || arreglo.obtenerNumeroDeElementos() != 4)
    return false;

  /* Construcción por copia */
  auto arregloCopia (arreglo);
  cout << "Construcción por copia:" << endl
       << arregloCopia << endl;
  if (arregloCopia[2] != 3 || arregloCopia[3] != 4)
    return false;

  /* Asignación por copia */
  arreglo.colocar(1, 50);
  cout << "Asignación por copia (antes):" << endl
       << arregloCopia << endl;
  if (arregloCopia[1] == 50)
    return false;
  arregloCopia = arreglo;
  cout << "Asignación por copia (después):" << endl
       << arregloCopia << endl;
  if (arregloCopia[1] != 50)
    return false;

  /* Construcción por movimiento. */
  auto arregloMovimiento (move(arreglo));
  cout << "Construcción por movimiento:" << endl
       << arregloMovimiento << endl;
   if (arregloMovimiento[1] != 50
     || arregloMovimiento.obtenerNumeroDeElementos() != 4)
     return false;

  return true;
}

/**
 * Prueba las operaciones de comparación de los arreglos.
 *
 * \todo ¿Por qué el constructor de los arreglos tiene que ser explícito?
 *
 * \return Estado de la prueba.
 */

bool ArregloPrueba::probarOperadoresComparacion()
{
  Arreglo<int> arreglo (5);
  for (int i = 0; i < 5; i++)
    arreglo.colocar(i, i + 1);

  /* Comparación de igualdad */
  cout << "Prueba de igualdad: " << endl
       << (arreglo == Arreglo<int>{1, 2, 3, 4, 5}) << endl
       << (arreglo == Arreglo<int>{1, 2, 4, 5}) << endl;
  if ((arreglo == Arreglo<int>{1, 2, 3, 4, 5}) != true ||
    (arreglo == Arreglo<int>{1, 2, 4, 5}) != false)
    return false;

  /* Comparación de desigualdad */
  cout << "Prueba de desigualdad: " << endl
       << (arreglo != Arreglo<int>{5}) << endl
       << (arreglo != Arreglo<int>{1, 2, 3, 4, 5}) << endl;
  if ((arreglo != Arreglo<int>{5}) != true ||
    (arreglo != Arreglo<int>{1, 2, 3, 4, 5}) != false)
    return false;

  return true;
}

/**
 * Prueba las funciones para pasar de número a arreglo y de arreglo a número.
 *
 * \return Estado de la prueba.
 */

bool ArregloPrueba::probarConversionesNumericas()
{
  /* Prueba decimal. */
  int numeroUno {6547};
  Arreglo<int> arregloUno = Arreglo<int>::convertirAArreglo(numeroUno, 10, 6);
  int copiaUno = arregloUno.convertirANumero(10);
  cout << "Prueba decimal: " << endl
       << "Número, " << numeroUno << endl
       << "Arreglo, " << arregloUno << endl
       << "Copia, " << copiaUno << endl;
  if (arregloUno != Arreglo<int>{7, 4, 5, 6, 0, 0} || numeroUno != copiaUno)
    return false;

  /* Prueba binaria. */
  int numeroDos {10};
  Arreglo<int> arregloDos = Arreglo<int>::convertirAArreglo(numeroDos, 2, 4);
  int copiaDos = arregloDos.convertirANumero(2);
  cout << "Prueba binaria: " << endl
       << "Número, " << numeroDos << endl
       << "Arreglo, " << arregloDos << endl
       << "Copia, " << copiaDos << endl;
  if (arregloDos != Arreglo<int>{0, 1, 0, 1} || numeroDos != copiaDos)
    return false;

  return true;
}