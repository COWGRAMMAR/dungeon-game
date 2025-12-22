CC = gcc
CFLAGS = -Wall -Wextra -I C:\\PDCurses\\include 
LDFLAGS = -L C:\\PDCurses\\wincon -lpdcurses

DIR = code
OBJ = main.o init.o logic.o
EXE = main.exe

app: $(OBJ)
	@del *.exe
	@echo File $(EXE) sudah dibersihkan 
	@$(CC) $(OBJ) -o main $(LDFLAGS)
	@echo File Siap dijalankan

main.o: $(DIR)/main.c 
	@$(CC) $(CFLAGS) -c $< -o $@

init.o: $(DIR)/init.c
	@$(CC) $(CFLAGS) -c $< -o $@

logic.o: $(DIR)/logic.c
	@$(CC) $(CFLAGS) -c $< -o $@

cln:
	@cls
	@del *.o *.exe
	@echo Semua file dihapus