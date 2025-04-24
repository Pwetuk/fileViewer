CC = gcc
CFLAGS = -Wall -Wextra -Ipaths 
OBj_DIR = obj

OBJS = $(OBj_DIR)/paths_methods.o $(OBj_DIR)/FSaccess.o $(OBj_DIR)/main.o $(OBj_DIR)/filters.o $(OBj_DIR)/settings.o

create_dir:
	@mkdir -p $(OBj_DIR)

all: create_dir main.out

$(OBj_DIR)/paths_methods.o: FSaccess/paths/paths_methods.c FSaccess/paths/paths.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBj_DIR)/filters.o: FSaccess/paths/filters/filters.c FSaccess/paths/filters/filters.h 
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBj_DIR)/FSaccess.o: FSaccess/FSaccess.c $(OBj_DIR)/filters.o FSaccess/FSaccess.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBj_DIR)/settings.o: FSaccess/settings/settings.c FSaccess/settings/settings.h FSaccess/settings/settings_types.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBj_DIR)/main.o: main.c FSaccess/paths/paths.h FSaccess/FSaccess.h FSaccess/fs_types.h 
	$(CC) $(CFLAGS) -c -o $@ $<

main.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBj_DIR)/*.o main.out

make_clean: all clean


