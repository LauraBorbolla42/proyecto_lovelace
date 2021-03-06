"""
  Modelo de tipos de usuarios,
  Aplicación web de sistema tokenizador.
  Proyecto Lovelace.
"""

import django

class TipoDeUsuario (django.db.models.Model):
  """
  Catálogo de tipos de usuario.

  A la fecha, se tienen 2 pensados:
  * Cliente.
  * Administrador.
  El contenido de este catálogo va de acuerdo al requerimiento sobre los actores
  del sistema.
  """

  nombre = django.db.models.CharField(
    verbose_name = 'nombre del tipo de usuario',
    unique = True,
    max_length = 100,
    primary_key = True)


  def __str__(self):
    """Representación en cadena de un tipo de usuario."""
    return self.nombre
