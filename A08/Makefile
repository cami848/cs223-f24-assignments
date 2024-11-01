CC=gcc
SOURCES=search warmup
FILES := $(subst .c,,$(SOURCES))
FLAGS=-g -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable

# By default, make runs the first target in the file
all: $(FILES)

% :: %.c read_ppm.c write_ppm.c shell.c
	$(CC) $(FLAGS) $< read_ppm.c write_ppm.c -o $@ -lm -lreadline

clean:
	rm -rf $(FILES)

