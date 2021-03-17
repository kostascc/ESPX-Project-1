/**
 * Real Time Embedded Systems
 * Project 1
 * 
 * Ⓒ 2021 K. Chatzis
 * kachatzis <at> ece.auth.gr
 **/

#ifndef PTHREAD_FIFO_H
#define PTHREAD_FIFO_H


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define QUEUESIZE 10
#define LOOP 20


typedef struct {
  void * (*work)(void *);
  void * arg;
} workFunction;


typedef struct {
  int buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;


queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, int in);
void queueDel (queue *q, int *out);


#endif