/*
 * Controlador de página de administración de tokens.
 * Aplicación web de sistema tokenizador.
 * Proyecto Lovelace.
 */

sistemaTokenizador.controller('controladorControl', [
  '$scope',
  '$location',
  '$mdDialog',
  'api',
  function (
    $scope,
    $location,
    $mdDialog,
    api
  )
  {
    /* Datos públicos. *******************************************************/

    $scope.cambiarTitulo("Control", 3);
    $scope.cliente = {};
    $scope.avisoDeActualizacion = false;

    /* Acciones públicas. ****************************************************/

    $scope.actualizarCliente = function ($event) {
      var padre = angular.element(document.body);
      $mdDialog.show({
        parent: padre,
        targetEvent: $event,
        templateUrl: '/estaticos/html/ventanas/operar_cliente.ventana.html',
        controller: 'controladorFormularioOperarCliente',
        locals: {
          "tituloOperacion": "Actualizar datos",
          "operacion": "actualizar"
        }
      }).then(function (respuesta) {
        if (respuesta != undefined) {
          console.log(respuesta);
        }
      });
    };

    $scope.eliminarCliente = function ($event) {
      var aviso = $mdDialog.confirm()
        .title('Advertencia')
        .textContent("@@include('mensajes/adv_eliminar_cliente.txt')")
        .ariaLabel('Advertencia')
        .targetEvent($event)
        .ok('Aceptar')
        .cancel('Cancelar')
        .multiple(true);
      $mdDialog.show(aviso).then(function (respuesta) {
        api.eliminarCliente().then(function (respuesta) {
          $scope.cerrarSesion();
        });
      }, function () {});
    };

    $scope.iniciarRefrescoDeLlaves = function ($event) {
      api.iniciarRefrescoDeLlaves().then(function (respuesta) {
        $scope.usuario.fields.estadoDeUsuario = 'en cambio de llaves'
      });
    };

    $scope.terminarRefrescoDeLlaves = function ($event) {
      api.terminarRefrescoDeLlaves().then(function (respuesta) {
        if (respuesta.data == "0") {
          $scope.usuario.fields.estadoDeUsuario = 'aprobado'

        } else if (respuesta.data == "1") {
          var aviso = $mdDialog.alert()
            .title('Error')
            .textContent(
              "@@include('mensajes/error_cliente_no_esta_en_cambio.txt')")
            .ariaLabel('Error')
            .targetEvent($event)
            .ok('Aceptar')
            .multiple(true);
          $mdDialog.show(aviso).then(function (respuesta) { });

        } else if (respuesta.data == "2") {
          var aviso = $mdDialog.confirm()
            .title('Advertencia')
            .textContent(
              "@@include('mensajes/adv_retokenizacion_incompleta.txt')")
            .ariaLabel('Advertencia')
            .targetEvent($event)
            .ok('Aceptar')
            .cancel('Cancelar')
            .multiple(true);
          $mdDialog.show(aviso).then(function (respuesta) {
            api.eliminarTokens().then(function (respuesta) {
              api.terminarRefrescoDeLlaves().then(function (respuesta) {
                $scope.usuario.fields.estadoDeUsuario = 'aprobado'
              });
            });
          }, function (respuesta) { });
        }
      });
    };

    /* Fase de inicialización. ***********************************************/

    api.verificarCriptoperiodo().then(function (respuesta) {
      if (respuesta.data == "1") {
        $scope.avisoDeActualizacion = true;
      }
    });

  }
]);
