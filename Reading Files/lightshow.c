#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>

#define LED1 7
#define LED2 0
#define LED3 2
#define LED4 3

typedef struct {
    Queue *queue;
    int pin;
} LedThreadData;

void *ledThread(void *arg) {
    LedThreadData *data = (LedThreadData *) arg;

    while (1) {
        int brightness, duration;
        removeFromQueue(data->queue, &brightness, &duration);

        if (brightness == -1 || duration == -1) {
            softPwmWrite(data->pin, 0);
            usleep(100000);
            continue;
        }

        softPwmWrite(data->pin, brightness);
        usleep(duration * 1000);
        softPwmWrite(data->pin, 0);
    }

    return NULL;
}

int main(int argc, char **argv) {

    wiringPiSetup();
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);

    softPwmCreate(LED1,0,100);
    softPwmCreate(LED2,0,100);
    softPwmCreate(LED3,0,100);
    softPwmCreate(LED4,0,100);
    Queue queues[4];
    initQueue(&queues[0]);
    initQueue(&queues[1]);
    initQueue(&queues[2]);
    initQueue(&queues[3]);

    LedThreadData ledThreads[4] = {
            {&queues[0], LED1},
            {&queues[1], LED2},
            {&queues[2], LED3},
            {&queues[3], LED4}
    };


    // Accept command line input one line at a time in the form LED(0-3) Brightness(0-100) Duration(ms)
    char input[100];
    int led,brightness,duration;
    while (scanf("%d %d %d", &led, &brightness, &duration)!= EOF) {
        if (led < 0 || led > 3) {
            printf("Invalid LED number: %d\n", led);
            continue;
        }

        if (brightness < 0 || brightness > 100) {
            printf("Invalid brightness value: %d\n", brightness);
            continue;
        }

        if (duration < 0) {
            printf("Invalid duration value: %d\n", duration);
            continue;
        }

        addToQueue(&queues[led], brightness, duration);
    }
    pthread_t threads[4];
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, ledThread, &ledThreads[i]);
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

