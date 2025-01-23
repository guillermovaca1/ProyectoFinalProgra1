#include "sistema.h"
// Función para guardar el inventario en un archivo
void guardarInventario(Producto productos[], int cantidad) { // Nombre de la función / estructura donde se guarda el producto/ cantidad donde se guarda los productos del arreglo
    FILE *archivo = fopen(INVENTARIO_ARCHIVO, "w"); //puntero en archivo/ abre el archivo con nombre inventario/ w para escribir 
    if (!archivo) {  //estructura de control/!= operador logico de negación/ variable que se busca evaluar
        printf("Error al abrir el archivo para guardar el inventario.\n");
        return;
    }

    // Escribir cada producto en el archivo
    for (int i = 0; i < cantidad; i++) {  //estructura de control/ movimiento dentro de la matriz
        fprintf(archivo, "%d|%s|%.2f|%d|%d|%d\n",  //Imprime la información del producto
                productos[i].id,  // ID guardado del producto
                productos[i].nombre, // Nombre guardado del producto
                productos[i].precio,  // Precio guardado del producto
                productos[i].cantidad,   // cantidad guardado del producto
                productos[i].esProducto,   //Si el producto es o no materia prima
                productos[i].cantidadMateriales);  //Cantidad de materiales necesaria para la creación si es producto elaborable
        
        // Escribir materiales necesarios (si aplica)
        if (productos[i].esProducto) {   // Evalua si el producto i del arreglo productos es un producto elaborable
            for (int j = 0; j < productos[i].cantidadMateriales; j++) {   //movimiento dentro de la matriz
                fprintf(archivo, "%d,%d\n",   //imprime los materiales necesarios y la cantidad de los mismos para la creación de productos 
                        productos[i].materialesNecesarios[j], 
                        productos[i].cantidadesNecesarias[j]);
            }
        }
    }

    fclose(archivo);
    printf("Inventario guardado exitosamente en '%s'.\n", INVENTARIO_ARCHIVO); //cierra el archivo e imprime si el guardado fue un éxito 
}

// Función para cargar el inventario desde un archivo
void cargarInventario(Producto productos[], int *cantidad) {  //declaración de función/Arreglo de la estructura Producto/puntero que represente la cantidad de productos en el inventario
    FILE *archivo = fopen(INVENTARIO_ARCHIVO, "r"); //puntero en archivo/ abre el archivo con nombre inventario/ r para leer 
    if (!archivo) {
        printf("No se encontró un archivo de inventario previo. Se creará uno nuevo al guardar.\n"); //estructura de control/!= operador logico de negación/ variable que se busca evaluar
        *cantidad = 0;
        return;
    }

    char linea[256]; //crea una secuencia de caracteres
    *cantidad = 0; //inicializa en 0 el puntero cantidad

    while (fgets(linea, sizeof(linea), archivo)) {
        // Leer datos básicos del producto
        Producto p;
        int materialesLeidos = 0; 

        sscanf(linea, "%d|%[^|]|%f|%d|%d|%d\n", //Función para leer cadena de caracteres/ lee un entero/Leer todo hasta encontrar un delimitador/lee un flotante
               &p.id,  //pasa la dirección de la variable id de la estructura a p para almacenar el valor 
               p.nombre,  //se pasa la cadena de caracteres (no hace falta el operador & ya que es una cadena de caracteres)
               &p.precio,  // se usa la dirección de precio para almacenar el valor leido
               &p.cantidad,   //se pasa la dirección de cantidad 
               &p.esProducto,  //pasa la dirección de esProducto
               &p.cantidadMateriales);  //almacena la dirección de cantidad de materiales

        // Leer materiales necesarios si es un producto elaborable
        if (p.esProducto) { // Evalua si el producto i del arreglo productos es un producto elaborable
            for (int i = 0; i < p.cantidadMateriales; i++) { //movimiento dentro de la matriz
                fgets(linea, sizeof(linea), archivo); //almacena el tamaño de la cadena de caracteres, en el archivo
                sscanf(linea, "%d,%d\n",   //lee datos de una linea de texto cadena de texto de datos que se van a leer/lee un numero entero
                       &p.materialesNecesarios[i],  //se almacena el primer valor leido materiales necesarios
                       &p.cantidadesNecesarias[i]);  //se almacena el segundo valor a leer cantidad de estos materiales
                materialesLeidos++; //contrador que se incrementea si lee bien la linea de codigo
            }
        }

        productos[*cantidad] = p;
        (*cantidad)++;
    }

    fclose(archivo);
    printf("Inventario cargado exitosamente desde '%s'.\n", INVENTARIO_ARCHIVO);  //cierre del archivo 
}


// Función para agregar un producto
void agregarP(Producto productos[], int *cantidad) { // Nombre de la función / estructura donde se guarda el producto/ cantidad donde se guarda los productos del arreglo
    if (*cantidad >= MAX_PRODUCTOS) {   // condicional para no soprepasar el maximo de productos 
        printf("El inventario está lleno. No se pueden agregar más productos.\n"); 
        return;
    }

    printf("Ingrese ID del producto: ");  //pide el id a crear
    productos[*cantidad].id = validarEntero(); //Validación que el id sea un entero
    getchar(); // Limpiar el buffer 
    printf("Ingrese nombre del producto: "); //pide el nombre del producto
    fgets(productos[*cantidad].nombre, MAX_NOMBRE, stdin); //especial para leer cadenas de caracteres con espacios
    productos[*cantidad].nombre[strcspn(productos[*cantidad].nombre, "\n")] = '\0'; // Eliminar salto de línea
    printf("Ingrese precio del producto: "); //pide el precio del producto 
    scanf("%f", &productos[*cantidad].precio); //almacena en float el precio y lo guarda en el struct de productos en precio
    printf("Ingrese cantidad del producto: "); //pide la cantidad de producto
    productos[*cantidad].cantidad = validarEntero(); //valida que la información ingresada sea un entero

    // Preguntar si es una materia prima o un producto terminado
    printf("¿El producto es elaborable? (1 para Sí, 0 para No): ");  //pregunta si el producto ingresado es materia prima o elaborable en fábrica
    productos[*cantidad].esProducto = validarEntero();  //valida que la información proporcionada sea entero

    if (productos[*cantidad].esProducto == 1) {  //condicional si el producto es un elaborable en fábrica
        // Si es un producto terminado, pedir los materiales necesarios
        printf("Ingrese la cantidad de materiales necesarios para este producto: ");
        productos[*cantidad].cantidadMateriales = validarEntero(); //valida que la información ingresada sea un entero

        for (int i = 0; i < productos[*cantidad].cantidadMateriales; i++) { //movimiento dentro de la estructura producto especificamente en la cantidad de materiales
            printf("Ingrese ID del material #%d necesario: ", i + 1); //Pide el id del numero de material propuesto/ se usa para mostrar al usuario un indice de material más intuituvo
            productos[*cantidad].materialesNecesarios[i] = validarEntero(); //valida que la información ingresada sea un entero
            printf("Ingrese cantidad de material #%d necesario: ", i + 1); //Pide la cantidad necesaria de los productos/ se usa para mostrar al usuario un indice de material más intuituvo
            productos[*cantidad].cantidadesNecesarias[i] = validarEntero(); //valida que la información ingresada sea un entero
        }
    }

    (*cantidad)++; //aumenta la cantidad despues de pasar todo el condicional 
    printf("Producto agregado exitosamente.\n");
}

// Función para editar un producto
void editarP(Producto productos[], int cantidad) {  // Nombre de la función / estructura donde se guarda el producto/ cantidad donde se guarda los productos del arreglo
    int id, i, encontrado = 0; // variables a usar dentro de la función/ inicialización en 0 para evitar problemas
    printf("Ingrese ID del producto a editar: "); //pide el ID del producto que se vaya a editar
    id = validarEntero();  //valida que la información ingresada sea un entero
    for (i = 0; i < cantidad; i++) { //busca el producto con el ID ingresado
        if (productos[i].id == id) {
            encontrado = 1;  //producto encontrado
            printf("Nuevo nombre del producto: ");
            getchar(); // Limpiar buffer
            fgets(productos[i].nombre, MAX_NOMBRE, stdin); //Lee el nuevo nombre ingresado
            productos[i].nombre[strcspn(productos[i].nombre, "\n")] = '\0'; // Eliminar salto de línea
            printf("Nuevo precio del producto: "); 
            scanf("%f", &productos[i].precio); //cambia el precio al nuevo ingresado
            printf("Nueva cantidad del producto: ");
            productos[i].cantidad = validarEntero(); //valida que la información ingresada sea un entero
            printf("Producto editado exitosamente.\n"); 
            break;
        }
    }
    if (!encontrado) { //estructura de control/!= operador logico de negación/ variable que se busca evaluar
        printf("El producto con el ID %d no fue encontrado.\n", id);
    }
}

// Función para eliminar un producto
void eliminarP(Producto productos[], int *cantidad) { // Nombre de la función / estructura donde se guarda el producto/ cantidad donde se guarda los productos del arreglo
    int id, i, encontrado = 0;  // variables a usar dentro de la función/ inicialización en 0 para evitar problemas
    printf("Ingrese ID del producto a eliminar: "); //pide el ID del producto que se vaya a editar
    id = validarEntero();  //valida que la información ingresada sea un entero
    for (i = 0; i < *cantidad; i++) { //producto encontrado
        if (productos[i].id == id) {   
            encontrado = 1;
            for (int j = i; j < *cantidad - 1; j++) { //recorre el arreglo de productos desde el indice hasta l acantidad menos 1
                productos[j] = productos[j + 1]; //copia el producto en la posición siguiente hacia la posición actual
            }
            (*cantidad)--; // resta la cantidad 
            printf("Producto eliminado exitosamente.\n");
            break;
        }
    }
    if (!encontrado) { //estructura de control/!= operador logico de negación/ variable que se busca evaluar
        printf("El producto con el ID %d no fue encontrado.\n", id); 
    }
}

// Función para listar los productos
void listarP(Producto productos[], int cantidad) {  // Nombre de la función / estructura donde se guarda el producto/ cantidad donde se guarda los productos del arreglo
    if (cantidad == 0) { //si la cantidad es 0 imprime que no hay productos en inventario 
        printf("No hay productos en el inventario.\n");
        return;
    }
    printf("\n--- Lista de Productos ---\n");  //imprime la lista de productos
    for (int i = 0; i < cantidad; i++) {  // movimiento dentro de los productos
        printf("ID: %d\n", productos[i].id);  //Imprime el ID
        printf("Nombre: %s\n", productos[i].nombre);  //Imprime el nombre
        printf("Precio: %.2f\n", productos[i].precio);  //Imprime el precio con 2 decimales
        printf("Cantidad: %d\n", productos[i].cantidad);  //Imprime la cantidad que hay en bodega de ese producto
        if (productos[i].esProducto == 1) { // si el producto al ingresar se coloco con un 1 es un producto 
            printf("Este producto es un producto terminado.\n");
        } else {
            printf("Este producto es materia prima.\n"); //Si se coloco como 0 es un materia prima
        }
        printf("--------------------------\n");
    }
}

// Función para validar entrada de números enteros
int validarEntero() { 
    int numero; //declara variable donde se almacenara el numero 
    while (scanf("%d", &numero) != 1) { //intenta leer un numero entero desde la entrada estandar, si el usuario ingresa un caracter o una tecla especial regresa como 0 
        printf("Entrada no válida. Por favor, ingrese un número entero: ");
        while (getchar() != '\n'); // Limpiar el buffer
    }
    return numero;
}

// Función para producir un producto, descontando los componentes necesarios
void produccion(Producto productos[], int *cantidadProductos, ListaMateriales materiales[], int cantidadMateriales) {
    int idProducto, cantidadFabricar;
    printf("ID del producto a fabricar: ");
    idProducto = validarEntero();
    printf("Cantidad a fabricar: ");
    cantidadFabricar = validarEntero();

    // Buscar el producto a fabricar
    Producto *producto = NULL;
    for (int i = 0; i < *cantidadProductos; i++) {
        if (productos[i].id == idProducto) {
            producto = &productos[i];
            break;
        }
    }

    if (producto == NULL || !producto->esProducto) {
        printf("Producto no encontrado o no es elaborable.\n");
        return;
    }

    // Verificar inventario de materiales necesarios
    for (int i = 0; i < producto->cantidadMateriales; i++) {
        int idMaterial = producto->materialesNecesarios[i];
        int cantidadRequerida = producto->cantidadesNecesarias[i] * cantidadFabricar;
        int materialSuficiente = 0;

        for (int j = 0; j < *cantidadProductos; j++) {
            if (productos[j].id == idMaterial) {
                if (productos[j].cantidad >= cantidadRequerida) {
                    materialSuficiente = 1;
                } else {
                    printf("Material %s insuficiente. Requiere %d, disponible %d.\n",
                           productos[j].nombre, cantidadRequerida, productos[j].cantidad);
                    return;
                }
                break;
            }
        }

        if (!materialSuficiente) {
            printf("Material con ID %d no encontrado en inventario.\n", idMaterial);
            return;
        }
    }

    // Descontar materiales e incrementar inventario del producto final
    for (int i = 0; i < producto->cantidadMateriales; i++) {
        int idMaterial = producto->materialesNecesarios[i];
        int cantidadRequerida = producto->cantidadesNecesarias[i] * cantidadFabricar;

        for (int j = 0; j < *cantidadProductos; j++) {
            if (productos[j].id == idMaterial) {
                productos[j].cantidad -= cantidadRequerida;
                break;
            }
        }
    }

    producto->cantidad += cantidadFabricar;
    printf("Producción completada: %d unidades de %s fabricadas.\n", cantidadFabricar, producto->nombre);
}

int main() {
    Producto productos[MAX_PRODUCTOS];
    int cantidadProductos = 0;
    ListaMateriales materiales[MAX_MATERIALES]; // Declaración del arreglo de materiales
    int cantidadMateriales = 0; // Inicialización de la cantidad de materiales

    // Cargar inventario desde el archivo
    cargarInventario(productos, &cantidadProductos);

    int opcion;
    do {
        printf("\n--- Menú ---\n");
        printf("1. Agregar Producto\n");
        printf("2. Editar Producto\n");
        printf("3. Eliminar Producto\n");
        printf("4. Listar Productos\n");
        printf("5. Producción\n");
        printf("6. Guardar y Salir\n");
        printf("Ingrese opción: ");
        opcion = validarEntero();

        switch (opcion) {
            case 1:
                agregarP(productos, &cantidadProductos);
                break;
            case 2:
                editarP(productos, cantidadProductos);
                break;
            case 3:
                eliminarP(productos, &cantidadProductos);
                break;
            case 4:
                listarP(productos, cantidadProductos);
                break;
            case 5:
                produccion(productos, &cantidadProductos, materiales, cantidadMateriales);
                break;
            case 6:
                guardarInventario(productos, cantidadProductos);
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida.\n");
                break;
        }
    } while (opcion != 6);

    return 0;
}