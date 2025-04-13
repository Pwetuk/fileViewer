CC = gcc
CFLAGS = -Wall -Wextra -Ipaths 
OBj_DIR = obj

OBJS = $(OBj_DIR)/paths_methods.o $(OBj_DIR)/FSaccess.o $(OBj_DIR)/main.o $(OBj_DIR)/filters.o

create_dir:
	@mkdir -p $(OBj_DIR)

all: create_dir main

$(OBj_DIR)/paths_methods.o: paths/paths_methods.c paths/paths.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBj_DIR)/filters.o: FSaccess/filters.c FSaccess/filters.h FSaccess/consts.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBj_DIR)/FSaccess.o: FSaccess/FSaccess.c $(OBj_DIR)/filters.o FSaccess/FSaccess.h FSaccess/filters.h FSaccess/consts.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBj_DIR)/main.o: main.c paths/paths.h FSaccess/FSaccess.h FSaccess/consts.h
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBj_DIR)/*.o

make_clean: all clean


