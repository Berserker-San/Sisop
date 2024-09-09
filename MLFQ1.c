#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define Procesos_Maximos 10
#define NIVELES 3

// Estructura que usaremos para representar los procesos
typedef struct {
    char id;
    int BT;
    int AT;
    int tiempo_restante;
    int tiempo_completado;
    int tiempo_espera;
    int tiempo_total;
    int nivel;
} Proceso;

// Round Robin (con castigo)
int round_robin_mlfq(Proceso Procesos[], int n, int quantum, int *tiempo_actual, char orden_ejecucion[], int nivel_actual) {
    bool completado[Procesos_Maximos] = {false};  // Un arreglo que indicara si el proceso ha sido completado
    int procesos_completados = 0;  
    bool procesos_en_cola;

    do {
    	procesos_en_cola = false;

        for (int i = 0; i < n; i++) {
        	
            if (!completado[i] && Procesos[i].AT <= *tiempo_actual && Procesos[i].nivel == nivel_actual && Procesos[i].tiempo_restante > 0) {
                procesos_en_cola = true;
				printf("Ejecutando proceso: %c, Tiempo entrada: %d\n", Procesos[i].id, *tiempo_actual);
				
                strncat(orden_ejecucion, &Procesos[i].id, 1);

                if (Procesos[i].tiempo_restante > quantum) {
                    *tiempo_actual += quantum;
                    Procesos[i].tiempo_restante -= quantum;
                    if (nivel_actual < NIVELES - 1) {
                        Procesos[i].nivel++;
                    }
                } else {
                    *tiempo_actual += Procesos[i].tiempo_restante;
                    Procesos[i].tiempo_restante = 0;
                    Procesos[i].tiempo_completado = *tiempo_actual;
                    Procesos[i].tiempo_total = Procesos[i].tiempo_completado - Procesos[i].AT;
                    Procesos[i].tiempo_espera = Procesos[i].tiempo_total - Procesos[i].BT;
                    completado[i] = true;
                    procesos_completados++;
                }
				
            printf("Terminando Proceso %c, tiempo salida %d\n", Procesos[i].id, *tiempo_actual);
            
			}
        }

        // Verificar si han llegado nuevos procesos para la cola actual antes de cambiar de nivel
        for (int i = 0; i < n; i++) {
            if (!completado[i] && Procesos[i].AT <= *tiempo_actual && Procesos[i].nivel <= nivel_actual && Procesos[i].tiempo_restante > 0) {
                procesos_en_cola = true;  // Si encontramos un proceso nuevo, volvemos a atender la cola actual
                nivel_actual = Procesos[i].nivel;
            }
        }

    } while (procesos_en_cola);  // Continuar hasta que no haya más procesos en la cola actual

    return *tiempo_actual;
}

// Simulación de MLFQ
void mlfq_simulation(Proceso Procesos[], int cantidad_procesos) {
    int quantum[] = {2, 4, 6};  // Los quanta para cada nivel
    int tiempo_actual = 0;
    char orden_ejecucion[100] = "";
    int prom_tiempo_completado = 0;
    int prom_tiempo_espera = 0;
    int prom_tiempo_total = 0;

    // Ejecutar colas en niveles ascendentes hasta que todos los procesos se completen
    for (int nivel = 0; nivel < NIVELES; nivel++) {
        tiempo_actual = round_robin_mlfq(Procesos, cantidad_procesos, quantum[nivel], &tiempo_actual, orden_ejecucion, nivel);
    }

    // Mostrar resultados finales
    printf("\nResultados:\n");
    for (int i = 0; i < cantidad_procesos; i++) {
        printf("Proceso %c: Tiempo Completado = %d, Tiempo Espera = %d, Tiempo Total = %d\n",
               Procesos[i].id, Procesos[i].tiempo_completado, Procesos[i].tiempo_espera, Procesos[i].tiempo_total);
        prom_tiempo_completado += Procesos[i].tiempo_completado;
        prom_tiempo_espera += Procesos[i].tiempo_espera;
        prom_tiempo_total += Procesos[i].tiempo_total;
    }

    // Promedios
    printf("Promedios: Prom T. Completado = %d, Prom T. Espera = %d, Prom T. Total = %d\n", 
           prom_tiempo_completado/cantidad_procesos, prom_tiempo_espera/cantidad_procesos, prom_tiempo_total/cantidad_procesos);

    // Orden de ejecución
    printf("\nOrden de ejecucion: %s\n", orden_ejecucion);
}

int main() {
    // Datos de entrada
    char IDS[] = {'A', 'B', 'C', 'D'};
    int BTS[] = {4, 3, 8, 1};
    int ATS[] = {0, 0, 0, 10};
    int cantidad_procesos = 4;

    // Procesos
    Proceso Procesos[Procesos_Maximos];
    for (int i = 0; i < cantidad_procesos; i++) {
        Procesos[i].id = IDS[i];
        Procesos[i].BT = BTS[i];
        Procesos[i].AT = ATS[i];
        Procesos[i].tiempo_restante = BTS[i];
		Procesos[i].nivel = 0;     // Todos los procesos comienzan en el nivel 0
    }

    // Simulación MLFQ
    mlfq_simulation(Procesos, cantidad_procesos);

    return 0;
}
