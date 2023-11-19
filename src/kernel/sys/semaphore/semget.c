#include <nanvix/const.h>
#include <sys/sem.h>

/**
 *  @brief Associa o processo à cadeia de processos do semáforo.
 *  @param sem Ponteiro para a estrutura de semáforo.
 *  @return O ID do semáforo.
 */
int associate_semaphore(struct sem *sem) {
    int table = sem->pos / 16; /* Encontra em qual tabela o semáforo está */
    int pos_table = sem->pos % 16; /* Descobre a posição do semáforo na tabela */
    int comp = 1;

    comp = comp << pos_table; /* Posiciona o valor 1 no bit que será alterado */

    int *b = &curr_proc->shared_sem[table]; /* Obtém a tabela em que o semáforo está */

    *b = *b | comp; /* Atribui o valor da tabela à operação de set bit */ 

    sem->procusing++;
 
    return sem->id;
}

/**
 *  @brief Cria um novo semáforo.
 *  @param key A chave do semáforo a ser criado.
 *  @return O ID do novo semáforo ou -1 em caso de erro.
 */
int create_semaphore(unsigned key) {
    struct sem *newsem = NULL;

    // Procura por um semáforo inativo na tabela de semáforos.
    for (int i = 0; i < SEM_MAX; i++){
        if (tabsem[i].state == INACTIVE){
            newsem = &tabsem[i];
            break;
        }
    }

    if (newsem == NULL)
        return -1;
    
    // Inicializa os campos do novo semáforo.
    newsem->key = key;
    newsem->value = 1;
    newsem->state = ACTIVE;
    newsem->procpriority = 0;
    newsem->procusing = 0;
    
    // Associa o processo ao semáforo criado.
    associate_semaphore(newsem);

    return newsem->id;
}

/**
 * @brief Cria e/ou retorna o ID de um semáforo ativo.
 * @param key A chave do semáforo a ser buscado/criado.
 * @return O ID do semáforo encontrado/criado.
 */
PUBLIC int sys_semget(sem_t key) {   
    // Busca o semáforo com a chave informada.
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE) 
            if (tabsem[i].key == key){
                return associate_semaphore(&tabsem[i]);
            }
    }

    // Cria um novo semáforo caso não exista.
    return create_semaphore(key);
}
