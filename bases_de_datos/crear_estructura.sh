#!/bin/bash

#
# Script para crear la estructura de la base.
# No necesita permisos de administrador.
#
# Proyecto Lovelace.
#

USUARIO='administrador_lovelace_cdv'
CONTRASENIA='l0v3lac3-admin'

mysql -u $USUARIO -p$CONTRASENIA < crear_estructura.sql