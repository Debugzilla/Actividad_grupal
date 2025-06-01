#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

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

void mostrar_creditos() {
    printf("\n--- Componentes del grupo:\n");
    printf("1. Arnau Erta Carmona\n");
    printf("2. Adrian Cendoya Penaranda\n");
    printf("Gracias por usar este programa.\n");
}

void cargar_registro(char *linea, int id) {
    RegistroCSV *reg = &registros[id];
    reg->id = id;

    char *token = strtok(linea, DELIMITADOR);
    int col = 0;

    while (token != NULL && col < MAX_COLUMNAS) {
        reg->columnas[col] = strdup(token);
        col++;
        token = strtok(NULL, DELIMITADOR);
    }
}

void cargar_csv(const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    char linea[MAX_LINEA];

    if (!archivo) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    while (fgets(linea, MAX_LINEA, archivo) && total_registros < MAX_REGISTROS) {
        linea[strcspn(linea, "\n")] = '\0';
        cargar_registro(linea, total_registros);
        total_registros++;
    }

    fclose(archivo);
}

void mostrar_todos_los_registros() {
    for (int i = 0; i < total_registros; i++) {
        printf("ID %d: ", registros[i].id);
        for (int j = 0; j < MAX_COLUMNAS && registros[i].columnas[j]; j++) {
            printf("%s%s", registros[i].columnas[j], j < MAX_COLUMNAS - 1 ? " | " : "");
        }
        printf("\n");
    }
}

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

float convertir_a_float(const char *texto) {
    char temp[64];
    int i = 0;
    while (*texto && i < 63) {
        temp[i++] = (*texto == ',') ? '.' : *texto;
        texto++;
    }
    temp[i] = '\0';
    return atof(temp);
}

float calcular_precio_maximo_gasolina_95E5() {
    float max = 0.0;
    for (int i = 0; i < total_registros; i++) {
        char *precio_str = registros[i].columnas[9]; // Columna 10 (índice 9)
        if (precio_str && strlen(precio_str) > 0) {
            float precio = convertir_a_float(precio_str);
            if (precio > max) {
                max = precio;
            }
        }
    }
    return max;
}

float calcular_precio_minimo_gasolina_95E5() {
    float min = FLT_MAX;
    int encontrado = 0;

    for (int i = 0; i < total_registros; i++) {
        char *precio_str = registros[i].columnas[9]; // columna 10

        if (precio_str && strlen(precio_str) > 0) {
            float precio = convertir_a_float(precio_str);

            // Solo considerar precios válidos mayores que 0
            if (precio > 0.0 && precio < min) {
                min = precio;
                encontrado = 1;
            }
        }
    }

    if (!encontrado) {
        printf("No se encontraron precios válidos.\n");
        return 0.0;
    }

    return min;
}



void mostrar_menu() {
    int opcion;

    do {
        printf("\n--- Menú ---\n");
        printf("1. Mostrar todos los registros\n");
        printf("2. Mostrar registro a partir de su ID\n");
        printf("3. Calcular el precio máximo (gasolina 95)\n");
        printf("4. Calcular el precio mínimo (gasolina 95)\n");
        printf("5. Mostrar todos los registros ordenados por precio [No implementado]\n");
        printf("6. Salir\n");
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
            case 3: {
                float maxim = calcular_precio_maximo_gasolina_95E5();
                printf("\nPrecio máximo de gasolina 95 E5: %.3f €\n", maxim);
                break;
            }
            case 4: {
                float minim = calcular_precio_minimo_gasolina_95E5();
                printf("\nPrecio mínimo de gasolina 95 E5: %.3f €\n", minim);
                break;
                printf("Función no implementada.\n");
                break;
            }
            case 5:
                printf("Función no implementada.\n");
                break;
            case 6:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Intente nuevamente.\n");
        }
    } while (opcion != 6);
}

int main() {
    const char *archivo_csv = "precios_albacete.csv";
    mostrar_creditos();
    cargar_csv(archivo_csv);
    mostrar_menu();
    return 0;
}
