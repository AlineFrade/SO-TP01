#include <nanvix/const.h>
#include <sys/sem.h>

/**
 * @brief Associates the current process with the semaphore's process chain.
 *
 * @param sem Pointer to the semaphore.
 *
 * @return The ID of the semaphore if successful, -1 otherwise.
 */
int associate_process_with_semaphore(struct sem *sem)
{
    int table = sem->pos / 16; /* Finds the table where the semaphore is located */
    int pos_table = sem->pos % 16; /* Determines the position of the semaphore in the table */
    int bit_mask = 1;

    bit_mask = bit_mask << pos_table; /* Positions the value 1 in the bit to be altered */

    int *shared_semaphore_table = &curr_proc->shared_sem[table]; /* Gets the table where the semaphore is */

    *shared_semaphore_table |= bit_mask; /* Assigns the value of the table to the set bit operation */ 

    sem->procusing++;

    return sem->id;
}

/**
 * @brief Creates a new semaphore.
 *
 * @param key Key of the semaphore to be created.
 *
 * @return ID of the created semaphore if successful, -1 otherwise.
 */
int create_new_semaphore(unsigned key)
{
    struct sem *new_semaphore = NULL;

    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == INACTIVE) {
            new_semaphore = &tabsem[i];
            break;
        }
    }

    if (new_semaphore == NULL)
        return -1;
    
    new_semaphore->key = key;
    new_semaphore->value = 1;
    new_semaphore->state = ACTIVE;
    new_semaphore->procpriority = 0;
    new_semaphore->procusing = 0;
    
    if (associate_process_with_semaphore(new_semaphore) == -1)
        return -1;

    return new_semaphore->id;
}

/**
 * @brief Creates and/or returns the ID of an active semaphore.
 *
 * @param key Key of the semaphore.
 *
 * @return ID of the semaphore if successful, -1 otherwise.
 */
PUBLIC int sys_semget(sem_t key)
{   
    // Search for the semaphore with the provided key.
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE && tabsem[i].key == key) {
            return associate_process_with_semaphore(&tabsem[i]);
        }
    }

    // Creates a semaphore if not found.
    return create_new_semaphore(key);
}
