#
# Tareas genéricas del repositorio.
# Proyecto Lovelace.
#

sitio_local:
	bundle exec jekyll serve

documentación_doxygen:
	doxygen .doxygen


# Toma de capturas de pantalla para reporte técnico.
# OJO: evidentemente, para que esto funcione, el sitio debe
# estar disponible en el dominio dado (el servidor de pruebas
# de django debe de estar corriendo).

# TODO:
# Agregar archivos de sass a las dependencias de los objetivos.

RUTA_BASE            := documentos_entregables/reporte_tecnico/contenidos/

CARPETA_IMAGENES     := $(RUTA_BASE)analisis_y_disenio_api_web/analisis/capturas
CARPETA_FUENTES      := sistema_tokenizador/archivos_web
LISTA_DE_FUENTES     := inicio_1920x1080.png \
	                      documentacion_1920x1080.png \
												iniciar_sesion_1920x1080.png \
												registrar_cliente_1920x1080.png \
												control_1920x1080.png \
												administracion_1920x1080.png \
												aviso_de_correo_1920x1080.png \
												aviso_de_espera_para_aprobacion_1920x1080.png \
												aviso_de_verificacion_exitosa_1920x1080.png \
												aviso_de_expiracion_de_vinculo_1920x1080.png
LISTA_DE_OBJETOS     := $(addprefix $(CARPETA_IMAGENES)/, \
	                      $(LISTA_DE_FUENTES))

LIB_CARPETA_IMAGENES := $(RUTA_BASE)analisis_y_disenio_tienda/analisis/capturas
LIB_CARPETA_FUENTES  := tienda/archivos_web
LIB_LISTA_DE_FUENTES := inicio_1920x1080.png \
											  detalles_1920x1080.png \
												iniciar_sesion_1920x1080.png \
												formulario_registro_1920x1080.png \
												aviso_correo_1920x1080.png \
												carrito_1920x1080.png \
												cuenta_1920x1080.png \
												formulario_tarjeta_1920x1080.png \
												formulario_direccion_1920x1080.png \
												formulario_actualizacion_de_usuario_1920x1080.png \
												forma_de_pago_1920x1080.png \
												direccion_de_entrega_1920x1080.png \
												resumen_de_compra_1920x1080.png
LIB_LISTA_DE_OBJETOS := $(addprefix $(LIB_CARPETA_IMAGENES)/, \
	                      $(LIB_LISTA_DE_FUENTES))

DEPENDENCIAS_COMUNES := index.html \
												js/configuraciones/tema.configuracion.js
SCRIPTS_CAPTURAS     := utilidades/capturas_selenium
DOMINIO 						 := http://127.0.0.1:8080
LIB_DOMINIO 				 := http://127.0.0.1:8081

todo: toma_de_capturas lib_toma_de_capturas
	@echo "Toma de capturas lista"

# Las fuentes están todas en los archivos web
VPATH := $(CARPETA_FUENTES)

toma_de_capturas: $(LISTA_DE_OBJETOS)
	@echo "Toma de capturas de sistema tokenizador lista"

$(CARPETA_IMAGENES)/inicio_1920x1080.png: \
		html/inicio.html \
		$(DEPENDENCIAS_COMUNES)
	python $(SCRIPTS_CAPTURAS)/inicio.py $(DOMINIO)/ $@

$(CARPETA_IMAGENES)/documentacion_1920x1080.png: \
		html/documentacion.html \
		$(DEPENDENCIAS_COMUNES)
	python $(SCRIPTS_CAPTURAS)/documentacion.py $(DOMINIO)/documentación $@

$(CARPETA_IMAGENES)/iniciar_sesion_1920x1080.png: \
		html/ventanas/iniciar_sesion.ventana.html \
		$(DEPENDENCIAS_COMUNES)
	python $(SCRIPTS_CAPTURAS)/iniciar_sesion.py $(DOMINIO)/ $@

$(CARPETA_IMAGENES)/registrar_cliente_1920x1080.png: \
		html/ventanas/operar_cliente.ventana.html \
		$(DEPENDENCIAS_COMUNES)
	python $(SCRIPTS_CAPTURAS)/registrar_cliente.py $(DOMINIO)/ $@

$(CARPETA_IMAGENES)/control_1920x1080.png: \
		html/control.html \
		$(DEPENDENCIAS_COMUNES)
	python $(SCRIPTS_CAPTURAS)/control.py \
		$(DOMINIO)/ $@ $(DOMINIO)/control

$(CARPETA_IMAGENES)/administracion_1920x1080.png: \
		html/administracion.html \
		$(DEPENDENCIAS_COMUNES)
	python $(SCRIPTS_CAPTURAS)/administracion.py \
		$(DOMINIO)/ $@ $(DOMINIO)/administración

$(CARPETA_IMAGENES)/aviso_de_correo_1920x1080.png: \
		html/ventanas/operar_cliente.ventana.html \
		js/controladores/secundarios/operar_cliente.controlador.js \
		$(DEPENDENCIAS_COMUNES)
	python $(SCRIPTS_CAPTURAS)/aviso_de_correo.py \
		$(DOMINIO)/ $@

$(CARPETA_IMAGENES)/aviso_de_espera_para_aprobacion_1920x1080.png: \
		html/inicio.html \
		$(DEPENDENCIAS_COMUNES)
	python $(SCRIPTS_CAPTURAS)/inicio.py $(DOMINIO)/?correo_verificado $@

$(CARPETA_IMAGENES)/aviso_de_verificacion_exitosa_1920x1080.png: \
		html/inicio.html \
		$(DEPENDENCIAS_COMUNES)
	python $(SCRIPTS_CAPTURAS)/inicio.py $(DOMINIO)/?nuevo_correo_verificado $@

$(CARPETA_IMAGENES)/aviso_de_expiracion_de_vinculo_1920x1080.png: \
		html/inicio.html \
		$(DEPENDENCIAS_COMUNES)
	python $(SCRIPTS_CAPTURAS)/inicio.py $(DOMINIO)/?correo_no_verificado $@

modelo_de_datos:
	python administrar_sistema_tokenizador.py graph_models -g -o \
	$(RUTA_BASE)analisis_y_disenio_api_web/analisis/diagramas/modelo_de_datos.png \
	general programa_tokenizador


# Capturas para el caso de prueba: librería

# Las fuentes están todas en los archivos web
VPATH := $(LIB_CARPETA_FUENTES)

lib_toma_de_capturas: $(LIB_LISTA_DE_OBJETOS)
	@echo "Toma de capturas de librería lista"

$(LIB_CARPETA_IMAGENES)/inicio_1920x1080.png: \
		html/inicio.html
	python $(SCRIPTS_CAPTURAS)/inicio.py $(LIB_DOMINIO)/ $@

$(LIB_CARPETA_IMAGENES)/detalles_1920x1080.png: \
		html/libro.html
	python $(SCRIPTS_CAPTURAS)/inicio.py $(LIB_DOMINIO)/libro/6 $@

$(LIB_CARPETA_IMAGENES)/iniciar_sesion_1920x1080.png: \
		html/ventanas/iniciar_sesion.ventana.html
	python $(SCRIPTS_CAPTURAS)/iniciar_sesion.py $(LIB_DOMINIO)/ $@

$(LIB_CARPETA_IMAGENES)/formulario_registro_1920x1080.png: \
		html/ventanas/operar_usuario.ventana.html
	python $(SCRIPTS_CAPTURAS)/libreria_registrar_usuario.py $(LIB_DOMINIO)/ $@

$(LIB_CARPETA_IMAGENES)/aviso_correo_1920x1080.png: \
		html/ventanas/operar_usuario.ventana.html \
		js/controladores/secundarios/operar_usuario.controlador.js
	python $(SCRIPTS_CAPTURAS)/libreria_aviso_de_correo.py $(LIB_DOMINIO)/ $@

$(LIB_CARPETA_IMAGENES)/carrito_1920x1080.png: \
		html/inicio.html
	python $(SCRIPTS_CAPTURAS)/libreria_carrito.py \
	$(LIB_DOMINIO)/ $@ $(LIB_DOMINIO)/carrito

$(LIB_CARPETA_IMAGENES)/cuenta_1920x1080.png: \
		html/cuenta.html
	python $(SCRIPTS_CAPTURAS)/libreria_cuenta.py \
		$(LIB_DOMINIO)/ $@ $(LIB_DOMINIO)/cuenta

$(LIB_CARPETA_IMAGENES)/formulario_tarjeta_1920x1080.png: \
		html/cuenta.html html/ventanas/tarjeta.ventana.html
	python $(SCRIPTS_CAPTURAS)/libreria_agregar_tarjeta.py \
		$(LIB_DOMINIO)/ $@ $(LIB_DOMINIO)/cuenta

$(LIB_CARPETA_IMAGENES)/formulario_direccion_1920x1080.png: \
		html/cuenta.html html/ventanas/direccion.ventana.html
	python $(SCRIPTS_CAPTURAS)/libreria_agregar_direccion.py \
		$(LIB_DOMINIO)/ $@ $(LIB_DOMINIO)/cuenta

$(LIB_CARPETA_IMAGENES)/formulario_actualizacion_de_usuario_1920x1080.png: \
		html/cuenta.html html/ventanas/operar_usuario.ventana.html
	python $(SCRIPTS_CAPTURAS)/libreria_editar_usuario.py \
		$(LIB_DOMINIO)/ $@ $(LIB_DOMINIO)/cuenta

$(LIB_CARPETA_IMAGENES)/forma_de_pago_1920x1080.png: \
		html/carrito.html
	python $(SCRIPTS_CAPTURAS)/libreria_forma_de_pago.py \
	$(LIB_DOMINIO)/ $@ $(LIB_DOMINIO)/carrito

$(LIB_CARPETA_IMAGENES)/direccion_de_entrega_1920x1080.png: \
		html/ventanas/finalizar_compra.ventana.html \
		js/controladores/secundarios/finalizar_compra.controlador.js
	python $(SCRIPTS_CAPTURAS)/libreria_direccion_de_entrega.py \
	$(LIB_DOMINIO)/ $@ $(LIB_DOMINIO)/cuenta

$(LIB_CARPETA_IMAGENES)/resumen_de_compra_1920x1080.png: \
		html/ventanas/finalizar_compra.ventana.html \
		js/controladores/secundarios/finalizar_compra.controlador.js
	python $(SCRIPTS_CAPTURAS)/libreria_resumen_de_compra.py \
	$(LIB_DOMINIO)/ $@ $(LIB_DOMINIO)/cuenta

# Traducción de archivos de latex a archivos de html
FUENTES   := $(RUTA_BASE)apendices/paginas_estaticas/
PRODUCTOS := $(RUTA_BASE)analisis_y_disenio_api_web/analisis/paginas_estaticas/

paginas_estaticas: \
		paginas_estaticas_inicio \
		$(PRODUCTOS)/documentacion.producto.html
	@echo "Traducción de páginas estáticas lista"

paginas_estaticas_inicio: \
		$(PRODUCTOS)/inicio/inicio.producto.html \
		$(PRODUCTOS)/inicio/ffx.producto.html \
		$(PRODUCTOS)/inicio/bps.producto.html \
		$(PRODUCTOS)/inicio/tkr.producto.html \
		$(PRODUCTOS)/inicio/ahr.producto.html \
		$(PRODUCTOS)/inicio/drbg.producto.html \
		$(PRODUCTOS)/inicio/comparacion.producto.html
	@echo "Traducción de páginas estáticas de inicio lista"

$(PRODUCTOS)/inicio/inicio.producto.html: $(FUENTES)/inicio/inicio.tex
	mkdir -p $(PRODUCTOS)/inicio
	pandoc \
		--filter pandoc-latex-levelup	\
		-f latex \
		-t html \
		-o $@ \
		$<

$(PRODUCTOS)/inicio/ffx.producto.html: $(FUENTES)/inicio/ffx.tex
	mkdir -p $(PRODUCTOS)/inicio
	pandoc \
		--filter pandoc-latex-levelup	\
		-f latex \
		-t html \
		-o $@ \
		$<

$(PRODUCTOS)/inicio/bps.producto.html: $(FUENTES)/inicio/bps.tex
	mkdir -p $(PRODUCTOS)/inicio
	pandoc \
		--filter pandoc-latex-levelup	\
		-f latex \
		-t html \
		-o $@ \
		$<

$(PRODUCTOS)/inicio/tkr.producto.html: $(FUENTES)/inicio/tkr.tex
	mkdir -p $(PRODUCTOS)/inicio
	pandoc \
		--filter pandoc-latex-levelup	\
		-f latex \
		-t html \
		-o $@ \
		$<

$(PRODUCTOS)/inicio/ahr.producto.html: $(FUENTES)/inicio/ahr.tex
	mkdir -p $(PRODUCTOS)/inicio
	pandoc \
		--filter pandoc-latex-levelup	\
		-f latex \
		-t html \
		-o $@ \
		$<

$(PRODUCTOS)/inicio/drbg.producto.html: $(FUENTES)/inicio/drbg.tex
	mkdir -p $(PRODUCTOS)/inicio
	pandoc \
		--filter pandoc-latex-levelup	\
		-f latex \
		-t html \
		-o $@ \
		$<

$(PRODUCTOS)/inicio/comparacion.producto.html: $(FUENTES)/inicio/comparacion.tex
	mkdir -p $(PRODUCTOS)/inicio
	pandoc \
		--filter pandoc-latex-levelup	\
		-f latex \
		-t html \
		-o $@ \
		$<

$(PRODUCTOS)/documentacion.producto.html: $(FUENTES)/documentacion.tex
	mkdir -p $(PRODUCTOS)
	pandoc \
		--filter pandoc-latex-levelup	\
		-f latex \
		-t html \
		-o $@ \
		$<
