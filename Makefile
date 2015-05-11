# Declaration of variables
CC = g++
CC_FLAGS = -w -g

# File names
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

# Main target
rksm: $(OBJECTS)
	$(CC) $(OBJECTS) -o rksm

# To obtain object files
%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
