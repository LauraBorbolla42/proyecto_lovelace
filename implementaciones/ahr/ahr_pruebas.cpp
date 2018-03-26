#include "cabeceras/ahr.hh"
#include <iostream>

using namespace std;
using namespace Implementaciones;

int main()
{
  unsigned char llave256[32] = { 0xc4,0x7b,0x02,0x94,0xdb,0xbb,0xee,0x0f,
                      0xec,0x47,0x57,0xf2,0x2f,0xfe,0xee,0x35,0x87,0xca,0x47,
                      0x30,0xc3,0xd3,0x3b,0x69,0x1d,0xf3,0x8b,0xab,0x07,0x6b,
                      0xc5,0x58};

  CDV* accesoADatos = new AccesoMySQL {};

  AHR tokenizador = AHR(7, accesoADatos);
  tokenizador.tokenizarHibridamente(llave256, 5256981745955292, 5256781745900292);
  cout << tokenizador.obtenerToken() << endl;
}
