#include <othm_thread.h>


#include <time.h>
#include <stdlib.h>



char *test_str = "This is pointless\n";

OTHM_SYMBOL_INIT(Bored);
OTHM_SYMBOL_INIT(Happy);
OTHM_SYMBOL_INIT(Sad);
OTHM_SYMBOL_INIT(Excited);


OTHM_CHAIN_DEFUN(testing3, testing3)
{
	struct othm_symbol_struct *sym;
	int r = rand() % 4;
	if (r == 0)
		sym = OTHM_SYMBOL(Bored);
	else if (r == 1)
		sym = OTHM_SYMBOL(Happy);
	else if (r == 2)
		sym = OTHM_SYMBOL(Sad);
	else if (r == 3)
		sym = OTHM_SYMBOL(Excited);
	return othm_pair_new(sym, NULL);
}

OTHM_CHAIN_DEFUN(testing4, testing4)
{
	othm_symbol_print(OTHMSYMBOLSTRUCT(arg));
	printf("\n");
	return othm_pair_new(arg, NULL);
}


OTHM_CHAIN_DEFUN(t5, t5)
{
	printf("bye\n");
	return othm_pair_new(NULL, NULL);
}

OTHM_CHAIN_DEFUN(t6, t6)
{
	printf("hi\n");
	return othm_pair_new(NULL, NULL);
}

int main(int argc, char **args)
{
	srand(time(NULL));
	long t = 5;
	struct othm_list *meta_control_chain = othm_chain_direct
		(othm_comp_from_prim(OTHM_PRIM_FUNCT(t6)),
		 NULL);
	struct othm_thread_control *meta_control = othm_thread_control_new(meta_control_chain, NULL);

	struct othm_list *control_chain = othm_chain_direct
		(othm_comp_from_prim(OTHM_PRIM_FUNCT(t5)),
		 NULL);
	struct othm_thread_control *control = othm_thread_control_new(control_chain, meta_control);

	struct othm_list *chain = othm_chain_direct
		(othm_comp_from_prim(OTHM_PRIM_FUNCT(testing3)),
		 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing4)),
		 NULL);
	printf("In main: creating thread %ld\n", t);
	struct othm_thread *thread = othm_thread_new(t, chain, control);
	othm_thread_start(thread);

	pthread_exit(NULL);
	return 0;
}
