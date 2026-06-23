/**
 *4. Tentativa com Variável de Lock
 *
 * Escolha um dos cenários anteriores, como venda de ingressos ou reserva de assentos, e tente
 * proteger a região crítica usando uma variável global g_lock.
 *
 * a) Teste várias vezes com quantidade diferentes do recurso compartilhado e explique por que
 * essa solução ainda pode falhar.
 * b) Descreva o problema entre verificar o lock e marcar o lock.
 */

// Usando o cenário 003 (Reserva de Assentos):


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define N_THREADS 4
#define N_ASSENTOS 100000

bool g_lock = false;
int g_assentos_livres = N_ASSENTOS;

void *reservar_assento(void *args);

/**
 * Mesmo com a introdução do g_lock, o mecanismo de verificação e alteração do próprio lock
 * não é atômico. Isso transforma o g_lock em um recurso compartilhado sujeito a condições
 * de corrida, criando uma nova região crítica. Ele reduz a frequência das inconsistências,
 * mas não garante a exclusão mútua necessária para evitá-las por completo.
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

	// espera ocupada (busy waiting)
    while (g_lock) { }
    g_lock = true;

    for (int i = 0; i < N_ASSENTOS / N_THREADS; i++) {
        // região crítica
        g_assentos_livres--;
        contagem_reservas++;
    }

	g_lock = false;

    int *reservas_realizadas = malloc(sizeof(int));
    if (!reservas_realizadas) return NULL;

    *reservas_realizadas = contagem_reservas;
    return reservas_realizadas;
}