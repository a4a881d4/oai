/*
 * Tsync.h
 *
 *  Created on: Oct 29, 2011
 *      Author: root
 */
#include <pthread.h>
#ifndef TSYNC_H_
#define TSYNC_H_

#define COMPILER_BARRIER() __asm__ __volatile__ ("" ::: "memory")
int mycount;
pthread_mutex_t exclusive;
pthread_mutex_t downlink_mutex[3][8];
pthread_mutex_t downlink_mutex_channel;
pthread_cond_t downlink_cond[3][8];
pthread_cond_t downlink_cond_channel;

pthread_mutex_t uplink_mutex[8][3];
pthread_mutex_t uplink_mutex_channel;
pthread_cond_t uplink_cond[8][3];
pthread_cond_t uplink_cond_channel;

int COT;
int COT_U;

volatile int _COT;
volatile int _COT_U;


int NUM_THREAD_DOWNLINK;
int NUM_THREAD_UPLINK;

ch_thread *e2u_t[8][3];
ch_thread *u2e_t[8][3];

pthread_t cthr_u[3][8];
pthread_t cthr_d[8][3];

#endif /* TSYNC_H_ */
