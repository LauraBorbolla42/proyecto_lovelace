"""
  Modelo de tipos de usuarios,
  Aplicación web de sistema tokenizador.
  Proyecto Lovelace.
"""

from django.db import models

class TipoDeUsuario (models.Model):
  """
  Catálogo de tipos de usuario.

  A la fecha, se tienen 2 pensados:
  * Usuario.
  * Administrador.
  El contenido de este catálogo va de acuerdo al requerimiento sobre los actores
  del sistema.
  """

  nombre = models.CharField(
    verbose_name = 'nombre del tipo de usuario',
    unique = True,
    max_length = 100)


  def __str__(self):
    """Representación en cadena de un tipo de usuario."""
    return str(self.id) + ' - ' + self.nombre
