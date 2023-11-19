#include <nanvix/const.h>
#include <sys/sem.h>

/**
 * @brief Increases the value of the semaphore identified by semid.
 *
 * @param semid Identifier of the semaphore.
 *
 * @return 0 if successful, -1 otherwise.
 */
int up(int semid)
{   
    // Search for the semaphore by its identifier.
    for (int i = 0; i < SEM_MAX; i++) {
        // Check if the semaphore is active and matches the given identifier.
        if (tabsem[i].state == ACTIVE && tabsem[i].id == semid) {
            // Checks if the process is associated with the semaphore.
            if (valid(&tabsem[i]) == -1)
                return -1;

            // If the semaphore value is 0, wakes up processes waiting on the semaphore.
            if (tabsem[semid].value == 0)
                wakeup(tabsem[i].chain);

            tabsem[i].value++; // Increases the semaphore value.
            
            return 0; // Indicates success.
        }
    } 
    return -1; // Returns -1 indicating failure to find the semaphore.
}

/**
 * @brief Decreases the value of the semaphore identified by semid.
 *
 * @param semid Identifier of the semaphore.
 *
 * @return 0 if successful, -1 otherwise.
 */
int down(int semid)
{
    // Search for the semaphore by its identifier.
    for (int i = 0; i < SEM_MAX; i++) {
        // Check if the semaphore is active and matches the given identifier.
        if (tabsem[i].state == ACTIVE && tabsem[i].id == semid) {
            // Checks if the process is associated with the semaphore.
            if (valid(&tabsem[i]) == -1)
                return -1;

            // Wait while the semaphore value is 0; sleep the current process.
            while (tabsem[i].value == 0) {
                sleep(tabsem[i].chain, curr_proc->priority);
            }

            tabsem[i].value--; // Decreases the semaphore value.
            
            return 0; // Indicates success.
        }
    }
    return -1; // Returns -1 indicating failure to find the semaphore.
}

/**
 * @brief Performs semaphore operations.
 *
 * @param semid Identifier of the semaphore.
 * @param op Operation to be performed (positive value for up, negative for down).
 *
 * @return 0 if successful, -1 otherwise.
 */
PUBLIC int sys_semop(int semid, int op)
{
    if (op > 0)
        return up(semid); // Performs the "up" operation.
    
    if (op < 0)
        return down(semid); // Performs the "down" operation.
    
    return -1; // Returns -1 indicating an invalid operation.
}
