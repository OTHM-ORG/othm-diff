all:
	gcc -o test test.c othm_thread.c othm_chain.c -lothm_symbols -lothm_base -lothm_hashmap -pthread
