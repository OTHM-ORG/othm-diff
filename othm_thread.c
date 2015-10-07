#include "othm_thread.h"

int othm_thread_stop_check(struct othm_thread *thread)
{
	int value;
	pthread_mutex_lock(&thread->stop_mutex);
	value = thread->stop_bool;
	pthread_mutex_unlock(&thread->stop_mutex);
	return value;
}

void othm_thread_stop_mutate(struct othm_thread *thread, int value)
{
	pthread_mutex_lock(&thread->stop_mutex);
	thread->stop_bool = value;
	pthread_mutex_unlock(&thread->stop_mutex);
}

void *othm_thread_run(void *thread)
{
#define THREAD ((struct othm_thread *) thread)
#define CURRENT_FUNCTION (((struct othm_comp *) exec_ptr->here)->prim)
	long tid;
	tid = (long)THREAD->t;
	printf("Hello World! It's me, thread #%ld!\n", tid);

	struct othm_list *exec_ptr = THREAD->chain;
	struct othm_pair pair;
	struct othm_term *result = NULL;
	struct othm_term *state = NULL;

	if (exec_ptr == NULL) {
		return NULL;
	}

	do {
		pair = OTHM_PRIM_FUNCT_GET(CURRENT_FUNCTION,
					   OTHM_CHAIN_FUNCT)
			(result, state, exec_ptr);
		result = pair.first;

		exec_ptr = (pair.second != NULL)
			? pair.second
			: exec_ptr->next;

	} while (!othm_thread_stop_check(THREAD) && exec_ptr);

	pthread_exit(NULL);

#undef  THREAD
#undef  CURRENT_FUNCTION
}

struct othm_thread *othm_thread_new(long t, struct othm_list *chain)
{
	struct othm_thread *thread = malloc(sizeof(struct othm_thread));
	thread->t = t;
	pthread_mutex_init(&thread->stop_mutex, NULL);
	thread->stop_bool = 0;
	thread->chain = chain;
	return thread;
}

void othm_thread_start(struct othm_thread *thread)
{
	thread->rc = pthread_create(&thread->thread, NULL,
				    othm_thread_run, (void *) thread);
}
