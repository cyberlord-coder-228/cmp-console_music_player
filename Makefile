CC = gcc

# main
CFLAGS_STRICT = -Wall -Wpedantic -Wextra
LDFLAGS_MAIN = -lasound -lFLAC
SRC = dll.c array.c find_music.c play_music.c main.c
OUT = cmp

# tests
CFLAGS_FORGIVING = -Wall -Wpedantic
LDFLAGS_TEST = -lcmocka
TEST_SRC = dll.c array.c test.c
OUT_TEST = test

all: main test

main:
	$(CC) $(CFLAGS_STRICT) $(LDFLAGS_MAIN) $(SRC) -o ${OUT}

test: test
	$(CC) $(CFLAGS_FORGIVING) $(LDFLAGS_TEST) $(TEST_SRC) -o ${OUT_TEST}

clean_cmp:
	rm -f ${OUT}

clean_test:
	rm -f ${OUT_TEST}
