#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

void* left(void* args){}                  // cars on the left

void* right(void* args){}             // cars on the right

void passing(int dir){}


int main() {
    int left_cars, right_cars;
    printf("cars on left: ");
    scanf("%d", &left_cars);
    printf("cars on right: ");
    scanf("%d", &right_cars);
    pthread_t right_car_thread[right_cars], left_car_threads[left_cars];
    for (int i = 0; i < left_cars; ++i) {
        pthread_create(&left_car_threads[i], NULL, left, NULL);
    }
    for (int i = 0; i < right_cars; ++i) {
        pthread_create(&right_car_thread[i], NULL, right, NULL);
    }
    for (int i = 0; i < left_cars; ++i) {
        pthread_join(left_car_threads[i], NULL);
    }
    for (int i = 0; i < right_cars; ++i) {
        pthread_join(right_car_thread[i], NULL);
    }
}