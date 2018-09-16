/**
 * 
 */

//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_CHILD_THREADS 27

typedef int bool;

#define TRUE 1
#define FALSE 0

/* Global data structures */
char sudokuPuzzle[9][9];  /* Matrix to store sudoku puzzle data */
bool rows[9];
bool columns[9];
bool subgrids[9];

/*  Structure to represent the index (row and column) range of the elements in the puzzle */
typedef struct{
	int topRow;
	int bottomRow;
	int leftColumn;
	int rightColumn;
} posIndex; 

int main(int argc, char *argv[])
{
	int i;
	bool x = TRUE, y = FALSE;

	/* Get sudoku puzzle data from a text file */
	FILE *filePtr;
	filePtr = fopen("SudokuPuzzle.txt", "r");
	if(filePtr == NULL) {
		printf("Error opening file for reading.");
	}else { 
		printf("Sudoku Puzzle Solution:\n");
		while(!feof(filePtr)){
		fgets(sudokuPuzzle, 9, filePtr);
		printf("%s", sudokuPuzzle);
		}
		printf("\n");
		fclose(filePtr);
	}
    printf("\n");
    system("pause");
}
/*
void *checkColumn()
{}

void *checkRow()
{}

void *checkSubGrid()
{}
*/