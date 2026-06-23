/**
 * 2. Saldo Bancário Compartilhado
 *
 * Crie um programa com duas threads acessando uma variável global saldo. Uma thread realiza
 * depósitos e outra realiza saques.
 * Inicialize saldo com 1000. A thread de depósito deve realizar 100.000 depósitos de 1 unidade. A
 * thread de saque deve realizar 100.000 saques de 1 unidade. Ao final, imprima o saldo esperado e o
 * saldo obtido.
 *
 * a) Aumente ou diminua o número de operações de depósito/saque, executando várias vezes
 * para ver se o problema ocorre.
 */

#include <stdio.h>
#include <pthread.h>

#define SALDO_INICIAL 1000
#define N_OPERACOES 100000

int g_saldo = SALDO_INICIAL;

void *depositar(void *args);
void *sacar(void *args);

/**
 * Com poucas operações, muitas execuções podem produzir o saldo correto.
 * Ao aumentar o número de operações, a condição de corrida aparece com mais frequência,
 * resultando em saldos diferentes do esperado.
 * O comportamento depende do escalonamento das threads.
 */
int main() {
    pthread_t thread_deposito, thread_saque;

    pthread_create(&thread_deposito, NULL, depositar, NULL);
    pthread_create(&thread_saque, NULL, sacar, NULL);

    pthread_join(thread_deposito, NULL);
    pthread_join(thread_saque, NULL);

    printf("Saldo Esperado: %d\n", SALDO_INICIAL);
    printf("Saldo Obtido: %d\n", g_saldo);

    return 0;
}

void *depositar(void *args) {
    for (int i = 0; i < N_OPERACOES; i++) {
        // região crítica
        g_saldo++;
    }
    return NULL;
}

void *sacar(void *args) {
    for (int i = 0; i < N_OPERACOES; i++) {
        // região crítica
        g_saldo--;
    }
    return NULL;
}