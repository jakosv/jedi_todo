CC = clang
PROG = main
CFLAGS = -Wall -g -ansi -pedantic
OBJMODULES = todolist.o todolist_view.o task_list.o project_list.o \
			 task.o project.o storage.o database.o

.PHONY: clean 

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(PROG): main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(PROG)
