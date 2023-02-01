CC=clang
PROG=main
CFLAGS=-Wall -g -ansi -pedantic

.PHONY: clean 

$(PROG):
	$(CC) main.c todolist.c task_list.c project_list.c database.c task.c project.c todolist_view.c storage.c -o $(PROG) $(CFLAGS)

clean:
	rm $(PROG)
