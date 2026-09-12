CC = gcc

CFLAGS = -std=c11 -Wall -Wextra -Iinc
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

TARGET = bin/fps

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=obj/%.o)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

format:
	clang-format -i src/*.c inc/*.h

clean:
	rm -rf obj
	rm -f $(TARGET)

.PHONY: run format clean