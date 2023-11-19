#include <nanvix/const.h>
#include <sys/sem.h>

/**
 * @brief Returns the current value of the semaphore identified by semid.
 *
 * @param semid Identifier of the semaphore.
 *
 * @return The current value of the semaphore if successful, -1 otherwise.
 */
int get_semaphore_value(int semid)
{
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE && tabsem[i].id == semid) {
            // Checks if the process is associated with the semaphore.
            if (valid(&tabsem[i]) == -1)
                return -1;

            return tabsem[i].value; // Returns the current value of the semaphore.
        }
    }
    return -1; // Returns -1 indicating failure to find the semaphore.
}

/**
 * @brief Sets the value of the semaphore identified by semid to val.
 *
 * @param semid Identifier of the semaphore.
 * @param val   Value to set the semaphore to.
 *
 * @return 0 if successful, -1 otherwise.
 */
int set_semaphore_value(int semid, int val)
{
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE && tabsem[i].id == semid) {
            // Checks if the process is associated with the semaphore.
            if (valid(&tabsem[i]) == -1)
                return -1;

            tabsem[i].value = val; // Sets the value of the semaphore.
            return 0; // Indicates success.
        }
    }
    return -1; // Returns -1 indicating failure to find the semaphore.
}

/**
 * @brief Decrements the value of the semaphore identified by semid and destroys it if possible.
 *
 * @param semid Identifier of the semaphore.
 *
 * @return 0 if successful, -1 otherwise.
 */
int decrement_and_destroy_semaphore(int semid)
{
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE && tabsem[i].id == semid) {
            // Checks if the process is associated with the semaphore.
            if (valid(&tabsem[i]) == -1)
                return -1;

            struct sem *sem = &tabsem[i]; // Creates a pointer to the semaphore.
            int table = sem->pos / 16; // Determines the table where the semaphore is located.
            int pos_table = sem->pos % 16; // Determines the position of the semaphore in the table.
            int comp = 1;

            comp = comp << pos_table; // Positions the value 1 in the bit to be changed.

            int *b = &curr_proc->shared_sem[table]; // Gets the table where the semaphore is.

            *b &= ~comp; // Assigns the value of the table to the set bit operation.

            sem->procusing--; // Decrements the count of processes using the semaphore.

            if (sem->procusing == 0) {
                sem->state = INACTIVE; // Sets the state of the semaphore to inactive.
                return 0; // Indicates success.
            }
            return 0; // Indicates success.
        }
    }
    return -1; // Returns -1 indicating failure to find the semaphore.
}

/**
 * @brief Executes operations on the semaphore identified by semid based on the cmd parameter.
 *
 * @param semid Identifier of the semaphore.
 * @param cmd   Command to perform on the semaphore.
 * @param val   Value associated with the command.
 *
 * @return The result of the operation if successful, -1 otherwise.
 */
PUBLIC int sys_semctl(int semid, int cmd, int val)
{
    switch (cmd) {
        case GETVAL:
            return get_semaphore_value(semid); // Gets the current value of the semaphore.
        case SETVAL:
            return set_semaphore_value(semid, val); // Sets the value of the semaphore.
        case IPC_RMID:
            return decrement_and_destroy_semaphore(semid); // Destroys the semaphore if possible.
    }

    return -1; // Returns -1 indicating an unsupported command.
}