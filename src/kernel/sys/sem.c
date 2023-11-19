#include <sys/sem.h>

PUBLIC struct sem tabsem[SEM_MAX]; // Tabela de semáforos

/**
 * @brief Inicializa a tabela de semáforos.
 */
void sem_init() {
    for (int i = 0; i < SEM_MAX; i++) {
        tabsem[i].state = INACTIVE; // Define o estado como inativo
        tabsem[i].procusing = 0; // Inicializa a contagem de processos usando o semáforo
        tabsem[i].id = i; // Define o ID do semáforo
        tabsem[i].pos = i; // Define a posição do semáforo na tabela
    }
}

/**
 * @brief Verifica se o processo está associado ao semáforo.
 * @param sem Ponteiro para a estrutura de semáforo.
 * @return 0 se o processo está associado, -1 se não está associado.
 */
int check_valid(struct sem *sem) {
    int table = sem->pos / 16; // Determina a tabela em que o semáforo está
    int pos_table = sem->pos % 16; // Determina a posição do semáforo na tabela
    int comp = 1;
    
    comp = comp << pos_table; // Posiciona o valor 1 no bit correspondente

    int b = curr_proc->shared_sem[table]; // Obtém a tabela em que o semáforo está associado
    
    int result = b & comp; // Realiza a operação AND bit a bit
    if (result == 0)
        return -1; // O processo não está associado ao semáforo

    return 0; // O processo está associado ao semáforo
}