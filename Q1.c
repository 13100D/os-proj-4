#include <stdio.h>
#include <pthread.h>
#include <unistd.h>



pthread_mutex_t forks[5];
pthread_mutex_t bowls;
pthread_mutex_t eat_lock; // Mutex for controlling eating access
pthread_cond_t can_eat[2];
int meals_eaten[5] = {0}; // Track meals eaten by each philosopher

void eating(int philosopher_number) {
    // Enter eating state
    meals_eaten[philosopher_number]++; // Increment meals eaten by this philosopher
    printf("Philosopher %d is eating (%d meals eaten)\n", philosopher_number, meals_eaten[philosopher_number]);
    sleep(2); // Simulating eating
}

void thinking(int philosopher_number) {
    // Enter thinking state
    printf("Philosopher %d is thinking\n", philosopher_number);
    sleep(1); // Simulating thinking
}

void* philosopher(void* arg) {
    int philosopher_number = *((int*)arg);
    int left_fork = philosopher_number;
    int right_fork = (philosopher_number + 1) % 5;

    while (1) {
        thinking(philosopher_number);

        pthread_mutex_lock(&eat_lock); // Lock for eating access
        pthread_mutex_lock(&forks[left_fork]);
        pthread_mutex_lock(&forks[right_fork]);

        pthread_mutex_unlock(&eat_lock); // Release eating lock

        eating(philosopher_number);

        pthread_mutex_unlock(&forks[right_fork]);
        pthread_mutex_unlock(&forks[left_fork]);

        // Simulate a delay before the philosopher can eat again
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t philosophers[5];
    int philosopher_numbers[5];

    pthread_mutex_init(&bowls, NULL);
    pthread_mutex_init(&eat_lock, NULL); // Initialize the eating lock
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_init(&forks[i], NULL);
        pthread_cond_init(&can_eat[i], NULL);
    }

    for (int i = 0; i < 5; ++i) {
        philosopher_numbers[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_numbers[i]);
    }

    for (int i = 0; i < 5; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
