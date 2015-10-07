#include "othm_thread.h"

char *test_str = "This is pointless\n";

OTHM_CHAIN_DEFUN(testing3, testing3)
{
	printf("testing3!!!!!!!!!\n");
	return othm_pair_new(NULL, NULL);
}

OTHM_CHAIN_DEFUN(testing4, testing4)
{
	printf("I heard they had a space program...\n");
	return othm_pair_new(NULL, NULL);
}

struct othm_pair testing(struct othm_typed *arg,
			 struct othm_typed *state,
			 struct othm_list *position)
{
	printf("hello!\n");
	return othm_pair_new(test_str, NULL);
}

struct othm_pair testing2(struct othm_typed *arg,
			  struct othm_typed *state,
			  struct othm_list *position)
{
	printf("%s", (char *) arg);
	return othm_pair_new(NULL, NULL);
}

OTHM_PRIM_FUNCT_INIT(testing, testing, struct othm_pair);
OTHM_PRIM_FUNCT_INIT(testing2, testing2, struct othm_pair);


int main(char **args)
{
	long t = 5;
	struct othm_list *chain = othm_chain_direct
		(othm_comp_from_prim(OTHM_PRIM_FUNCT(testing)),
		 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing2)),
		 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing3)),
		 NULL);
	printf("In main: creating thread %ld\n", t);
	struct othm_thread *thread = othm_thread_new(t, chain);
	othm_thread_start(thread);
	struct othm_list *chain2 = othm_chain_direct
		(othm_comp_from_prim(OTHM_PRIM_FUNCT(testing4)), NULL);
	othm_thread_start(othm_thread_new(t + 1, chain2));
	/* OTHM_THREAD_STOP_MUTATE(thread, 1); */
	pthread_exit(NULL);
	return 0;
}
