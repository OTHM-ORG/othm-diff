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
			    struct othm_thread_control *control,
			    struct othm_thread_control *lower_control)
{
#define CURRENT_FUNCTION (((struct othm_comp *) exec_ptr->here)->prim)

	struct othm_pair pair;
	struct othm_list *exec_ptr;

	/* void *controller_state; */
	/* void *controller_result; */

	exec_ptr = chain;
	control->skip = 0;
	control->result = input;
        control->state = NULL;

	if (exec_ptr == NULL)
		return NULL;

	do {
		if (control->controller_control != NULL) {
		        othm_thread_run_chain(thread,
					      NULL,
					      control->controller,
					      control->controller_control,
					      control);
		}
		if (!control->skip) {
			pair = OTHM_PRIM_FUNCT_GET(CURRENT_FUNCTION,
						   OTHM_CHAIN_FUNCT)
				(control->result, control->state, exec_ptr,
				 control, lower_control);
			control->result = pair.first;

			exec_ptr = (pair.second != NULL)
				? pair.second
				: exec_ptr->next;
		} else {
		        control->skip = 0;
			exec_ptr = exec_ptr->next;
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
			      THREAD->top_control, NULL);
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
	if (controller_control == NULL) {
		/* Basically since struct othm_thread_control contains its state in its
		   controller_control, the lowest level chain without a controller must
		   still have that field filled to operate. Hopefully this means it is
		   easy to add a controller to a chain that does not have one.
		*/
		control->controller_control = malloc(sizeof(struct othm_thread_control));
		control->controller_control->controller = NULL;
		control->controller_control->controller_control = NULL;
	} else {
		control->controller_control = controller_control;
	}

	return control;
}

void othm_thread_start(struct othm_thread *thread)
{
	thread->rc = pthread_create(&thread->thread, NULL,
				    othm_thread_run, (void *) thread);
}
