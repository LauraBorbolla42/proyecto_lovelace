"""
funciones.py Funciones para módulo de tienda,
Tienda en línea.
Proyecto Lovelace.
"""

import datetime
import django
import json
import traceback

import tienda.utilidades as utilidades
import tienda.libreria as libreria

from .models.compra import Compra
from .models.direccion import Direccion
from .models.emisor import Emisor
from .models.estado import Estado
from .models.metodo import Metodo
from .models.paquete import Paquete
from .models.tarjeta import Tarjeta
from .models.tipo_de_direccion import TipoDeDireccion
from .models.tipo_de_tarjeta import TipoDeTarjeta
from .models.usuario import Usuario
from ..tienda import negocio


################################################################################
# Gestión de sesión ############################################################
################################################################################


def usuarioDeSesion (peticion):
  """Regresa el usuario de la sesión.

  En caso de no existir, se regresa un http vacío."""

  if 'usuario' in peticion.session:
    return django.http.HttpResponse(peticion.session['usuario'])
  else:
    return django.http.HttpResponse()


def iniciarSesion (peticion):
  """Valida las credenciales dadas para iniciar una sesión.

  En caso correcto, registra al usuario en la sesión y regresa el objeto del
  usuario; en caso incorrecto, regresa un http con un código de error.

  Importante: esto funciona de forma distinta a las sesiones del sistema
  tokenizador; ahí se serializaba en la sesión una instancia directa del
  modelo; aquí no se puede hacer eso, dado que el modelo del usuario tiene la
  contraseña; en su lugar, se serializa un diccionario con el nombre y el
  correo del usuario en la sesión."""

  objetoDePeticion = json.loads(peticion.body)
  usuario = negocio.autentificar(objetoDePeticion)

  if usuario == None:
    return django.http.HttpResponse("0")

  if usuario.verificado == False:
    return django.http.HttpResponse("1")

  peticion.session['usuario'] = \
    json.dumps({
      'pk': usuario.pk,
      'nombre': usuario.nombre,
      'correo': usuario.correo})
  return django.http.HttpResponse(peticion.session['usuario'])


def cerrarSesion (peticion):
  """Elimina el objeto usuario de la sesión."""
  del peticion.session['usuario']
  return django.http.HttpResponse()


################################################################################
# Operaciones de clientes ######################################################
################################################################################


def operarUsuario (peticion):
  """Función diccionario para operaciones sobre un usuario."""

  if peticion.method == 'POST':
    return registrarUsuario(peticion)

  elif peticion.method == 'PUT':
    return actualizarUsuario(peticion)

  else:
    return django.http.HttpResponseNotAllowed()


def registrarUsuario (peticion):
  """Registra a un nuevo usuario en la base de datos.

  Registra al usuario dado en la base de datos y envía un correo con
  el vínculo de verificación

  Regresa
    0 en caso de éxito.
    1 en caso de que el usuario ya exista.                 """

  usuarioEnPeticion = json.loads(peticion.body)

  # verifica si ya existe el usuario
  if negocio.correoPreviamenteRegistrado(usuarioEnPeticion):
    return django.http.HttpResponse("1")

  # Guarda al usuario
  usuario = negocio.guardarUsuario(usuarioEnPeticion)

  return django.http.HttpResponse("0")


@utilidades.privilegiosRequeridos
def actualizarUsuario (peticion):
  """Actualiza el usuario en sesión con los datos dados.

  Actualiza los datos del usuario dado en la base de datos y
  envía un correo con el vínculo de verificación

  Regresa
    0 en caso de éxito.
    1 en caso de error.                                    """

  usuarioEnPeticion = json.loads(peticion.body)
  pk = json.loads(peticion.session['usuario'])['pk']

  # verifica si ya existe el correo
  if negocio.correoPreviamenteRegistrado(usuarioEnPeticion, pk):
    return django.http.HttpResponse("1")

  # Guarda al usuario
  banderaCorreoModificado = negocio.actualizarUsuario(usuarioEnPeticion, pk)

  if banderaCorreoModificado:
    return django.http.HttpResponse("2")
  else:
    return django.http.HttpResponse("0")


def verificarCorreo (peticion, vinculo, hrs = None):
  """ Verifica el correo asociado al vínculo dado.
      Se puede o no poner las horas que tenía de vida el vínculo,
      para ver si este ya expiró.                                   """

  usuario = Usuario.objects.get(vinculo = vinculo)
  if hrs != None and datetime.datetime.now() - usuario.fecha > \
    datetime.timedelta(hours = hrs):
    usuario.delete()
    return django.http.HttpResponseRedirect('/?correo_no_verificado')
  else:
    usuario.verificado = True
    usuario.vinculo = None
    usuario.save()
    return django.http.HttpResponseRedirect('/?correo_verificado')


def verificarCorreoDeRegistro (peticion, vinculo):
  """Verifica el correo asociado al vínculo de registro dado."""
  return verificarCorreo(peticion, vinculo, 72)


def verificarCorreoDeActualizacion (peticion, vinculo):
  """Verifica el correo asociado al vínculo de actualización dado."""
  return verificarCorreo(peticion, vinculo)


################################################################################
# Operaciones de carrito #######################################################
################################################################################

def operarCarrito (peticion):
  """Gestión del recurso del carrito."""
  if peticion.method == 'GET':
    return obtenerCarrito(peticion)
  elif peticion.method == 'POST':
    return guardarCarrito(peticion)
  else:
    return django.http.HttpResponseNotAllowed()


def obtenerCarrito (peticion):
  """Regresa el carrito en sesión.

  En caso de no existir, regresa un HTTP vacío."""
  if 'carrito' in peticion.session:
    return django.http.HttpResponse(peticion.session['carrito'])
  else:
    return django.http.HttpResponse()


def guardarCarrito (peticion):
  """Guarda la representación del carrito en las variables de sesión."""
  peticion.session['carrito'] = peticion.body.decode('utf-8')
  return django.http.HttpResponse()


@utilidades.privilegiosRequeridos
def registrarCompra (peticion):
  """Registra una compra del cliete en sesión."""

  objetoDePeticion = json.loads(peticion.body)
  carrito = json.loads(peticion.session['carrito'])
  identificador = json.loads(peticion.session['usuario'])['pk']

  # Registrar compra
  compra = Compra(
    fecha = datetime.datetime.now(),
    tarjeta = Tarjeta.objects.get(pk = objetoDePeticion['tarjeta']),
    usuario = Usuario.objects.get(pk = identificador),
    direccion = Direccion.objects.get(pk = objetoDePeticion['direccion']))
  compra.save()

  tarjeta = Tarjeta.objects.get(pk = objetoDePeticion['tarjeta'])

  # Registrar libros de compra
  for libro in carrito['libros']:
    paquete = Paquete(
      libro = libreria.models.libro.Libro.objects.get(pk = libro['pk']),
      compra = compra,
      numero = libro['cantidad'],
      precio_unitario = libro['precio'])
    paquete.save()

    # Restar de existencias
    paquete.libro.existencias -= paquete.numero;
    paquete.libro.save()

  del peticion.session['carrito']

  try:
    numeroDeTarjeta = negocio.detokenizar(tarjeta.token, str(tarjeta.metodo))
    return django.http.HttpResponse(numeroDeTarjeta)
  except Exception as error:
    print(traceback.format_exc())
    return django.http.HttpResponse()


################################################################################
# Operaciones sobre tarjetas ###################################################
################################################################################

@utilidades.privilegiosRequeridos
def obtenerTarjetas (peticion):
  """Regresa arreglo con las tarjetas del usuario en sesión.

  TODO:
  *  El campo «tarjeta», del usuario, debería de ser «tarjetas»: por algo es un
     campo muchos a muchos.
  """
  identificador = json.loads(peticion.session['usuario'])['pk']
  tarjetas = Usuario.objects.get(pk = identificador).tarjeta.filter(
    activa = True)
  return utilidades.respuestaJSON(tarjetas)


def obtenerDireccionDeTarjeta (peticion, idDeDireccion):
  """Regresa la dirección asociada a la tarjeta dada.

  TODO:
  * Agregar decorador de privilegios.
  * Validar que la dirección pedida sea del cliente en sesión."""
  direccion = Direccion.objects.get(pk = idDeDireccion)
  return utilidades.respuestaJSON(direccion)


@utilidades.privilegiosRequeridos
def operarTarjeta (peticion, idDeTarjeta = 0):
  """Gestión de tarjetas."""
  if peticion.method == 'DELETE':
    return eliminarTarjeta(peticion, idDeTarjeta)
  elif peticion.method == 'POST':
    return agregarTarjeta(peticion)
  else:
    return django.http.HttpResponseNotAllowed()


def eliminarTarjeta (peticion, idDeTarjeta):
  """Elimina la tarjeta dada.

  Pasa la tarjeta (dada por su identificador) a estado inactivo.
  TODO: validar que la tarjeta dada corresponda al usuario en sesión."""
  tarjeta = Tarjeta.objects.get(pk = idDeTarjeta)
  tarjeta.activa = False;
  tarjeta.save()

  # Operar la dirección:
  identificador = json.loads(peticion.session['usuario'])['pk']

  # Si la consulta regresa al menos una tarjeta, entonces no se hace nada
  # con la dirección.
  tarjetas = Usuario.objects.get(pk = identificador).tarjeta.filter(
    direccion = tarjeta.direccion).exclude(pk = tarjeta.pk).count()
  if tarjetas == 0:
    # La dirección no está asociada a ninguna tarjeta. Pasa a inactivo.
    tarjeta.direccion.activa = False
    tarjeta.direccion.save()

  return django.http.HttpResponse()


@utilidades.privilegiosRequeridos
def agregarTarjeta (peticion):
  """Registra un nuevo método de pago del cliente en sesión.

  Respuestas:
  * En caso de una inserción exitosa, se regresa el objeto
    de la nueva tarjeta.
  * En caso de una inserción duplicada se regresa un 1.
  * En caso de una inserción duplicada excepto por la fecha de
    expiración, se regresa un 2.
  * En caso de error al comunicarse con el sistema tokenizador,
    se regresa un 3."""

  objetoDePeticion = json.loads(peticion.body)
  identificador = json.loads(peticion.session['usuario'])['pk']

  try:
    # Buscar tarjetas iguales
    # Si no hay, se lanza excepción.
    similar = Usuario.objects.get(pk = identificador).tarjeta.get(
      terminacion = objetoDePeticion['pan'][-4:],
      tipoDeTarjeta = TipoDeTarjeta.objects.get(pk = objetoDePeticion['tipo']),
      emisor = Emisor.objects.get(pk = objetoDePeticion['emisor']),
      titular = objetoDePeticion['titular'])

    # Trayectoria alternativa 05E: La tarjeta ingresada ya ha sido almacenada.
    if similar.activa == True:
      fecha_uno = similar.expiracion
      fecha_dos = django.utils.dateparse.parse_datetime(
        objetoDePeticion['expiracion'])
      if fecha_uno.year == fecha_dos.year and \
        fecha_uno.month == fecha_dos.month:
        return django.http.HttpResponse("1")

      # Trayectoria alternativa 05H: Hay una tarjeta existente y activa
      # con los mismos datos, excepto la fecha de vencimiento.
      else:
        return django.http.HttpResponse("2")

    # Trayectoria alternativa 05F: La tarjeta ingresada ya ha sido
    # almacenada y se encuentra inactiva.
    else:
      similar.expiracion = django.utils.dateparse.parse_datetime(
        objetoDePeticion['expiracion'])
      if objetoDePeticion['direccion']['pk'] == 0:
        # Crear nueva dirección
        # TODO: Para evitar posibles duplicados, antes de insertar la nueva
        # dirección se tendría que buscar entre las direcciones inactivas.
        direccion = negocio.crearDireccion(objetoDePeticion['direccion'])
        similar.direccion = direccion;

      else:
        # Dirección existente
        similar.direccion = Direccion.objects.get(
          pk = objetoDePeticion['direccion']['pk'])

      similar.activa = True
      similar.save()
      return utilidades.respuestaJSON(similar)

  except Tarjeta.DoesNotExist:
    pass

  # Trayectoria principal
  token = None
  try:
    token = negocio.tokenizar(
      objetoDePeticion['pan'], objetoDePeticion['metodo'])
  except Exception as error:
    # Trayectoria alternativa 05G: El código HTTP de respuesta no es un 2XX.
    print(traceback.format_exc())
    return django.http.HttpResponse("3")

  direccion = None
  if objetoDePeticion['direccion']['pk'] == 0:
    direccion = negocio.crearDireccion(objetoDePeticion['direccion'])
  else:
    direccion = Direccion.objects.get(
      pk = objetoDePeticion['direccion']['pk'])

  tarjeta = Tarjeta(
    token = token,
    terminacion = objetoDePeticion['pan'][-4:],
    metodo = Metodo.objects.get(nombre = objetoDePeticion['metodo']),
    emisor = Emisor.objects.get(pk = objetoDePeticion['emisor']),
    titular = objetoDePeticion['titular'],
    direccion = direccion,
    expiracion = django.utils.dateparse.parse_datetime(
      objetoDePeticion['expiracion']),
    tipoDeTarjeta = TipoDeTarjeta.objects.get(pk = objetoDePeticion['tipo']),
    activa = True)
  tarjeta.save()

  usuario = Usuario.objects.get(pk = identificador)
  usuario.tarjeta.add(tarjeta);
  usuario.save()
  return utilidades.respuestaJSON(tarjeta)


def obtenerEmisores (peticion):
  """Regresa el catálogo de emisores."""
  emisores = Emisor.objects.all()
  return utilidades.respuestaJSON(emisores)


def obtenerMetodos (peticion):
  """Regresa el catálogo de métodos (algoritmos tokenizadores)."""
  metodos = Metodo.objects.all()
  return utilidades.respuestaJSON(metodos)


def obtenerTipos (peticion):
  """Regresa el catálogo de tipos de tarjeta."""
  tipos = TipoDeTarjeta.objects.all()
  return utilidades.respuestaJSON(tipos)


################################################################################
# Operaciones sobre direcciones ################################################
################################################################################

@utilidades.privilegiosRequeridos
def obtenerDirecciones (peticion):
  """Regresa arreglo con las direcciones del usuario en sesión.

  TODO:
  *  El campo «direccion», del usuario, debería de ser «direcciones»: por
     algo es un campo muchos a muchos.
  """
  identificador = json.loads(peticion.session['usuario'])['pk']
  direcciones = Usuario.objects.get(pk = identificador).direccion.filter(
    activa = True)
  return utilidades.respuestaJSON(direcciones)


@utilidades.privilegiosRequeridos
def operarDireccion (peticion, idDeDireccion):
  """Gestión de direcciones."""
  if peticion.method == 'DELETE':
    return eliminarDireccion(peticion, idDeDireccion)
  else:
    return django.http.HttpResponseNotAllowed()


@utilidades.privilegiosRequeridos
def eliminarDireccion (peticion, idDeDireccion):
  """Elimina la tarjeta dada.

  Pasa la tarjeta (dada por su identificador) a estado inactivo.
  TODO:
  * Validar que el identificador dado sea de una dirección del cliente en
    sesión."""
  direccion = Direccion.objects.get(pk = idDeDireccion)
  direccion.activa = False;
  direccion.save()
  return django.http.HttpResponse()


def obtenerEstados (peticion):
  """Regresa el catálogo de estados."""
  estados = Estado.objects.all()
  return utilidades.respuestaJSON(estados)

def agregarDireccionDeEntrega (peticion):
  """Agrega a la base de datos la dirección dada."""
  datosCrudos = json.loads(peticion.body)
  identificador = json.loads(peticion.session['usuario'])['pk']

  direccionNueva = Direccion(
    tipoDeDireccion = TipoDeDireccion.objects.get(
      nombre = 'Punto de entrega'),
    estado = Estado.objects.get(nombre = datosCrudos['estado']),
    municipio = datosCrudos['municipio'].lower(),
    colonia = datosCrudos['colonia'].lower(),
    calle = datosCrudos['calle'].lower(),
    numeroExterior = datosCrudos['numeroExterior'],
    cp = datosCrudos['cp'],
    activa = True)

  # Esta parte se incluye porque, por alguna extraña razón, el número interior
  # no es opcional, sino obligatorio. También, ¿por qué están guardados como
  # caracteres y no como números?
  if 'numeroInterior' in datosCrudos:
    direccionNueva.numeroInterior = datosCrudos['numeroInterior']
  else:
    direccionNueva.numeroInterior = 0

  direcciones = Usuario.objects.get(pk = identificador).direccion.all()

  for direccionGuardada in direcciones:
    if direccionGuardada.calle.lower()  == direccionNueva.calle and \
      direccionGuardada.numeroExterior  == str(direccionNueva.numeroExterior) and \
      direccionGuardada.colonia.lower() == direccionNueva.colonia and \
      direccionGuardada.municipio.lower()==direccionNueva.municipio and \
      direccionGuardada.cp              == direccionNueva.cp and \
      direccionGuardada.estado          == direccionNueva.estado and \
      direccionGuardada.tipoDeDireccion == direccionNueva.tipoDeDireccion:

      if direccionGuardada.activa == True:
        return django.http.HttpResponse("0")
      else:
        direccionGuardada.activa = True;
        direccionGuardada.save();
        return utilidades.respuestaJSON(direccionNueva)

  direccionNueva.save();
  Usuario.objects.get(pk = identificador).direccion.add(direccionNueva);

  return utilidades.respuestaJSON(direccionNueva)
