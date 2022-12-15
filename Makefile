

mygtkapp: Sudoku_Project.c
	gcc `pkg-config --cflags gtk+-3.0` -o Sudoku_Project Sudoku_Project.c `pkg-config --libs gtk+-3.0`

