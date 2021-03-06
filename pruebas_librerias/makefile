#
# Construcción de programa de pruebas de librerías.
# Proyecto Lovelace.
#

PROGRAMA_PRINCIPAL	:= programa_de_pruebas
CARPETA_BINARIOS		:= binarios
CARPETA_OBJETOS			:= objetos
LISTA_DE_FUENTES		:= programa_de_pruebas.cpp \
											 utilidades/prueba.cpp \
											 utilidades/conjunto_de_pruebas.cpp \
											 cryptopp/prueba_AES.cpp \
											 cryptopp/prueba_salsa20.cpp \
											 cryptopp/prueba_SHA256.cpp \
											 cryptopp/prueba_CBC_MAC.cpp
LISTA_DE_OBJETOS		:= $(addprefix $(CARPETA_OBJETOS)/, \
												 $(LISTA_DE_FUENTES:.cpp=.o))
BANDERAS_GENERALES	:= -std=c++14 -Wall -ggdb

$(CARPETA_BINARIOS)/$(PROGRAMA_PRINCIPAL): $(LISTA_DE_OBJETOS)
	mkdir -p $(CARPETA_BINARIOS)
	$(CXX) $(BANDERAS_GENERALES) $(LISTA_DE_OBJETOS) -lcryptopp -o $@

$(CARPETA_OBJETOS)/programa_de_pruebas.o: programa_de_pruebas.cpp
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/utilidades/prueba.o: ../utilidades/prueba.cpp
	mkdir -p $(CARPETA_OBJETOS)/utilidades
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/utilidades/conjunto_de_pruebas.o: \
	../utilidades/conjunto_de_pruebas.cpp
	mkdir -p $(CARPETA_OBJETOS)/utilidades
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/cryptopp/prueba_AES.o: cryptopp/prueba_AES.cpp
	mkdir -p $(CARPETA_OBJETOS)/cryptopp
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/cryptopp/prueba_salsa20.o: cryptopp/prueba_salsa20.cpp
	mkdir -p $(CARPETA_OBJETOS)/cryptopp
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/cryptopp/prueba_SHA256.o: cryptopp/prueba_SHA256.cpp
	mkdir -p $(CARPETA_OBJETOS)/cryptopp
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/cryptopp/prueba_CBC_MAC.o: cryptopp/prueba_CBC_MAC.cpp
	mkdir -p $(CARPETA_OBJETOS)/cryptopp
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/openssl/prueba_IDEA.o: openssl/prueba_IDEA.cpp
	mkdir -p $(CARPETA_OBJETOS)/openssl
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

correr: $(CARPETA_BINARIOS)/$(PROGRAMA_PRINCIPAL)
	./$<

limpiar:
	rm -Rvf $(CARPETA_BINARIOS) $(CARPETA_OBJETOS)

# make RSA ----------------------------------------------------------------

RSA: prueba_RSA $(CARPETA_OBJETOS)/cryptopp/prueba_RSA
	mkdir -p $(CARPETA_BINARIOS)
	$(CXX) $(BANDERAS_GENERALES) $(CARPETA_OBJETOS)/cryptopp/prueba_RSA \
	-lcryptopp -o $(CARPETA_BINARIOS)/$@

prueba_RSA: cryptopp/prueba_RSA.cpp
	mkdir -p $(CARPETA_OBJETOS)/cryptopp
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $(CARPETA_OBJETOS)/cryptopp/$@

correr_RSA: $(CARPETA_BINARIOS)/RSA
	./$<
