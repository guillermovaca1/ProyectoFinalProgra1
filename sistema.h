#ifndef SISTEMA_H //Verifica si el archjivo no ha sido definido previamente 
#define SISTEMA_H //Define el archivo para evitar múltiples inclusiones

#include <stdio.h>
#include <string.h>
// Definición de constantes
#define MAX_PRODUCTOS 100 // Número máximo de productos que el sistema puede manejar
#define MAX_NOMBRE 50 // Número máximo de caracteres que se puede colocar en el nombre
#define MAX_MATERIALES 10 // Número máximo de materiales que pueden ser usados para crear un producto 
#define INVENTARIO_ARCHIVO "inventario.txt"
// Estructura para un producto
typedef struct {
    int id; // código numérico único del producto 
    char nombre[MAX_NOMBRE]; // Nombre del producto
    float precio; //Precio del producto
    int cantidad; // Cantidad guardada en el inventario 
    int esProducto;  // 0 si es materia prima, 1 si es un producto elaborable
    int materialesNecesarios[MAX_MATERIALES]; // IDs de productos necesarios
    int cantidadesNecesarias[MAX_MATERIALES];  // Cantidades de los productos necesarios
    int cantidadMateriales; // Cuántos productos son necesarios para la fabricación
} Producto;

// Estructura para la lista de materiales (componentes necesarios para un producto)
typedef struct {
    int idProductoFinal; // ID del producto terminado
    int idComponente; // ID del componente necesario
    int cantidadNecesaria; // Cantidad requerida del componente
} ListaMateriales;

// Declaración de funciones
void agregarP(Producto productos[], int *cantidad);
void editarP(Producto productos[], int cantidad);
void eliminarP(Producto productos[], int *cantidad);
void listarP(Producto productos[], int cantidad);
int validarEntero();
float validarFlotante();
void produccion(Producto productos[], int *cantidadProductos, ListaMateriales materiales[], int cantidadMateriales);
// Declaración de funciones para manejo de archivos
void guardarInventario(Producto productos[], int cantidad);
void cargarInventario(Producto productos[], int *cantidad);


#endif //fin de la protección contra las inclusiones múltiples