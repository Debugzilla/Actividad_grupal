#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINEA 2048
#define MAX_COLUMNAS 35
#define MAX_REGISTROS 1000
#define DELIMITADOR ";"

typedef struct {
    int id;
    char *columnas[MAX_COLUMNAS];
} RegistroCSV;

RegistroCSV registros[MAX_REGISTROS];
int total_registros = 0;

// Muestra los créditos del programa
void mostrar_creditos() {
    printf("\n--- Componentes del grupo:\n");
    printf("1. Arnau Erta Carmona\n");
    printf("2. Adrian Cendoya Penaranda\n");
    printf("Gracias por usar este programa.\n");
}

// Función para dividir una línea CSV y almacenarla en la estructura
void cargar_registro(char *linea, int id) {
    RegistroCSV *reg = &registros[id];
    reg->id = id;

    char *token = strtok(linea, DELIMITADOR);
    int col = 0;

    while (token != NULL && col < MAX_COLUMNAS) {
        reg->columnas[col] = strdup(token);  // Duplica el contenido del campo
        col++;
        token = strtok(NULL, DELIMITADOR);
    }
}

// Carga todos los registros del archivo CSV
void cargar_csv(const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    char linea[MAX_LINEA];

    if (!archivo) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    while (fgets(linea, MAX_LINEA, archivo) && total_registros < MAX_REGISTROS) {
        linea[strcspn(linea, "\n")] = '\0'; // Elimina salto de línea
        cargar_registro(linea, total_registros);
        total_registros++;
    }

    fclose(archivo);
}

// Muestra todos los registros
void mostrar_todos_los_registros() {
    for (int i = 0; i < total_registros; i++) {
        printf("ID %d: ", registros[i].id);
        for (int j = 0; j < MAX_COLUMNAS && registros[i].columnas[j]; j++) {
            printf("%s%s", registros[i].columnas[j], j < MAX_COLUMNAS - 1 ? " | " : "");
        }
        printf("\n");
    }
}

// Muestra un registro por su ID
void mostrar_registro_por_id(int id) {
    if (id < 0 || id >= total_registros) {
        printf("ID no válido.\n");
        return;
    }

    printf("Registro ID %d:\n", id);
    for (int j = 0; j < MAX_COLUMNAS && registros[id].columnas[j]; j++) {
        printf("Columna %d: %s\n", j + 1, registros[id].columnas[j]);
    }
}

// Muestra el contenido bruto del CSV
void mostrar_csv(const char *nombre_archivo) {
    FILE *archivo;
    char linea[MAX_LINEA];

    archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    while (fgets(linea, MAX_LINEA, archivo)) {
        printf("%s", linea);
    }

    fclose(archivo);
}

// Muestra el menú principal
void mostrar_menu() {
    int opcion;

    do {
        printf("\n--- Menú ---\n");
        printf("1. Mostrar todos los registros\n");
        printf("2. Mostrar registro a partir de su ID\n");
        printf("4. Calcular el precio máximo (gasolina 95) [No implementado]\n");
        printf("5. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                mostrar_todos_los_registros();
                break;
            case 2: {
                int id;
                printf("Ingrese el ID del registro: ");
                scanf("%d", &id);
                mostrar_registro_por_id(id);
                break;
            }
            case 3:
                mostrar_creditos();
                break;
            case 4:
                printf("Función no implementada.\n");
                break;
            case 5:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Intente nuevamente.\n");
        }
    } while (opcion != 5);
}

// Función principal
int main() {
    const char *archivo_csv = "precios_albacete.csv";
    mostrar_creditos();
    cargar_csv(archivo_csv);
    mostrar_creditos();
    mostrar_menu();
    return 0;
}
