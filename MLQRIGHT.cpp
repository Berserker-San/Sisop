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
    int prom_tiempo_completado = 0;
    int prom_tiempo_espera = 0;
    int prom_tiempo_total = 0;

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
        printf("Proceso %c: Tiempo Completado = %d, Tiempo Espera = %d, Tiempo Total = %d\n", queue_1[i].id, queue_1[i].tiempo_completado, queue_1[i].tiempo_espera, queue_1[i].tiempo_total);
    	prom_tiempo_completado += queue_1[i].tiempo_completado;
    	prom_tiempo_espera += queue_1[i].tiempo_espera;
    	prom_tiempo_total += queue_1[i].tiempo_total;
    }
    
    for (int i = 0; i < q2_tamano; i++) {
        printf("Proceso %c: Tiempo Completado = %d, Tiempo Espera = %d, Tiempo Total = %d\n", queue_2[i].id, queue_2[i].tiempo_completado, queue_2[i].tiempo_espera, queue_2[i].tiempo_total);
    	prom_tiempo_completado += queue_2[i].tiempo_completado;
    	prom_tiempo_espera += queue_2[i].tiempo_espera;
    	prom_tiempo_total += queue_2[i].tiempo_total;
	}
    
    for (int i = 0; i < q3_tamano; i++) {
        printf("Proceso %c: Tiempo Completado = %d, Tiempo Espera = %d, Tiempo Total = %d\n", queue_3[i].id, queue_3[i].tiempo_completado, queue_3[i].tiempo_espera, queue_3[i].tiempo_total);
    	prom_tiempo_completado += queue_3[i].tiempo_completado;
    	prom_tiempo_espera += queue_3[i].tiempo_espera;
    	prom_tiempo_total += queue_3[i].tiempo_total;
	}
	
	printf("Promedios: Prom T. Completado = %d, Prom T. Espera = %d, Pron T. Total = %d\n", prom_tiempo_completado/cantidad_procesos, prom_tiempo_espera/cantidad_procesos, prom_tiempo_total/cantidad_procesos);

    // Orden de ejecución
    printf("\nOrden de ejecucion: %s\n", orden_ejecucion);
}

int main() {
    // Datos de entrada
    char IDS[] = {'A', 'B', 'C', 'D'};
    int BTS[] = {4, 3, 8, 1};
    int ATS[] = {0, 0, 0, 10};
    int Colas[] = {1, 1, 2, 3};
    int cantidad_procesos = 4;
	
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