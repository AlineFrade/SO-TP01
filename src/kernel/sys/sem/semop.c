#include <nanvix/const.h>
#include <sys/sem.h>
#include <nanvix/klib.h>

int up(int semid)
{   
    kprintf("SOCORRRRROOOOOOOOOOOOOOOOOOOOO");

    // Search for the semaphore by its identifier.
    for (int i = 0; i < SEM_MAX; i++) {
        // Check if the semaphore is active and matches the given identifier.
        if (tabsem[i].state == ACTIVE && tabsem[i].id == semid) {
            // Checks if the process is associated with the semaphore.
            if (valid(&tabsem[i]) == -1){}
                return -1;

            // If the semaphore value is 0, wakes up processes waiting on the semaphore.
            if (tabsem[semid].value == 0)
                wakeup(curr_proc->chain);

            tabsem[i].value++; // Increases the semaphore value.
            
            return 0; // Indicates success.
        }
    } 
    return -1; // Returns -1 indicating failure to find the semaphore.
}

int down(int semid)
{
      kprintf("SOCORRRRROOOOOOOOOOOOOOOOOOOOO");
    // Search for the semaphore by its identifier.
    for (int i = 0; i < SEM_MAX; i++) {
        // Check if the semaphore is active and matches the given identifier.
        if (tabsem[i].state == ACTIVE && tabsem[i].id == semid) {
            // Checks if the process is associated with the semaphore.
            if (valid(&tabsem[i]) == -1)
                return -1;

            // Wait while the semaphore value is 0; sleep the current process.
            while (tabsem[i].value == 0) {
                sleep(curr_proc->chain, curr_proc->priority);
            }

            tabsem[i].value--; // Decreases the semaphore value.
            
            return 0; // Indicates success.
        }
    }
    return -1; // Returns -1 indicating failure to find the semaphore.
}

PUBLIC int sys_semop(int semid, int op)
{
    if (op > 0)
        return up(semid); // Performs the "up" operation.
    
    if (op < 0)
        return down(semid); // Performs the "down" operation.
    
    return -1; // Returns -1 indicating an invalid operation.
}
