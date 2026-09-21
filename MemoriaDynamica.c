#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *datos;
    size_t size;
    size_t capacity;
} ArregloDinamico;

void arreglo_iniciar(ArregloDinamico *arr, size_t capacidad_inicial) {
    arr->datos = (int *) malloc(capacidad_inicial * sizeof(int));

    if (arr->datos == NULL) {
        fprintf(stderr, "Error: malloc no pudo reservar memoria.\n");
        exit(EXIT_FAILURE);
    }

    arr->size = 0;
    arr->capacity = capacidad_inicial;

    printf("[INFO] Arreglo iniciado con capacidad para %zu elementos.\n",
           capacidad_inicial);
}

void arreglo_agregar(ArregloDinamico *arr, int valor) {
    if (arr->size == arr->capacity) {
        size_t nueva_capacidad = arr->capacity * 2;

        int *tmp = (int *) realloc(arr->datos,
                                   nueva_capacidad * sizeof(int));

        if (tmp == NULL) {
            fprintf(stderr, "Error: realloc no pudo ampliar la memoria.\n");
            free(arr->datos);
            exit(EXIT_FAILURE);
        }

        arr->datos = tmp;
        arr->capacity = nueva_capacidad;
    }

    arr->datos[arr->size] = valor;
    arr->size++;
}

void arreglo_insertar(ArregloDinamico *arr, size_t indice, int valor) {
    if (indice > arr->size) {
        printf("Error: indice fuera de rango.\n");
        return;
    }

    if (arr->size == arr->capacity) {
        size_t nueva_capacidad = arr->capacity * 2;
        int *tmp = realloc(arr->datos, nueva_capacidad * sizeof(int));

        if (tmp == NULL) {
            printf("Error: realloc fallo.\n");
            return;
        }

        arr->datos = tmp;
        arr->capacity = nueva_capacidad;
    }

    for (size_t i = arr->size; i > indice; i--) {
        arr->datos[i] = arr->datos[i - 1];
    }

    arr->datos[indice] = valor;
    arr->size++;
}

void arreglo_eliminar(ArregloDinamico *arr, size_t indice) {
    if (indice >= arr->size) {
        printf("Error: indice fuera de rango.\n");
        return;
    }

    for (size_t i = indice; i < arr->size - 1; i++) {
        arr->datos[i] = arr->datos[i + 1];
    }

    arr->size--;

    if (arr->size < arr->capacity / 4 && arr->capacity > 2) {
        size_t nueva_capacidad = arr->capacity / 2;

        if (nueva_capacidad < arr->size)
            nueva_capacidad = arr->size;

        int *tmp = realloc(arr->datos,
                           nueva_capacidad * sizeof(int));

        if (tmp != NULL) {
            arr->datos = tmp;
            arr->capacity = nueva_capacidad;
        }
    }
}

int arreglo_buscar(ArregloDinamico *arr, int valor) {
    for (size_t i = 0; i < arr->size; i++) {
        if (arr->datos[i] == valor) {
            return i;
        }
    }

    return -1;
}

void arreglo_liberar(ArregloDinamico *arr) {
    free(arr->datos);
    arr->datos = NULL;
    arr->size = 0;
    arr->capacity = 0;

    printf("[INFO] Memoria liberada correctamente.\n");
}

int main(void) {
    ArregloDinamico arr;

    arreglo_iniciar(&arr, 2);

    for (int i = 1; i <= 10; i++) {
        arreglo_agregar(&arr, i * 10);
    }

    printf("\nContenido inicial:\n");

    for (size_t i = 0; i < arr.size; i++) {
        printf("%d ", arr.datos[i]);
    }

    int valor = 999;

    arreglo_insertar(&arr, 3, valor);

    printf("\n\nDespues de insertar 999:\n");

    for (size_t i = 0; i < arr.size; i++) {
        printf("%d ", arr.datos[i]);
    }

    printf("\n\nPosicion de 999: %d\n",
           arreglo_buscar(&arr, 999));

    arreglo_eliminar(&arr, 3);

    printf("\nDespues de eliminar la posicion 3:\n");

    for (size_t i = 0; i < arr.size; i++) {
        printf("%d ", arr.datos[i]);
    }

    printf("\n");

    arreglo_liberar(&arr);

    return 0;
}
