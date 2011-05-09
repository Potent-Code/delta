CC := gcc
#CFLAGS := -O3 -Wall
CFLAGS := -g -Wall
LDFLAGS := -lm

NAME := rlucches

# object names
OBJ := matrix.o vector.o vector_function.o linear_system.o float_cmp.o newton_method.o euler_method.o

all: mathlib.so

# build shared library
mathlib.so: ${OBJ}
	@echo "Compiling executable $@..."
	@${CC} ${CFLAGS} ${LDFLAGS} -shared -Wl,-soname,mathlib.so.0 -o $@ ${OBJ} -lc

# build objects
%.o: %.c
	@echo "Compiling object $@..."
	@${CC} ${CFLAGS} -fpic -c $<

# clean up
clean:
	@echo "Cleaning up..."
	@rm -f ${OBJ} mathlib.so
