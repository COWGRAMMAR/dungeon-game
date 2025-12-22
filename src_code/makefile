CC = gcc
CFLAGS = -Wall -Wextra -IC:/PDCurses/include 
LDFLAGS = -L C:/PDCurses/wincon -lpdcurses

OBJ_DIR = build

SRC = main.c init.c logic.c
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

EXE = $(ROOT)main.exe

app: $(OBJ)
	@del /Q $(EXE)
	@echo File $(EXE) sudah dibersihkan 
	@$(CC) $(OBJ) -o $(EXE) $(LDFLAGS)
	@echo File Siap dijalankan

$(OBJ_DIR)/%.o: %.c 
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

cln:
	@cls
	@del /Q "$(OBJ_DIR)\*".o "$(EXE)"
	@echo Semua file dihapus