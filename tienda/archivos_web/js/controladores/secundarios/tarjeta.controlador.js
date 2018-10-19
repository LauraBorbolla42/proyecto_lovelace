/*
 * Controlador para aregar un método de pago.
 * Tienda en línea.
 * Proyecto Lovelace.
 */

tienda.controller('controladorFormularioTarjeta', [
  '$scope',
  '$mdDialog',
  'api',
  'direcciones',
  function (
    $scope,
    $mdDialog,
    api,
    direcciones
  )
  {
    /* Funciones públicas. ***************************************************/

    $scope.cancelar = function () {
      $mdDialog.hide();
    };

    /* Secuencia de inicio. **************************************************/
    $scope.tarjeta = {};
    $scope.direccion = {};
    $scope.direcciones = direcciones;
    $scope.direccionAnterior = undefined;

    $scope.emisores = [];
    $scope.metodos = [];
    $scope.tipos = [];
    $scope.estados = [];

    api.obtenerEmisores().then(function (respuesta) {
      $scope.emisores = respuesta.data;
    });

    api.obtenerMetodos().then(function (respuesta) {
      $scope.metodos = respuesta.data;
    });

    api.obtenerTipos().then(function (respuesta) {
      $scope.tipos = respuesta.data;
    })

    api.obtenerEstados().then(function (respuesta) {
      $scope.estados = respuesta.data;
    })
  }
]);
