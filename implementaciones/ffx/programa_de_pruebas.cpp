/**
 * \file
 * \brief Archivo principal de pruebas de ffx y componentes.
 * Proyecto Lovelace.
 */

#include "cabeceras/arreglo_prueba.hh"
#include <iostream>
#include <vector>

using namespace ImplementacionesPruebas;
using namespace std;

/**
 * \brief Programa de prueba de FFX y derivados.
 *
 * \return Estado de las pruebas.
 */

int main()
{
  vector<Prueba> clasesDePrueba
  {
    ArregloPrueba {}
  };
  for (auto claseDePrueba : clasesDePrueba)
    if (!claseDePrueba.probar())
      exit(-1);
  exit(0);
}
