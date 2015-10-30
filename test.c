#include <othm_thread.h>

#include <time.h>
#include <stdlib.h>

#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define COMPL(b) PRIMITIVE_CAT(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0

#define CHECK(...) CHECK_N(__VA_ARGS__, 0,)
#define CHECK_N(x, n, ...) n
#define PROBE(x) x, 1,

#define NOT(x) CHECK(PRIMITIVE_CAT(NOT_, x))
#define NOT_0 PROBE(~)

#define IS_PAREN(x) CHECK(IS_PAREN_PROBE x)
#define IS_PAREN_PROBE(...) PROBE(~)

/* Returns 0 unless probed */
#define BOOL(x) COMPL(NOT(x))

#define IF(c) IIF(BOOL(c))
#define IIF(c) PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

#define OTHM_END
#define MAP_MACRO(m, f, ...) 			\
	IF(f)					\
	((EVAL(f(m)) , MAP_MACRO_2(m, ...))	\
	, NULL)
#define MAP_MACRO_2(m, f, ...) 			\
	MAP_MACRO(m, f, ...)
#define chain_test_cast(x) othm_comp_from_prim(OTHM_PRIM_FUNCT(x))
#define CHAIN_MACRO(...) MAP_MACRO(chain_test_cast, ...)

OTHM_SYMBOL_INIT(Nothing);
OTHM_SYMBOL_INIT(And);


OTHM_CHAIN_DEFUN(MaybeController, MaybeController)
{
	if (lower_control->result == OTHM_SYMBOL(Nothing))
		lower_control->skip = 1;
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

OTHM_CHAIN_DEFUN(Identity, Identity)
{
	return othm_pair_new(NULL, NULL);
}

int main(int argc, char **args)
{

#define test_macro(a) IF(a)(1, test_macro2(a))
#define test_macro2(a) EXPAND(EXPAND(EXPAND(test_macro(NOT(a)))))

#define EMPTY()
#define EXPAND(...) __VA_ARGS__
#define DEFER(id) id EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
	printf("%i\n",  EXPAND(OBSTRUCT(0)));
	/* srand(time(NULL)); */
	/* long t = 5; */

	/* struct othm_list *identity_control_chain = othm_chain_direct */
	/* 	(othm_comp_from_prim(OTHM_PRIM_FUNCT(Identity)), */
	/* 	 NULL); */

	/* struct othm_list *maybe_control_chain = othm_chain_direct */
	/* 	(othm_comp_from_prim(OTHM_PRIM_FUNCT(MaybeController)), */
	/* 	 NULL); */

	/* struct othm_list *maybe_control = */
	/* 	othm_thread_controller_new(maybe_control_chain, */
	/* 				   identity_control_chain, */
	/* 				   identity_control_chain, */
	/* 				   identity_control_chain, */
	/* 				   identity_control_chain, */
	/* 				   identity_control_chain, */
	/* 				   identity_control_chain, */
	/* 				   NULL); */

	/* struct othm_list *chain = othm_chain_direct */
	/* 	(othm_comp_from_prim(OTHM_PRIM_FUNCT(testing)), */
	/* 	 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing2)), */
	/* 	 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing3)), */
	/* 	 othm_comp_from_prim(OTHM_PRIM_FUNCT(testing4)), */
	/* 	 NULL); */
	/* printf("In main: creating thread %ld\n", t); */
	/* struct othm_thread *thread = othm_thread_new(t, chain, maybe_control); */
	/* othm_thread_start(thread); */

	/* pthread_exit(NULL); */
	return 0;
}
