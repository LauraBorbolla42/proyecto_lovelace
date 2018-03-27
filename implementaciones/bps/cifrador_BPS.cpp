/**
 * \file
 * \brief Implementación de la clase del cifrador BPS.
 *
 * Proyecto Lovelace.
 */

#include "cabeceras/cifrador_BPS.hh"

#include "cabeceras/utilidades.hh"
#include "cabeceras/codificador.hh"
#include "cabeceras/cifrador_de_ronda.hh"
#include "cabeceras/cifrador_BC.hh"
#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <gmpxx.h>
#include <gmp.h>

#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>

using namespace std;
using namespace CryptoPP;
using namespace Implementaciones;

/* ========================================================================= */

CifradorBPS::CifradorBPS(vector<char> alfabeto, unsigned int numRondas)
{
  mAlfabeto    = alfabeto;
  mNumRondas   = numRondas;
  mCodificador.colocarAlfabeto(mAlfabeto);
  mTipoCifrador = CifradorDeRonda::BANDERA_AES;
}

/* ========================================================================= */

CifradorBPS::CifradorBPS(vector<char> alfabeto, unsigned int numRondas,
                                                 unsigned int cifrador)
{
  mAlfabeto    = alfabeto;
  mNumRondas   = numRondas;
  mCodificador.colocarAlfabeto(mAlfabeto);
  mTipoCifrador = cifrador;
}

/* ========================================================================= */

/**
 * Este método sirve para cifrar la cadena dada con la llave y el tweak dados.
 * El funcionamiento de este método es el del modo de operación del algoritmo
 * de cifrado que preserva el formato BPS.
 */

string CifradorBPS::cifrar(string mensaje, byte llave[], mpz_class tweak)
{
  CifradorBC BC;
  BC.colocarTipoCifrador(mTipoCifrador);
  int tamCifradorDeRonda = BC.obtenerCifradorDeRonda().obtenerTamBloque();

  /* Obtención del tamaño máximo de bloque del cifrador interno BC */
  unsigned int tamTotal, tamMax;
  tamTotal  = mensaje.size();
  tamMax = 2 * ((tamCifradorDeRonda - 32) * log(2)) / log(mAlfabeto.size());

  /* Configuración del cifrador interno BC */
  BC.colocarAlfabeto(mAlfabeto);
  BC.colocarTamBloque(tamMax);
  BC.colocarNumRondas(mNumRondas);

  /* En caso de que la cadena dada tenga una longitud menor al tamaño máximo
  del cifrador BC, simplemente se cifra la cadena con BC */
  if(tamTotal <= tamMax)
  {
    BC.colocarTamBloque(tamTotal);
    return BC.cifrar(mensaje, llave, tweak);
  }

  string bloqueA     {""};
  string salida      {""};
  string salidaFinal {""};

  unsigned int numBloques = tamTotal / tamMax;
  unsigned int tamUltimoBloque = tamTotal % tamMax;
  mpz_class contadorU = 0;
  mpz_class tweakU = 0;

  /* En caso de que la cadena dada tenga una longitud mayor al tamaño máximo
  del cifrador BC, se van cifrando bloques x de longitud igual a la longitud
  máxima de BC, donde x es igual a la suma modular de un bloque i con un
  bloque i-1 */
  BC.colocarTamBloque(tamMax);
  for (unsigned int i = 0; i < numBloques; i++)
  {
    /* Xor del contador u = 2^16 + 2^48 con el tweak que entrara
    al cifrador BC */
    contadorU = i;
    contadorU = (contadorU << 16) + (contadorU << 48);
    tweakU = tweak^contadorU;

    /* Cifrado de la suma modular del bloque i con el bloque i-1 */
    bloqueA = mensaje.substr(i * tamMax, tamMax);
    salida = BC.cifrar(mCodificador.sumaMod(bloqueA, salida), llave, tweakU);
    salidaFinal += salida;
  }

  /* En caso de que la cadena dada tenga una longitud que no es múltiplo
  del tamaño máximo de BC, el último bloque se cifra configurando BC
  a su longitud */
  if(tamUltimoBloque != 0)
  {
    /* Xor del contador u = 2^16 + 2^48 con el tweak que entrará
    al cifrador BC */
    BC.colocarTamBloque(tamUltimoBloque);
    contadorU = numBloques;
    contadorU = (contadorU << 16) + (contadorU << 48);
    tweakU = tweak ^ contadorU;

    /* Cifrado de la suma modular del bloque i con el bloque i-1 */
    bloqueA = mensaje.substr(tamTotal - tamUltimoBloque, tamUltimoBloque);
    salida = BC.cifrar(mCodificador.sumaMod(bloqueA, salida), llave, tweakU);
    salidaFinal += salida;
  }

  return salidaFinal;
}

/* ========================================================================= */

/**
 * Este método sirve para descifrar la cadena dada con la llave y tweak dados.
 * El funcionamiento de este método es el del modo de operación del algoritmo
 * de cifrado que preserva el formato BPS.
 */

string CifradorBPS::descifrar(string mensaje, byte llave[], mpz_class tweak)
{
  CifradorBC BC;
  BC.colocarTipoCifrador(mTipoCifrador);
  int tamCifradorDeRonda = BC.obtenerCifradorDeRonda().obtenerTamBloque();

  /* Obtención del tamaño máximo de bloque del cifrador interno BC */
  unsigned int tamTotal, tamMax;
  tamTotal = mensaje.size();
  tamMax = 2 * ((tamCifradorDeRonda - 32) * log(2)) / log(mAlfabeto.size());

  /* Configuración del cifrador interno BC */
  BC.colocarAlfabeto(mAlfabeto);
  BC.colocarTamBloque(tamMax);
  BC.colocarNumRondas(mNumRondas);

  /* En caso de que la cadena dada tenga una longitud menor al tamaño máximo
  del cifrador BC, simplemente se descifra la cadena con BC */
  if(tamTotal <= tamMax)
  {
    BC.colocarTamBloque(tamTotal);
    return BC.descifrar(mensaje, llave, tweak);
  }

  string bloqueA     {""};
  string bloqueB     {""};
  string salida      {""};
  string salidaFinal {""};

  unsigned int numBloques = tamTotal / tamMax;
  unsigned int tamUltimoBloque = tamTotal % tamMax;
  mpz_class contadorU = 0;
  mpz_class tweakU = 0;

  /* En caso de que la cadena dada tenga una longitud que no es múltiplo
  del tamaño máximo de BC, el último bloque se descifra configurando BC
  a su longitud */
  if(tamUltimoBloque != 0)
  {
    /* Xor del contador u = 2^16 + 2^48 con el tweak que entrara
    al cifrador BC */
    BC.colocarTamBloque(tamUltimoBloque);
    contadorU = numBloques;
    contadorU = (contadorU << 16) + (contadorU << 48);
    tweakU = tweak^contadorU;

    /* Obtención del bloque i e i-1 */
    bloqueB = mensaje.substr(tamTotal - tamUltimoBloque, tamUltimoBloque);
    bloqueA = mensaje.substr((numBloques - 1) * tamMax, tamMax);

    /* Resta modular del resultado de decifrar el bloque i, menor el bloque i-1 */
    salida = mCodificador.restaMod(BC.descifrar(bloqueB, llave, tweakU), bloqueA);
    salidaFinal = salida + salidaFinal;
  }

  /* En caso de que la cadena dada tenga una longitud mayor al tamaño máximo
  del cifrador BC, se van obteniendo bloques descifrados x de longitud igual
  a la longitud máxima de BC, donde x es igual a la resta modular del
  resultado de descifrar un bloque un bloque i con un bloque i-1 */
  BC.colocarTamBloque(tamMax);
  for(int i = numBloques - 1; i > 0; i--)
  {
    /* Xor del contador u = 2^16 + 2^48 con el tweak que entrara
    al cifrador BC */
    contadorU = i;
    contadorU = (contadorU << 16) + (contadorU << 48);
    tweakU = tweak ^ contadorU;

    /* Obtención del bloque i e i-1 */
    bloqueB = mensaje.substr(i * tamMax, tamMax);
    bloqueA = mensaje.substr((i - 1) * tamMax, tamMax);

    /* Resta modular del resultado de decifrar el bloque i, menor el bloque i-1 */
    salida = mCodificador.restaMod(BC.descifrar(bloqueB, llave, tweakU), bloqueA);
    salidaFinal = salida + salidaFinal;
  }

  /* Descifrado del primer bloque en caso de que la cadena dada fuese de una
  longitud mayor al tamaño máximo del cifrador BC */
  bloqueA = mensaje.substr(0, tamMax);
  salida = BC.descifrar(bloqueA, llave, tweak);
  salidaFinal = salida + salidaFinal;

  return salidaFinal;
}

/* ========================================================================= */

vector<char> CifradorBPS::obtenerAlfabeto()
{
  return mAlfabeto;
}

/* ========================================================================= */

unsigned int CifradorBPS::obtenerNumRondas()
{
  return mNumRondas;
}

/* ========================================================================= */

void CifradorBPS::colocarAlfabeto(vector<char> alfabeto)
{
  mAlfabeto = alfabeto;
  mCodificador.colocarAlfabeto(mAlfabeto);
}

/* ========================================================================= */

void CifradorBPS::colocarNumRondas(unsigned int numRondas)
{
  mNumRondas = numRondas;
}

/* ========================================================================= */

void CifradorBPS::colocarTipoCifrador(unsigned int tipo)
{
  mTipoCifrador = tipo;
}
