#
# Construcción de programa de pruebas de implementaciones.
# Proyecto Lovelace.
#

PROGRAMA_PRINCIPAL	:= programa_de_pruebas
CARPETA_BINARIOS		:= binarios
CARPETA_OBJETOS			:= objetos
LISTA_DE_FUENTES		:= programa_de_pruebas.cpp \
											 arreglo.cpp \
											 arreglo_prueba.cpp \
											 arreglo_de_digitos.cpp \
											 intermediario_de_arreglo_de_digitos.cpp \
											 arreglo_de_digitos_prueba.cpp \
											 utilidades_matematicas_prueba.cpp \
											 codificador.cpp \
											 codificador_prueba.cpp \
											 prueba.cpp \
											 conjunto_de_pruebas.cpp
LISTA_DE_OBJETOS		:= $(addprefix $(CARPETA_OBJETOS)/, \
												 $(LISTA_DE_FUENTES:.cpp=.o))
BANDERAS_GENERALES	:= -std=c++14 -Wall -Werror -Wfatal-errors -ggdb
BANDERAS_DE_LIGADO	:= -lcryptopp

$(CARPETA_BINARIOS)/$(PROGRAMA_PRINCIPAL): $(LISTA_DE_OBJETOS)
	mkdir -p $(CARPETA_BINARIOS)
	$(CXX) $(BANDERAS_GENERALES) $(LISTA_DE_OBJETOS) $(BANDERAS_DE_LIGADO) -o $@

$(CARPETA_OBJETOS)/programa_de_pruebas.o: programa_de_pruebas.cpp
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/arreglo.o: arreglo.cpp cabeceras/arreglo.hh
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/arreglo_prueba.o: arreglo_prueba.cpp cabeceras/arreglo.hh
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/arreglo_de_digitos.o: arreglo_de_digitos.cpp \
	cabeceras/arreglo_de_digitos.hh
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/arreglo_de_digitos_prueba.o: arreglo_de_digitos_prueba.cpp \
	cabeceras/arreglo_de_digitos_prueba.hh
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/utilidades_matematicas_prueba.o: \
	utilidades_matematicas_prueba.cpp cabeceras/utilidades_matematicas.hh
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/intermediario_de_arreglo_de_digitos.o: \
	intermediario_de_arreglo_de_digitos.cpp \
	cabeceras/intermediario_de_arreglo_de_digitos.hh
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/codificador.o: codificador.cpp \
	cabeceras/codificador.hh
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/codificador_prueba.o: codificador_prueba.cpp \
	cabeceras/codificador_prueba.hh
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/prueba.o: prueba.cpp
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

$(CARPETA_OBJETOS)/conjunto_de_pruebas.o: conjunto_de_pruebas.cpp
	mkdir -p $(CARPETA_OBJETOS)
	$(CXX) $(BANDERAS_GENERALES) -c $< -o $@

correr: $(CARPETA_BINARIOS)/$(PROGRAMA_PRINCIPAL)
	./$<

limpiar:
	rm -Rvf $(CARPETA_BINARIOS) $(CARPETA_OBJETOS)
