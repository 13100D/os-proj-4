#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define max_cars 5

int left_cars, right_cars, cars_on_bridge = 0;
sem_t left_go, right_go, bridge_avail, bridgecountvaravail;




void passing(int dir){
    char arr[2][50] = {"car go nyoom LEFT<---RIGHT\n", "car go nyoom RIGHT--->LEFT\n"};
    sem_wait(&bridgecountvaravail);
    cars_on_bridge++;
    sem_post(&bridgecountvaravail);
    sleep(3);
    printf("%s", arr[dir]);
    sem_wait(&bridgecountvaravail);
    cars_on_bridge--;
    if(cars_on_bridge == 0){
        sem_post(&bridge_avail);
    }
    sem_post(&bridgecountvaravail);

}

void* left(void* args){
    //wait for some signal to go
    sem_wait(&left_go);
    passing(1);
}                  // cars on the left

void* right(void* args){
    //wait for some signal to go
    sem_wait(&right_go);
    passing(0);
}             // cars on the right

int main() {
    printf("cars on left: ");
    scanf("%d", &left_cars);
    printf("cars on right: ");
    scanf("%d", &right_cars);


    pthread_t right_car_thread[right_cars], left_car_threads[left_cars];
    sem_init(&left_go, 0, 0);
    sem_init(&right_go, 0, 0);
    sem_init(&bridge_avail, 0, 1);
    sem_init(&bridgecountvaravail, 0, 1);
    for (int i = 0; i < left_cars; ++i) {
        pthread_create(&left_car_threads[i], NULL, left, NULL);
    }
    for (int i = 0; i < right_cars; ++i) {
        pthread_create(&right_car_thread[i], NULL, right, NULL);
    }


    for (int i = 0; i < left_cars / max_cars; i++) {
        sem_wait(&bridge_avail);
        printf("--------------------\n");
        for (int j = 0; j < max_cars; j++) {
            sem_post(&left_go);
        }

    }
    if (left_cars % max_cars != 0) {
        sem_wait(&bridge_avail);
        printf("--------------------\n");
        for (int i = 0; i < left_cars % max_cars; i++) {
            sem_post(&left_go);
        }
    }
    for(int i=0; i<right_cars/max_cars; i++){
        sem_wait(&bridge_avail);
        printf("--------------------\n");
        for(int j=0; j<max_cars; j++){
        sem_post(&right_go);
        }

    }
    sem_wait(&bridge_avail);
    printf("--------------------\n");
    for(int i=0; i<right_cars%max_cars; i++){
        sem_post(&right_go);
    }

    for (int i = 0; i < left_cars; ++i) {
        pthread_join(left_car_threads[i], NULL);
    }
    for (int i = 0; i < right_cars; ++i) {
        pthread_join(right_car_thread[i], NULL);
    }
}