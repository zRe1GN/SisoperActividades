/* ============================================================================
 * SISTEMAS OPERATIVOS
 * Ejercicio 1 (PLANTILLA): Particionamiento de memoria
 * Algoritmos de asignacion: First Fit, Best Fit, Worst Fit, Next Fit
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PARTICIONES 10
#define LIBRE 0
#define OCUPADA 1

typedef struct {
    int id;
    int tamano;
    int estado;
    int id_proceso;
} Particion;

int puntero_next_fit = 0;

void inicializar_particiones(Particion particiones[], int n) {
    int tamanos[MAX_PARTICIONES] = {100, 500, 200, 300, 600, 150, 400, 250, 350, 120};
    for (int i = 0; i < n; i++) {
        particiones[i].id = i;
        particiones[i].tamano = tamanos[i];
        particiones[i].estado = LIBRE;
        particiones[i].id_proceso = -1;
    }
}

void mostrar_particiones(Particion particiones[], int n) {
    printf("\n----------------------------------------------------------\n");
    printf(" ID | Tamano(KB) | Estado  | Proceso\n");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf(" %-3d| %-11d| %-8s| %s\n",
               particiones[i].id,
               particiones[i].tamano,
               particiones[i].estado == LIBRE ? "LIBRE" : "OCUPADA",
               particiones[i].estado == OCUPADA ? "" : "-");
        if (particiones[i].estado == OCUPADA) {
            printf("     -> Proceso P%d\n", particiones[i].id_proceso);
        }
    }
    printf("----------------------------------------------------------\n");
}

/* ----------------------------------------------------------------------
 * First Fit
 * ---------------------------------------------------------------------- */
int first_fit(Particion particiones[], int n, int tam_proceso) {
    for (int i = 0; i < n; i++) {
        if (particiones[i].estado == LIBRE && particiones[i].tamano >= tam_proceso) {
            return i;
        }
    }
    return -1;
}

/* ----------------------------------------------------------------------
 * Best Fit
 * ---------------------------------------------------------------------- */
int best_fit(Particion particiones[], int n, int tam_proceso) {
    int mejor_indice = -1;
    int mejor_tamano = __INT_MAX__;

    for (int i = 0; i < n; i++) {
        if (particiones[i].estado == LIBRE && particiones[i].tamano >= tam_proceso) {
            if (particiones[i].tamano < mejor_tamano) {
                mejor_tamano = particiones[i].tamano;
                mejor_indice = i;
            }
        }
    }
    return mejor_indice;
}

/* ----------------------------------------------------------------------
 * Worst Fit
 * ---------------------------------------------------------------------- */
int worst_fit(Particion particiones[], int n, int tam_proceso) {
    int peor_indice = -1;
    int peor_tamano = -1;

    for (int i = 0; i < n; i++) {
        if (particiones[i].estado == LIBRE && particiones[i].tamano >= tam_proceso) {
            if (particiones[i].tamano > peor_tamano) {
                peor_tamano = particiones[i].tamano;
                peor_indice = i;
            }
        }
    }
    return peor_indice;
}

/* ----------------------------------------------------------------------
 * Next Fit
 * ---------------------------------------------------------------------- */
int next_fit(Particion particiones[], int n, int tam_proceso) {
    for (int i = 0; i < n; i++) {
        int indice_actual = (puntero_next_fit + i) % n;
        if (particiones[indice_actual].estado == LIBRE && particiones[indice_actual].tamano >= tam_proceso) {
            puntero_next_fit = (indice_actual + 1) % n;
            return indice_actual;
        }
    }
    return -1;
} 

/* ----------------------------------------------------------------------
 * Liberar particion
 * ---------------------------------------------------------------------- */
int liberar_particion(Particion particiones[], int n, int id_proceso) {
    for (int i = 0; i < n; i++) {
        if (particiones[i].estado == OCUPADA && particiones[i].id_proceso == id_proceso) {
            particiones[i].estado = LIBRE;
            particiones[i].id_proceso = -1;
            return 1;
        }
    }
    return 0;
}

void asignar_particion(Particion particiones[], int indice, int id_proceso) {
    particiones[indice].estado = OCUPADA;
    particiones[indice].id_proceso = id_proceso;
}

int main() {
    Particion particiones[MAX_PARTICIONES];
    int n = MAX_PARTICIONES;
    int opcion, algoritmo, tam_proceso, id_proceso, indice;
    static int contador_procesos = 1;

    inicializar_particiones(particiones, n);

    do {
        printf("\n============ SIMULADOR DE PARTICIONAMIENTO DE MEMORIA ============\n");
        printf("1. Ver mapa de memoria\n");
        printf("2. Asignar un proceso\n");
        printf("3. Liberar un proceso\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                mostrar_particiones(particiones, n);
                break;

            case 2:
                printf("Tamano del proceso (KB): ");
                scanf("%d", &tam_proceso);

                printf("Algoritmo a usar:\n");
                printf(" 1. First Fit\n 2. Best Fit\n 3. Worst Fit\n 4. Next Fit\n");
                printf("Opcion: ");
                scanf("%d", &algoritmo);

                switch (algoritmo) {
                    case 1: indice = first_fit(particiones, n, tam_proceso); break;
                    case 2: indice = best_fit(particiones, n, tam_proceso);  break;
                    case 3: indice = worst_fit(particiones, n, tam_proceso); break;
                    case 4: indice = next_fit(particiones, n, tam_proceso);  break;
                    default:
                        printf("Algoritmo invalido.\n");
                        indice = -1;
                }

                if (indice == -1) {
                    printf("No hay particion disponible para un proceso de %d KB.\n", tam_proceso);
                } else {
                    id_proceso = contador_procesos++;
                    asignar_particion(particiones, indice, id_proceso);
                    printf("Proceso P%d asignado a la particion %d (%d KB).\n",
                           id_proceso, indice, particiones[indice].tamano);
                }
                break;

            case 3:
                printf("Id del proceso a liberar: ");
                scanf("%d", &id_proceso);
                if (liberar_particion(particiones, n, id_proceso)) {
                    printf("Proceso P%d liberado correctamente.\n", id_proceso);
                } else {
                    printf("No se encontro el proceso P%d.\n", id_proceso);
                }
                break;

            case 0:
                printf("Saliendo del simulador...\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 0);

    return 0;
}
