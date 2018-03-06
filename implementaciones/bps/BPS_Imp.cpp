/**
 * Proyecto Lovelace.
 * Cifrador BPS.
 */

#include "cabeceras/cifrador_BPS.hh"

#include <iostream>
#include <iomanip>
#include <vector>
#include <gmpxx.h>
#include <gmp.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>

using namespace std;
using namespace CryptoPP;

int main(int argc, char* argv[]) {
  
  string tipoalfa = argv[1];
  string claro = argv[2];

  byte llave[AES::DEFAULT_KEYLENGTH];            /* Generacion de la llave */
  memset(llave, 0x23, AES::DEFAULT_KEYLENGTH);
  mpz_class tweak = 0xF0F0F0F01F1F1F1F;          /* Generacion del tweak */

  unsigned int num_de_rondas = 8;                /* Numero de rondas */

  vector<char> alfabeto;                         /* Creacion del alfabeto */
  
  if(tipoalfa == "NUMERICO"){
    for(int i=0; i<10; i++)
      alfabeto.push_back('0' + i);
  }
  else if(tipoalfa == "ALFABETICO_MINUSCULAS")
  {
    for(int i=0; i<26; i++)
      alfabeto.push_back('a' + i);
  }
  else if(tipoalfa == "ALFABETICO_MAYUSCULAS")
  {
    for(int i=0; i<26; i++)
      alfabeto.push_back('A' + i);
  }
  else if(tipoalfa == "ALFABETICO")
  {
    for(int i=0; i<26; i++)
    {
      alfabeto.push_back('a' + i);
      alfabeto.push_back('A' + i);
    }
  }
  else if(tipoalfa == "ALFANUMERICO_MINUSCULAS")
  {
    for(int i=0; i<10; i++)
      alfabeto.push_back('0' + i);
    for(int i=0; i<26; i++)
      alfabeto.push_back('a' + i);
  }
  else if(tipoalfa == "ALFANUMERICO_MAYUSCULAS")
  {
    for(int i=0; i<10; i++)
      alfabeto.push_back('0' + i);
    for(int i=0; i<26; i++)
      alfabeto.push_back('A' + i);
  }
  else if(tipoalfa == "ALFANUMERICO")
  {
    for(int i=0; i<10; i++)
      alfabeto.push_back('0' + i);
    for(int i=0; i<26; i++)
    {
      alfabeto.push_back('a' + i);
      alfabeto.push_back('A' + i);
    }
  }
  else 
    cout << "Especifica uno de los siguentes alfabetos" << endl
         << "NUMERICO"                << endl
         << "ALFABETICO"              << endl
         << "ALFANUMERICO"            << endl
         << "ALFABETICO_MINUSCULAS"   << endl
         << "ALFABETICO_MAYUSCULAS"   << endl
         << "ALFANUMERICO_MINUSCULAS" << endl
         << "ALFANUMERICO_MAYUSCULAS" << endl;

  string cifrado, descifrado;
  CifradorBPS BPS(alfabeto,num_de_rondas);

  cout << "==============================================" << endl;
  cout << "Texto claro:      " << claro << endl;
  cout << "==============================================" << endl;
  cifrado = BPS.cifrar(claro, llave, tweak);
  cout << "Texto cifrado:    " << cifrado << endl;
  cout << "==============================================" << endl;
  descifrado = BPS.descifrar(cifrado, llave, tweak);
  cout << "Texto descifrado: " << descifrado << endl;
  cout << "==============================================" << endl;

  return 0;
}