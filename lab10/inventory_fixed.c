#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
 
#define ORDER_QUANTITY 30
 
int main(void)
{
    struct SharedData {
        int count;
        sem_t semaphore;
    };

    // Allocate shared memory for inventory
    struct SharedData *inventory = mmap(
        NULL,
        sizeof(struct SharedData),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );
 
    if (inventory == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    inventory->count = 100;
    sem_init(&inventory->semaphore, 1, 1);
 

    printf("Starting inventory: %d\n", inventory->count);
    fflush(stdout);
 
    // Create first order-processing process
    pid_t pid1 = fork();
 
    if (pid1 < 0) {
        perror("fork");
        sem_destroy(&inventory->semaphore);
        munmap(inventory, sizeof(struct SharedData));

        return 1;
    }
 
    if (pid1 == 0) {
        // Process customer order 1
        sem_wait(&inventory->semaphore);

        int available = inventory->count;
 
        if (available >= ORDER_QUANTITY) {
            available -= ORDER_QUANTITY;
            inventory->count = available;
 
            printf("Order 1 fulfilled: %d units\n",
                   ORDER_QUANTITY);
        }
        sem_post(&inventory->semaphore);
 
        _exit(0);
    }
 
    // Create second order-processing process
    pid_t pid2 = fork();
 
    if (pid2 < 0) {
        perror("fork");
        waitpid(pid1, NULL, 0);
        sem_destroy(&inventory->semaphore);
        munmap(inventory, sizeof(struct SharedData));
        return 1;
    }
 
    if (pid2 == 0) {
        // Process customer order 2
        sem_wait(&inventory->semaphore);

        int available = inventory->count;
 
        if (available >= ORDER_QUANTITY) {
            available -= ORDER_QUANTITY;
            inventory->count = available;
 
            printf("Order 2 fulfilled: %d units\n",
                   ORDER_QUANTITY);
        }
        sem_post(&inventory->semaphore);
 
        _exit(0);
    }
 
    // Wait for both order-processing processes
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
 
    printf("\nExpected inventory: 40\n");
    printf("Actual inventory:   %d\n", inventory->count);
 
    // Release shared memory
    sem_destroy(&inventory->semaphore);
    munmap(inventory, sizeof(struct SharedData));
 
    return 0;
}