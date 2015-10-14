#ifndef OTHM_CHAIN_H
#define OTHM_CHAIN_H

#include <stdlib.h>
#include <othm_symbols.h>
#include <stdarg.h>



#define OTHM_CHAIN_DEFUN(NAME, SYMBOL_NAME)			   \
	struct othm_pair NAME(void *arg,			   \
			      void *state,			   \
			      struct othm_list *position);	   \
	OTHM_PRIM_FUNCT_INIT(NAME, SYMBOL_NAME, struct othm_pair); \
	struct othm_pair NAME(void *arg,			   \
			      void *state,			   \
			      struct othm_list *position)

#define OTHM_CHAIN_FUNCT struct othm_pair (*) (void *,			\
					       void *,			\
					       struct othm_list *)

struct othm_comp {
	struct othm_symbol_struct *prim;
};


struct othm_comp *othm_comp_from_prim(struct othm_symbol_struct *prim);

struct othm_list *othm_chain_direct(struct othm_comp *first, ...);

#endif
