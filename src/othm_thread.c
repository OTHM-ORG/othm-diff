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
			    struct othm_thread_control *lower_control,
			    struct othm_list *controller)
{
#define CURRENT_FUNCTION (((struct othm_comp *) exec_ptr->here)->prim)

	/* struct othm_pair pair; */
	struct othm_list *exec_ptr;

	exec_ptr = chain;
	control->skip = 0;
	control->result = input;
        control->state = NULL;

	if (exec_ptr == NULL)
		return NULL;

	do {
		if (controller != NULL) {

		        othm_thread_run_chain(thread,
					      NULL,
					      controller->here,
					      control->controller_control,
					      control,
					      controller->next);
		}
		if (!control->skip) {
			OTHM_PRIM_FUNCT_GET(CURRENT_FUNCTION,
					    OTHM_CHAIN_FUNCT)
				(exec_ptr, control, lower_control);
			/* control->result = pair.first; */

			exec_ptr = (control->position != NULL)
				? control->position
				: exec_ptr->next;
			control->position = NULL;
		} else {
		        control->skip = 0;
			exec_ptr = exec_ptr->next;
		}

	} while (!othm_thread_stop_check(thread) && exec_ptr);

	return NULL;
#undef  CURRENT_FUNCTION
}

void *othm_thread_run(void *thread)
{
#define THREAD ((struct othm_thread *) thread)
	othm_thread_run_chain(THREAD,
			      NULL,
			      THREAD->chain,
			      THREAD->top_control,
			      NULL,
			      THREAD->controller);
	pthread_exit(NULL);
#undef  THREAD
}

struct othm_thread *othm_thread_new(long t, struct othm_list *chain,
				    struct othm_list *controller)
{
	struct othm_thread *thread;
	struct othm_thread_control *tc;
	struct othm_list *c;

	thread = malloc(sizeof(struct othm_thread));
	thread->t = t;
	pthread_mutex_init(&thread->stop_mutex, NULL);
	thread->stop_bool = 0;
	thread->chain = chain;
	thread->controller = controller;

	thread->top_control = malloc(sizeof(struct othm_thread_control));
	tc = thread->top_control;
	for (c = controller; c != NULL; c = c->next) {
		tc->controller_control = malloc(sizeof(struct othm_thread_control));
		tc = tc->controller_control;
	}
	return thread;
}

struct othm_list *othm_thread_controller_new(struct othm_list *first, ...)
{
	va_list argp;
	struct othm_list *list;
	struct othm_list *head;
	struct othm_list *tail;

	if (first == NULL)
		return NULL;

	list = first;
	head = malloc(sizeof(struct othm_list));
	tail = head;
	va_start(argp, first);
	do {
		tail->here = list;
	} while ((list = va_arg(argp, struct othm_list *))
		 ? (tail->next = malloc(sizeof(struct othm_list)),
		    tail = tail->next, 1)
		 : 0);

	tail->next = NULL;
	va_end(argp);

	return head;
}

void othm_thread_start(struct othm_thread *thread)
{
	thread->rc = pthread_create(&thread->thread, NULL,
				    othm_thread_run, (void *) thread);
}
