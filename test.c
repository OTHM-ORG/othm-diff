#include <othm_thread.h>

#include <time.h>
#include <stdlib.h>

OTHM_SYMBOL_INIT(Nothing);
OTHM_SYMBOL_INIT(And);


OTHM_CHAIN_DEFUN(MaybeMonad, MaybeMonad)
{
	/* struct othm_thread_control *control = (struct othm_thread_control *) arg; */
	if (lower_control->result == OTHM_SYMBOL(Nothing))
		control->skip = 1;
	return othm_pair_new(NULL, NULL);
}

OTHM_CHAIN_DEFUN(testing, testing)
{
	printf("hi\n");
	return othm_pair_new(NULL, NULL);
}

OTHM_CHAIN_DEFUN(testing2, testing2)
{
	printf("bye\n");
	return othm_pair_new(NULL, NULL);
}

OTHM_CHAIN_DEFUN(testing3, testing3)
{
	struct othm_symbol_struct *sym;

	printf("you should see this!\n");

	if (getchar() == 'n')
		sym = OTHM_SYMBOL(Nothing);
	else
		sym = OTHM_SYMBOL(And);
	return othm_pair_new(sym, NULL);
}

OTHM_CHAIN_DEFUN(testing4, testing4)
{
	othm_symbol_print(OTHMSYMBOLSTRUCT(arg));
	printf(" this if not nothing!\n");
	return othm_pair_new(NULL, NULL);
}

int main(int argc, char **args)
{
	srand(time(NULL));
	long t = 5;

	struct othm_list *maybe_control_chain = othm_chain_direct
		(othm_comp_from_prim(OTHM_PRIM_FUNCT(MaybeMonad)),
		 NULL);
	struct othm_thread_control *maybe_control = othm_thread_control_new(maybe_control_chain, NULL);

	struct othm_list *chain = othm_chain_direct
		(othm_comp_from_prim(OTHM_PRIM_FUNCT(testing)),
		 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing2)),
		 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing3)),
		 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing4)),
		 NULL);
	printf("In main: creating thread %ld\n", t);
	struct othm_thread *thread = othm_thread_new(t, chain, maybe_control);
	othm_thread_start(thread);

	pthread_exit(NULL);
	return 0;
}
