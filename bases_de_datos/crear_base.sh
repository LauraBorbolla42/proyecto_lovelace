#!/bin/bash

#
# Envoltura de script de MySQL para crear base de datos.
# Requiere permisos de administración sobre MySQL.
#
# Proyecto Lovelace.
#

mysql -u root -p < crear_base.sql
