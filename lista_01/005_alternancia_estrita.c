/**
 * 5. Alternância Estrita em Dois Caixas
 *
 * Considere dois caixas de atendimento representados por duas threads. Os dois acessam uma
 * mesma variável global atendimentos_realizados. Use uma variável turn para alternar qual caixa
 * pode registrar atendimento.
 *
 *  a) Faça os dois caixas executarem a mesma quantidade de atendimentos.
 *  b) Faça o Caixa 0 tentar registrar mais atendimentos que o Caixa 1.
 *  c) Explique o que acontece quando o Caixa 1 termina e o Caixa 0 ainda quer registrar novos
 *  atendimentos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_THREADS 2
#define N_ATENDIMENTOS 100000

int g_turn = 0;
int g_atendimentos_realizados = 0;

int proximo_id(int id);
void *atender_caixa(void *thread_id);

/**
 * A alternância estrita cria uma dependência entre as threads para acessar
 * a região crítica. Quando uma das threads termina suas atividades antes das
 * outras, a alternância estrita pode fazer com as threads restantes fiquem
 * aguardando indefinidamente sua vez, mesmo que não exista mais concorrência
 * pela região crítica.
 */
int main () {
    pthread_t threads[N_THREADS];
	int thread_ids[N_THREADS];

    for (int i = 0; i < N_THREADS; i++) {
		thread_ids[i] = i;
        pthread_create(&threads[i], NULL, atender_caixa, &thread_ids[i]);
    }

	for (int i = 0; i < N_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Atendimentos realizados: %d\n", g_atendimentos_realizados);

    return 0;
}

int proximo_id(int thread_id) {
	int proximo = thread_id + 1;
	return proximo >= N_THREADS ? 0 : proximo;
}

void *atender_caixa(void *id) {
	int thread_id = *(int*)id;

	while (1) {

		// espera ocupada (busy wait)
		while (g_turn != thread_id) { }

		// prevenção de impasses (deadlocks)
		if (g_atendimentos_realizados == N_ATENDIMENTOS) {
			g_turn = proximo_id(thread_id);
			break;
		}

		// região crítica
		g_atendimentos_realizados++;

		g_turn = proximo_id(thread_id);
	}
}