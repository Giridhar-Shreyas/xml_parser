SRC_FILES=Main.c xml.c
CFLAGS += -std=c99 -g -Wall -Wextra -Wno-missing-field-initializers

all: test

test: $(SRC_FILES)
	gcc $(CFLAGS) $(SRC_FILES) -lm -o main.out

clean:
	rm -f main.out *.o

run: test
	./main.out

