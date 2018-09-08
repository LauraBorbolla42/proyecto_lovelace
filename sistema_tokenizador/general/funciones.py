"""
  funciones.py Funciones de parte pública de sitio,
  Aplicación web de sistema tokenizador.
  Proyecto Lovelace.
"""

import json, hashlib
from .models.estado_de_usuario import EstadoDeUsuario
from .models.tipo_de_usuario import TipoDeUsuario
from .models.usuario import Usuario
from django.http import HttpResponse
from django.db.utils import IntegrityError
from sistema_tokenizador import utilidades
from sistema_tokenizador.configuraciones import DIRECTORIO_BASE
from sistema_tokenizador.general import negocio


def inicio (peticion):
  """
  Liga a archivo estático de página de inicio.

  Todas las urls de vistas principales pasan por aquí. index.html solamente
  contiene la aplicación de angular. Es el módulo de ngRoute, en el cliente,
  quien hace la resolución a un html en específico.
  """

  respuesta = open(\
    DIRECTORIO_BASE +
    'sistema_tokenizador/archivos_web/compilados/index.html', 'rb')
  return HttpResponse(content = respuesta)


@utilidades.privilegiosRequeridos(1)
def administracionDeTokens (peticion):
  """
  Liga a página de administración de tokens.

  Redirige la petición a la función de inicio. Lo importante aquí es la
  necesidad de privilegios para poder pasar por aquí: solamente los
  actores de tipo usuario pueden ver esta página (id = 1).

  Esta es la primera función que usa la notación de decoradores de python
  (la llamada a la función decoradora con un «@» antes de la definición
  decorada). La escritura del arroba es pura azúcar a nivel de sitaxis; es
  equivalente a esto:

    def administracionDeTokens (peticion):
      return inicio(peticion)
    administracionDeTokens =
      utilidades.privilegiosRequeridos(administracionDeTokens, 1)

  Más sobre el propio decorador en su definición, esto es, en el archivo
  de las utilidades.
  """

  return inicio(peticion)


@utilidades.privilegiosRequeridos(2)
def administracion (peticion):
  """
  Liga a página de administración.

  Redirige la petición a la función de inicio. Lo importante aquí es la
  necesidad de privilegios para poder pasar por aquí: solamente los
  actores de tipo administrador pueden ver esta página (id = 2).
  """

  return inicio(peticion)


def usuarioDeSesion (peticion):
  """
  Regresa el usuario de la sesión.

  En caso de no existir, se regresa un http vacío.
  """

  if 'usuario' in peticion.session:
    return HttpResponse(json.dumps(peticion.session['usuario']))
  else:
    return HttpResponse()


def iniciarSesion (peticion):
  """
  Valida las credenciales dadas para iniciar una sesión.

  En caso correcto, registra al usuario en la sesión y regresa el objeto del
  usuario; en caso incorrecto, regresa un http con un código de error.

  Ojo, lo que se guarda en la sesión y se regresa no es una instancia de
  Usuario, sino que es un diccionario con el correo y el id del tipo de usuario.
  El usuario no es serializable por el atributo de la contraseña.
  """

  objetoDePeticion = json.loads(peticion.body)
  usuario = negocio.autentificar(objetoDePeticion)
  if usuario != None:
    if usuario.estadoDeUsuario.nombre == 'no verificado':
      return HttpResponse("1")
    elif usuario.estadoDeUsuario.nombre == 'verificado':
      return HttpResponse("2")
    elif usuario.estadoDeUsuario.nombre == 'rechazado':
      return HttpResponse("3")
    elif usuario.estadoDeUsuario.nombre == 'en lista negra':
      return HttpResponse("4")
    else:
      usuarioSerializable = {
        'correo': usuario.correo,
        'tipoDeUsuario': usuario.tipoDeUsuario.pk};
      peticion.session['usuario'] = usuarioSerializable
      return HttpResponse(json.dumps(usuarioSerializable))
  else:
    return HttpResponse("0")


def cerrarSesion (peticion):
  """Elimina el objeto usuario de la sesión."""
  del peticion.session['usuario']
  return HttpResponse()


def registrarCliente (peticion):
  """
  Registra a un nuevo cliente en la base de datos

  Registra a el cliente dado en la base de datos y envía un correo con
  el vínculo de verificación; en caso de éxito se regresa un 0; en
  caso de que el cliente ya exista en la base se regresa un 1.
  """

  objetoDePeticion = json.loads(peticion.body)
  usuario = Usuario(
    correo = objetoDePeticion['correo'],
    contrasenia = hashlib.sha256(objetoDePeticion['contrasenia']
      .encode()).digest(),
    tipoDeUsuario = TipoDeUsuario.objects.get(
      nombre = 'cliente'),
    estadoDeUsuario = EstadoDeUsuario.objects.get(
      nombre = 'no verificado'))

  try:
    usuario.save()
  except IntegrityError:
    return HttpResponse("1")

  # TODO: mandar correo a usuario
  return HttpResponse("0")
