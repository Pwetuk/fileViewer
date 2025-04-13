CC = gcc
CFLAGS = -Wall -Wextra -Ipaths 
OBj_DIR = obj

OBJS = $(OBj_DIR)/paths_methods.o $(OBj_DIR)/FSaccess.o $(OBj_DIR)/main.o

create_dir:
	@mkdir -p $(OBj_DIR)

all: create_dir main

$(OBj_DIR)/paths_methods.o: paths/paths_methods.c paths/paths.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBj_DIR)/FSaccess.o: FSaccess/FSaccess.c FSaccess/FSaccess.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBj_DIR)/main.o: main.c paths/paths.h FSaccess/FSaccess.h
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBj_DIR)/main.o $(OBj_DIR)/paths_methods.o $(OBj_DIR)/FSaccess.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBj_DIR)/*.o

make_clean: all clean


