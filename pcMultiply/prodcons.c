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
#include <unistd.h>
#include "counter.h"
#include "matrix.h"
#include "pcmatrix.h"
#include "prodcons.h"


// Define Locks and Condition variables here
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER, full = PTHREAD_COND_INITIALIZER;

// Producer consumer data structures
int count = 0;
int fill_ptr = 0;
int use_ptr = 0;

volatile int pcount = 0;
volatile int ccount = 0;
volatile int prgcount = 0;

//thread sharing matrices
Matrix * M1 = NULL;
Matrix * M2 = NULL;
Matrix * M3 = NULL;

// Bounded buffer put() get()
int put(Matrix * value)
{
 *(bigmatrix + fill_ptr) = value;
 fill_ptr = (fill_ptr + 1)%MAX;
 count++;
 return 0;
}

Matrix * get()
{
  Matrix *temp = *(bigmatrix + use_ptr);
  use_ptr = (use_ptr +1)%MAX;
  count--;
  return temp;
}

// Matrix PRODUCER worker thread
void *prod_worker(void *arg)
{
  ProdConsStats * prod_count = (ProdConsStats*) arg;
  for(; pcount < LOOPS; pcount++) {
     pthread_mutex_lock(&mutex);
     while (count == MAX && prgcount == 0) {
       pthread_cond_wait(&empty, &mutex);
     }
     if(prod_count->matrixtotal == LOOPS){
       pthread_cond_broadcast(&full);
     } else {
       Matrix * M1 = GenMatrixRandom();
       put(M1);
       prod_count->matrixtotal++;
       prod_count->sumtotal += SumMatrix(M1);	
       pthread_cond_signal(&full);
     }
     pthread_mutex_unlock(&mutex);
   }
   return prod_count;

}

// Matrix CONSUMER worker thread
void *cons_worker(void *arg)
{
  ProdConsStats * con_count = (ProdConsStats*) arg;
 
  for (; ccount < LOOPS; ccount++) {
   
    pthread_mutex_lock(&mutex);
    while (count == 0 && prgcount == 0) {
       
        pthread_cond_wait(&full, &mutex); 
    }
      //check if program is finished -> terminate leftover threads
      if (M1 == NULL && M2 == NULL && prgcount != 1) {
        M1 = get();
        con_count->matrixtotal++;
        con_count->sumtotal += SumMatrix(M1);
        
     } else if (M1 != NULL && M2 == NULL && prgcount != 1) {
         M2 = get();
         con_count->matrixtotal++;
         con_count->sumtotal += SumMatrix(M2);
         M3 = MatrixMultiply(M1, M2);
         
         //check multiplication
         if (M3 != NULL) {
            con_count->multtotal++;
            DisplayMatrix(M1, stdout);
            printf("   X\n");
            DisplayMatrix(M2, stdout);
            printf("   =\n");
            DisplayMatrix(M3, stdout);
            printf("\n");
            FreeMatrix(M1);
            FreeMatrix(M2);
            FreeMatrix(M3);
            M1 = NULL;
            M2 = NULL;
            M3 = NULL;
           } else { 
              FreeMatrix(M2);
              M2 = NULL;
              
              if (count == 0) {
                sleep((unsigned int ).1);
                
              } 
            }
        }
    
    if (con_count->matrixtotal == LOOPS) {
      if (M1 != NULL) {
        // printf("M1 Free.\n");
        FreeMatrix(M1);
        M1 = NULL; 
      }
     
      prgcount = 1;
      pthread_cond_broadcast(&empty);
    } else {
       pthread_cond_signal(&empty);
    }
    pthread_mutex_unlock(&mutex); 

  }
  return con_count;
}
