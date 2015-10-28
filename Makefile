SRC_FILES = \
	src/othm_thread.c \
	src/othm_chain.c

OBJ_PATH = bin
OBJ_FILES = $(patsubst %,$(OBJ_PATH)/%,$(notdir $(SRC_FILES:.c=.o)))

LIBRARY_FILE = libothm_thread.so
HEADER_FILE_THREAD = othm_thread.h
HEADER_FILE_CHAIN  = othm_chain.h

all: bin $(LIBRARY_FILE)

bin:
	mkdir -p bin

$(LIBRARY_FILE): $(OBJ_FILES)
	gcc -shared -o $(LIBRARY_FILE) $(OBJ_FILES)

define make-obj
$(patsubst %.c, $(OBJ_PATH)/%.o, $(notdir $(1))): $(1)  src/$(HEADER_FILE_THREAD) src/$(HEADER_FILE_CHAIN)
	gcc -c -Wall -Werror -fPIC $$< -o $$@
endef

$(foreach src,$(SRC_FILES),$(eval $(call make-obj,$(src))))

.PHONY: clean test install
clean:
	-rm  $(LIBRARY_FILE) $(OBJ_FILES) test
test:
	gcc -g -Wall -o test test.c -lothm_thread -lothm_symbols -lothm_base -lothm_hashmap -pthread
	./test
install:
	cp $(LIBRARY_FILE) /usr/lib/
	cp src/$(HEADER_FILE_THREAD) /usr/include/
	cp src/$(HEADER_FILE_CHAIN) /usr/include/
	ldconfig
uninstall:
	-rm  /usr/lib/$(LIBRARY_FILE)
	-rm  /usr/include/$(HEADER_FILE_THREAD)
	-rm  /usr/include/$(HEADER_FILE_CHAIN)
	ldconfig
