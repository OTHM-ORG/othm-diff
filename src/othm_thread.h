#ifndef OTHM_THREAD_H
#define OTHM_THREAD_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <othm_symbols.h>

#include "othm_chain.h"

struct othm_thread {
	pthread_t thread;
	int rc;
	long t;
	pthread_mutex_t stop_mutex;
	int stop_bool;
	struct othm_list *chain;

	struct othm_list *controller;
	struct othm_thread_control *top_control;
};


struct othm_thread_control {
	int skip;
	void *result;
        void *state;
	struct othm_list *position;

	struct othm_thread_control *controller_control;
};

int othm_thread_stop_check(struct othm_thread *thread);

void othm_thread_stop_mutate(struct othm_thread *thread, int value);

void *othm_thread_chain(void *thread);

struct othm_thread *othm_thread_new(long t, struct othm_list *chain,
				    struct othm_list *controller);

struct othm_list *othm_thread_controller_new(struct othm_list *first, ...);

void othm_thread_start(struct othm_thread *thread);

#endif
