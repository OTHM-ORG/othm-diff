SRC_FILES = \
	src/othm_thread.c \
	src/othm_chain.c

OBJ_PATH = bin
OBJ_FILES = $(patsubst %,$(OBJ_PATH)/%,$(notdir $(SRC_FILES:.c=.o)))

LIBRARY_FILE = libothm_thread.so
HEADER_FILE_THREAD = othm_thread.h
HEADER_FILE_CHAIN  = othm_chain.h

all: $(LIBRARY_FILE)

$(LIBRARY_FILE): $(OBJ_FILES) src/$(HEADER_FILE)
	gcc -shared -o $(LIBRARY_FILE) $(OBJ_FILES)

define make-obj
$(patsubst %.c, $(OBJ_PATH)/%.o, $(notdir $(1))): $(1)
	gcc -c -Wall -Werror -fPIC $$< -o $$@
endef

$(foreach src,$(SRC_FILES),$(eval $(call make-obj,$(src))))

.PHONY : clean test install
clean :
	-rm  $(LIBRARY_FILE) $(OBJ_FILES) test
test :
	gcc -g -Wall -o test test.c -lothm_thread -lothm_symbols -lothm_base -lothm_hashmap -pthread
	./test
install :
	cp $(LIBRARY_FILE) /usr/lib/
	cp src/$(HEADER_FILE_THREAD) /usr/include/
	cp src/$(HEADER_FILE_CHAIN) /usr/include/
	ldconfig
uninstall :
	-rm  /usr/lib/$(LIBRARY_FILE)
	-rm  /usr/include/$(HEADER_FILE_THREAD)
	-rm  /usr/include/$(HEADER_FILE_CHAIN)
	ldconfig


# all: libothm_diff.so

# libothm_diff.so: othm_diff.o
# 	gcc -shared -o libothm_diff.so othm_diff.o

# othm_diff.o: othm_diff.c othm_diff.h
# 	gcc -c -Wall -Werror -fPIC othm_diff.c -o othm_diff.o

# .PHONY : clean test install uninstall

# clean :
# 	-rm -f test othm_diff.o libothm_diff.so test.o test2.o
# test :
# 	gcc -c test.c
# 	gcc -Wall -o test test.o -lothm_diff -lothm_symbols -lothm_base -lothm_hashmap -pthread
# 	./test
# install :
# 	cp othm_diff.h     /usr/include/
# 	cp libothm_diff.so /usr/lib/
# uninstall :
# 	-rm -f /usr/include/othm_diff.h
# 	-rm -f /usr/lib/libothm_diff.so


# all:
# 	gcc -o test test.c othm_thread.c othm_chain.c 
