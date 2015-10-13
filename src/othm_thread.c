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

void *othm_thread_run_chain(struct othm_thread *thread,
			    void *input,
			    struct othm_list *chain,
			    struct othm_thread_control *control)
{
#define CURRENT_FUNCTION (((struct othm_comp *) exec_ptr->here)->prim)

	struct othm_pair pair;
	struct othm_list *exec_ptr;
	int skip;
	void *result;
        void *state;

	/* void *controller_state; */
	/* void *controller_result; */

	exec_ptr = chain;
	skip = 0;
	result = input;
        state = NULL;


	if (exec_ptr == NULL)
		return NULL;

	do {
		if (control != NULL) {
		        othm_thread_run_chain(thread,
					      NULL,
					      control->controller,
					      control->controller_control);
		}
		if (!skip) {
			pair = OTHM_PRIM_FUNCT_GET(CURRENT_FUNCTION,
						   OTHM_CHAIN_FUNCT)
				(result, state, exec_ptr);
			result = pair.first;

			exec_ptr = (pair.second != NULL)
				? pair.second
				: exec_ptr->next;
		} else {
		        skip = 0;
		}

	} while (!othm_thread_stop_check(thread) && exec_ptr);
	/* pthread_exit(NULL); */
	return NULL;
#undef  CURRENT_FUNCTION
}

void *othm_thread_run(void *thread)
{
#define THREAD ((struct othm_thread *) thread)
	/* struct othm_thread_control test; */
	/* test.controller = NULL; */
	othm_thread_run_chain(THREAD, NULL, THREAD->chain,
			      THREAD->top_control);
	pthread_exit(NULL);
#undef  THREAD
}

struct othm_thread *othm_thread_new(long t, struct othm_list *chain,
				    struct othm_thread_control *control)
{
	struct othm_thread *thread = malloc(sizeof(struct othm_thread));
	thread->t = t;
	pthread_mutex_init(&thread->stop_mutex, NULL);
	thread->stop_bool = 0;
	thread->chain = chain;
	thread->top_control = control;
	return thread;
}

struct othm_thread_control *othm_thread_control_new(struct othm_list *controller,
						    struct othm_thread_control
						    *controller_control)
{
	struct othm_thread_control *control = malloc(sizeof(struct othm_thread_control));
	control->controller = controller;
	control->controller_control = controller_control;

	return control;
}

void othm_thread_start(struct othm_thread *thread)
{
	thread->rc = pthread_create(&thread->thread, NULL,
				    othm_thread_run, (void *) thread);
}
