#include "queue.h"

#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

void addToQueue(Queue *queue, int brightness, int duration) {
    // Create a new node
    Node *node = malloc(sizeof(Node));
    assert(node);

    // Initialize the node
    node->brightness = brightness;
    node->duration = duration;
    node->next = NULL;

    if (queue->back) {
        // The queue is not empty, add the new node to the back
        queue->back->next = node;
    } else {
        // The queue is empty, make this the first node
        queue->front = node;
    }

    queue->back = node;
}

void removeFromQueue(Queue *queue, int *brightness, int *duration) {
    if (!queue->front) {
        // The queue is empty, return error values
        *brightness = -1;
        *duration = -1;
        return;
    }

    // Return the front node and make the next node the new front
    Node *front = queue->front;
    queue->front = front->next;

    // If there is no new front, also make the back pointer NULL
    if (!queue->front) queue->back = NULL;

    // Return the values of the removed node
    *brightness = front->brightness;
    *duration = front->duration;

    // And finally free the removed node
    free(front);
}

int queueSize(Queue *queue) {
    size_t i = 0;

    // While `node` is a valid node, increment i and set `node` to the next node
    for (Node *node = queue->front; node; node = node->next, ++i);

    return i;
}

void initQueue(Queue *queue) {
    // Initialize the front and back to NULL (i.e., empty)
    queue->front = NULL;
    queue->back = NULL;

    // Initialize the mutex for later locking and unlocking
    pthread_mutex_init(&queue->mutex, NULL);
}
