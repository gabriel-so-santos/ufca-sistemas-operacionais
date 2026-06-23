/**
 * 1. Venda de Ingressos sem Sincronização
 *
 * Crie um programa com 5 threads vendedoras e uma variável global ingressos_disponiveis iniciada
 * com 300000. Cada thread deve tentar vender ingressos enquanto ainda houver ingressos
 * disponíveis. Ao final, imprima ingressos iniciais, vendas registradas e ingressos restantes
 *
 * a) Identifique a região crítica do programa.
 * b) Aumente ou diminua o número de ingressos disponíveis executando várias vezes para ver
 * se o problema ocorre.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N_THREADS 5
#define INGRESSOS_INICIAIS 300000

int g_ingressos_disponiveis = INGRESSOS_INICIAIS;

void *vender_ingresso(void *args);

/**
 * A região crítica é a leitura e atualização da variável compartilhada g_ingressos_disponiveis.
 * Como não existe sincronização, várias threads podem ler o mesmo valor simultaneamente
 * e realizar decrementos concorrentes, causando inconsistências na contagem de ingressos.
 */
int main() {
    pthread_t threads[N_THREADS];
    int *buffer = NULL;
    int vendas_registradas = 0;

    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, vender_ingresso, NULL);
    }

    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], (void**)&buffer);

        if (buffer) {
            vendas_registradas += *buffer;
            free(buffer);
        }
    }

    printf("Ingressos Iniciais: %d\n", INGRESSOS_INICIAIS);
    printf("Vendas Registradas: %d\n", vendas_registradas);
    printf("Ingressos Restantes: %d\n", g_ingressos_disponiveis);

    return 0;
}

void *vender_ingresso(void *args) {
    int contagem_vendas = 0;

    // região crítica
    while (g_ingressos_disponiveis > 0) {
        g_ingressos_disponiveis--;
        contagem_vendas++;
    }

    int *vendas_registradas = malloc(sizeof(int));
    if (!vendas_registradas) return NULL;

    *vendas_registradas = contagem_vendas;
    return vendas_registradas;
}