#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


sem_t boardable_mutex, full_mutex, end_journey_mutex,boardedcountmutex;
int passengers_boarded = 0;
int car_capacity;
int total_passengers;
int passengers_waiting = 0;
int passengers_remaining = 0;

void load() {
    printf("Car is loading passengers.\n");
    sleep(1);
}

void unload() {
    printf("Car is unloading passengers.\n");
    sleep(1);
}

void board(int passenger_id) {
    printf("Passenger %d is boarding.\n", passenger_id);
    sleep(1);
}

void offboard(int passenger_id) {
    printf("Passenger %d is getting off.\n", passenger_id);
    sleep(1);
}
void* car(void* args) {
    while (1) {
        // Load passengers
        load();

        // Signal passengers that the car is boardable
        for (int i = 0; i < car_capacity; ++i) {
            sem_post(&boardable_mutex);
        }

        // Wait until the car is full or there are no more passengers
        sem_wait(&full_mutex);

        // Run the duration of the ride
        printf("Car is on the ride.\n");
        sleep(2);  // Simulating the duration of the ride

        // Unload passengers
        unload();

        // Signal passengers that the journey has ended
        for (int i = 0; i < car_capacity; ++i) {
            sem_post(&end_journey_mutex);
        }
    }
}

void* passenger(void* args) {
    int passenger_id = *((int*)args);

    while (1) {
        // Wait until the car is boardable
        sem_wait(&boardable_mutex);

        // Board the car
        board(passenger_id);
        sem_wait(&boardedcountmutex);
        passengers_boarded++;
        sem_post(&boardedcountmutex);
        // If the car is full or no more passengers are waiting, signal the car
        if (passengers_boarded == car_capacity || passengers_waiting == 0) {
            sem_post(&full_mutex);
        }

        // Wait for the end of the journey
        sem_wait(&end_journey_mutex);

        // Get off the car
        offboard(passenger_id);

        // Reset passengers_boarded count
        if (passengers_boarded == car_capacity) {
            passengers_boarded = 0;
        }
    }
}


int main() {


    // Initialize semaphores
    sem_init(&boardable_mutex, 0, 0);
    sem_init(&full_mutex, 0, 0);
    sem_init(&end_journey_mutex, 0, 0);
    sem_init(&boardedcountmutex,0,1);
    // Take car capacity and total passengers as input
    printf("Enter car capacity: ");
    scanf("%d", &car_capacity);

    printf("Enter total number of passengers: ");
    scanf("%d", &total_passengers);
    if(total_passengers < car_capacity){
        printf("Total passengers cannot be less than car capacity.\n");
        return 0;
    }
    pthread_t car_thread, passenger_threads[total_passengers];
    int passenger_ids[total_passengers];
    passengers_waiting = total_passengers;
    passengers_remaining = total_passengers;

    // Create car thread
    pthread_create(&car_thread, NULL, car, NULL);

    // Create passenger threads
    for (int i = 0; i < total_passengers; ++i) {
        passenger_ids[i] = i + 1;
        pthread_create(&passenger_threads[i], NULL, passenger, &passenger_ids[i]);
    }

    // Join passenger threads
    for (int i = 0; i < total_passengers; ++i) {
        pthread_join(passenger_threads[i], NULL);
    }

    // Join the car thread
    pthread_join(car_thread, NULL);

    // Destroy semaphores
    sem_destroy(&boardable_mutex);
    sem_destroy(&full_mutex);
    sem_destroy(&end_journey_mutex);

    return 0;
}
