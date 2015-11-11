#include <othm_thread.h>

#include <time.h>
#include <stdlib.h>

OTHM_SYMBOL_INIT(Nothing);
OTHM_SYMBOL_INIT(And);

OTHM_CHAIN_DEFUN(MaybeController, MaybeController)
{
	if (lower_control->result == OTHM_SYMBOL(Nothing))
		lower_control->skip = 1;
	return othm_pair_new(NULL, NULL);
}

OTHM_CHAIN_DEFUN_TAGGED_BOTH(testing, testing,
			     char, char)
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

OTHM_CHAIN_DEFUN(Identity, Identity)
{
	return othm_pair_new(NULL, NULL);
}

int main(int argc, char **args)
{

	srand(time(NULL));
	long t = 5;

	struct othm_list *identity_control_chain = othm_chain_direct
		(NULL, othm_comp_from_prim(OTHM_PRIM_FUNCT(Identity)),
		 NULL);

	struct othm_list *maybe_control_chain = othm_chain_direct
		(NULL, othm_comp_from_prim(OTHM_PRIM_FUNCT(MaybeController)),
		 NULL);

	struct othm_list *maybe_control =
		othm_thread_controller_new(maybe_control_chain,
					   identity_control_chain,
					   identity_control_chain,
					   identity_control_chain,
					   identity_control_chain,
					   identity_control_chain,
					   identity_control_chain,
					   NULL);

	/* struct othm_list *chain = othm_chain_direct */
	/* 	(othm_comp_from_prim(OTHM_PRIM_FUNCT(testing)), */
	/* 	 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing2)), */
	/* 	 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing3)), */
	/* 	 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing4)), */
	/* 	 NULL); */
	struct othm_list *chain =
		OTHM_CHAIN_DIRECT(NULL, testing, testing2, testing3, testing4);
	printf("In main: creating thread %ld\n", t);
	struct othm_thread *thread = othm_thread_new(t, chain, maybe_control);
	othm_thread_start(thread);

	pthread_exit(NULL);
	return 0;
}
