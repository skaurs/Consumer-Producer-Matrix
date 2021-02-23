/*
 *  prodcons module
 *  Producer Consumer module
 *
 *  Implements routines for the producer consumer module based on
 *  chapter 30, section 2 of Operating Systems: Three Easy Pieces
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 */

// Include only libraries for this module
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "counter.h"
#include "matrix.h"
#include "pcmatrix.h"
#include "prodcons.h"


// Define Locks and Condition variables here
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_MUTX_INITIALIZER;
pthread_cond_t empty, full;

// Producer consumer data structures
Matrix * bigmatrix[MAX]
int count = 0;
int fill_ptr = 0;
int use_ptr = 0;

// Bounded buffer bigmatrix defined in prodcons.h
//Matrix ** bigmatrix;

int matrixTot = 0;
int matrixProd = 0;
int matrixCons = 0;


// Bounded buffer put() get()
int put(Matrix * value)
{
 bigmatrix[fill_ptr] = value;
 fill_ptr = (fill_ptr + 1)%MAX;
 counter++;
 matrixTot++;
 matrixProd++; // metrix being made
}

Matrix * get()
{
  Matrix *temp = bigmatrix[use_ptr];
  use_ptr = (use_ptr +1)%MAX;
  counter--;
  matrixCons++;
  matrixTot++;
  return temp;
}

// Matrix PRODUCER worker thread
void *prod_worker(void *arg)
{
  int i;
  for(i = 0; i < loops; i++) {
     Pthread_mutex_lock(&mutex);
     while (count == MAX) 
	Pthread_cond_wait(&empty, &mutex);
     put(i);
     Pthread_cond_signal(&fill);
     Pthread_mutex_unlock(&mutex);
   }
}

// Matrix CONSUMER worker thread
void *cons_worker(void *arg)
{
  return NULL;
}
