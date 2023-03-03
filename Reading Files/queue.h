//This content was created by members of the OS course at TU Delft. Not work of my own

/**
 * @file queue.h
 *
 * A queue datastructure.
 *
 * This header defines the interface of the queue. You should not have to
 * modify this file; see queue.c instead.
 */

#pragma once

#include <pthread.h>

/**
 * A node in the queue.
 */
typedef struct Node {
    int brightness; ///< The brightness of the LED (0-255).
    int duration; ///< The duration, in ms, of the brightness level.
    struct Node *next; ///< The next node in the queue or NULL if this is the
    ///< last node.
} Node;

/**
 * A queue.
 */
typedef struct Queue {
    Node *front; ///< The first node of the queue or NULL if the queue is empty.
    Node *back; ///< The last node of the queue or NULL if the queue is empty.
    pthread_mutex_t mutex; ///< The mutex guarding the queue when multiple
    ///< threads are active.
} Queue;

/**
 * Adds an entry to the back queue.
 *
 * @param queue the queue to add to
 * @param brightness the brightness to add
 * @param duration the duration to add
 */
void addToQueue(Queue *queue, int brightness, int duration);

/**
 * Removes an entry from the front of the queue.
 *
 * \p brightness and \p duration are set to -1 if the queue is empty.
 *
 * @param queue the queue to remove from
 * @param brightness where to store the removed brightness (may not be NULL)
 * @param duration where to store the removed duration (may not be NULL)
 */
void removeFromQueue(Queue *queue, int *brightness, int *duration);

/**
 * Returns the length (in number of brightness-duration pairs) of the queue.
 *
 * @param queue the queue to calculate the size of
 * @return the length of the queue
 */
int queueSize(Queue *queue);

/**
 * Initializes a queue.
 *
 * The queue will be empty.
 *
 * @param queue the queue to initialize
 */
void initQueue(Queue *queue);
