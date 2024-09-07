#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define Procesos_Maximos 10

// Estructura que usaremos para representar los procesos
typedef struct {
    char id;
    int BT;
    int AT;
    int cola;
    int tiempo_restante;
    int tiempo_completado;
    int tiempo_espera;
    int tiempo_total;
} Proceso;

// Round Robin
int round_robin(Proceso Procesos[], int n, int quantum, int tiempo_actual, char orden_ejecucion[]) {
    bool completado[Procesos_Maximos] = {false};  // Un arreglo que indicara si el proceso ha sido completado o no
    int procesos_completados = 0;  

    while (procesos_completados < n) {
        for (int i = 0; i < n; i++) {
            if (!completado[i] && Procesos[i].AT <= tiempo_actual) {

                strncat(orden_ejecucion, &Procesos[i].id, 1); // Orden de ejecución

                if (Procesos[i].tiempo_restante > quantum) {
                    tiempo_actual += quantum;
                    Procesos[i].tiempo_restante -= quantum;
                } else {
                    tiempo_actual += Procesos[i].tiempo_restante;
                    Procesos[i].tiempo_restante = 0;
                    Procesos[i].tiempo_completado = tiempo_actual;
                    Procesos[i].tiempo_total = Procesos[i].tiempo_completado - Procesos[i].AT;
                    Procesos[i].tiempo_espera = Procesos[i].tiempo_total - Procesos[i].BT;
                    completado[i] = true;
                    procesos_completados++;
                }
            }
        }
    }
    return tiempo_actual;
}

// MLQ con tres colas Round Robin
void mlq_simulation(Proceso Procesos[], int cantidad_procesos) {
    Proceso queue_1[Procesos_Maximos], queue_2[Procesos_Maximos], queue_3[Procesos_Maximos];
    int q1_tamano = 0, q2_tamano = 0, q3_tamano = 0;
    int tiempo_actual = 0;

    // Cadena para guardar el orden de ejecución de los procesos
    char orden_ejecucion[100] = "";

    // Separar los procesos en las tres colas
    for (int i = 0; i < cantidad_procesos; i++) {
        if (Procesos[i].cola == 1) {
            queue_1[q1_tamano++] = Procesos[i];
        } else if (Procesos[i].cola == 2) {
            queue_2[q2_tamano++] = Procesos[i];
        } else if (Procesos[i].cola == 3) {
            queue_3[q3_tamano++] = Procesos[i];
        }
    }

    // Cola 1 con cuantum 2
    if (q1_tamano > 0) {
        tiempo_actual = round_robin(queue_1, q1_tamano, 2, tiempo_actual, orden_ejecucion);
    }

    // Cola 2 con cuantum 4
    if (q2_tamano > 0) {
        tiempo_actual = round_robin(queue_2, q2_tamano, 4, tiempo_actual, orden_ejecucion);
    }

    // Cola 3 con cuantum 6
    if (q3_tamano > 0) {
        tiempo_actual = round_robin(queue_3, q3_tamano, 6, tiempo_actual, orden_ejecucion);
    }

    // Resultados por cola
    printf("\nResultados:\n");
    for (int i = 0; i < q1_tamano; i++) {
        printf("Proceso %c: Tiempo Completado = %d, Tiempo Espera = %d, Tiempo Total = %d\n",
               queue_1[i].id, queue_1[i].tiempo_completado, queue_1[i].tiempo_espera, queue_1[i].tiempo_total);
    }
    for (int i = 0; i < q2_tamano; i++) {
        printf("Proceso %c: Tiempo Completado = %d, Tiempo Espera = %d, Tiempo Total = %d\n",
               queue_2[i].id, queue_2[i].tiempo_completado, queue_2[i].tiempo_espera, queue_2[i].tiempo_total);
    }
    for (int i = 0; i < q3_tamano; i++) {
        printf("Proceso %c: Tiempo Completado = %d, Tiempo Espera = %d, Tiempo Total = %d\n",
               queue_3[i].id, queue_3[i].tiempo_completado, queue_3[i].tiempo_espera, queue_3[i].tiempo_total);
    }

    // Orden de ejecución
    printf("\nOrden de ejecución: %s\n", orden_ejecucion);
}

void lectura(const char* nombre_archivo, char IDS[], int BTS[], int ATS[], int Colas[], int* cantidad_procesos) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }
    // Leer IDS
    for (int i = 0; i < Procesos_Maximos; i++) {
        fscanf(archivo, " %c", &IDS[i]);
    }
    // Leer Burst Times
    for (int i = 0; i < Procesos_Maximos; i++) {
        fscanf(archivo, "%d", &BTS[i]);
    }
    // Leer Arrival Times
    for (int i = 0; i < Procesos_Maximos; i++) {
        fscanf(archivo, "%d", &ATS[i]);
    }
    // Leer colas
    for (int i = 0; i < Procesos_Maximos; i++) {
        fscanf(archivo, "%d", &Colas[i]);
    }
    // Leer cantidad de procesos
    fscanf(archivo, "%d", cantidad_procesos);
    fclose(archivo);
}

int main() {
    // Datos de entrada
    char IDS[Procesos_Maximos];
    int BTS[Procesos_Maximos], ATS[Procesos_Maximos], Colas[Procesos_Maximos];
    int cantidad_procesos;

    // Lectura de datos
    lectura("ejemploMLQ.txt", IDS, BTS, ATS, Colas, &cantidad_procesos);

	printf("\nCantidad de procesos: %d\n", cantidad_procesos);
	printf("\nID1: %s\n", IDS[0]);
	
    // Procesos
    Proceso Procesos[Procesos_Maximos];
    for (int i = 0; i < cantidad_procesos; i++) {
        Procesos[i].id = IDS[i];
        Procesos[i].BT = BTS[i];
        Procesos[i].AT = ATS[i];
        Procesos[i].cola = Colas[i];
        Procesos[i].tiempo_restante = BTS[i];  // El tiempo restante es el tiempo que necesita el proceso (al inicio)
    }

    // Simulación MLQ
    mlq_simulation(Procesos, cantidad_procesos);

    return 0;
}