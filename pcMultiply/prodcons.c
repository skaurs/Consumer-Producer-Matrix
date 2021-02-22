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

// Bounded buffer put() get()
int put(Matrix * value)
{
 int matrixTot = 0;
 int matrixProd = 0;
 bigmatrix[fill_ptr] = value;
 fill_ptr = (fill_ptr)%MAX;
 counter++;
 matrixTot++;
 matrixProd++;
}

Matrix * get()
{
  return NULL;
}

// Matrix PRODUCER worker thread
void *prod_worker(void *arg)
{
  return NULL;
}

// Matrix CONSUMER worker thread
void *cons_worker(void *arg)
{
  return NULL;
}
