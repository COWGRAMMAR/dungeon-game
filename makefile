CC = gcc
CFLAGS = -Wall -Wextra -I C:\\PDCurses\\include 
LDFLAGS = -L C:\\PDCurses\\wincon -lpdcurses

DIR = dungeon
OBJ = main.o init.o


app: $(OBJ)
	@del main
	@echo File Siap dijalankan
	@$(CC) $(OBJ) -o main $(LDFLAGS)

main.o: main.c 
	@$(CC) $(CFLAGS) -c $< -o $@

init.o: init.c
	@$(CC) $(CFLAGS) -c $< -o $@

cln:
	@cls
	@del *.o main
	@echo Semua file dihapus