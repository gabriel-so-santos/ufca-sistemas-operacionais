/**
 *3. Reserva de Assentos
 *
 * Um cinema possui 100000 assentos disponíveis. Crie 4 threads representando sistemas de reserva
 * diferentes tentando reservar assentos ao mesmo tempo.
 * Para facilitar a observação da condição de corrida, use assentos_livres = 100000. Cada thread
 * deve tentar fazer várias reservas.
 *
 * Ao final, imprima:
 * * Assentos iniciais
 * * Reservas realizadas
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N_THREADS 4
#define N_ASSENTOS 100000

int g_assentos_livres = N_ASSENTOS;

void *reservar_assento(void *args);

/**
 * Mesmo que as threads executem exatamente o total de reservas, a condição
 * de corrida provoca perda de atualizações na variável compartilhada g_assentos_livres.
 * Como alguns decrementos são sobrescritos por outras threads, podem restar assentos
 * disponíveis ao final da execução, mesmo após todas as reservas terem sido realizadas.
 */
int main() {
    pthread_t threads[N_THREADS];
    int *buffer = NULL;
    int reservas_realizadas = 0;

    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, reservar_assento, NULL);
    }

    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], (void**)&buffer);

        if (buffer) {
            reservas_realizadas += *buffer;
            free(buffer);
        }
    }

    printf("Assentos Iniciais: %d\n", N_ASSENTOS);
    printf("Reservas Realizadas: %d\n", reservas_realizadas);
    printf("Assentos Restantes: %d\n", g_assentos_livres);

    return 0;
}

void *reservar_assento(void *args) {
    int contagem_reservas = 0;

    for (int i = 0; i < N_ASSENTOS / N_THREADS; i++) {
        // região crítica
        g_assentos_livres--;
        contagem_reservas++;
    }

    int *reservas_realizadas = malloc(sizeof(int));
    if (!reservas_realizadas) return NULL;

    *reservas_realizadas = contagem_reservas;
    return reservas_realizadas;
}