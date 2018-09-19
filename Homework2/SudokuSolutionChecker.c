/**
 * Eric Jacobson
 * CS3600-001
 * 9/14/2018
 * 
 * Homework 2
 * A C program to verify the solution to a Sudoku Puzzle using multithreading.
 * 
 * Most Linux/Mac OS users
 * 
 * gcc SudokuSolutionChecker.c -o SudokuSolutionChecker -lpthread
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_CHILD_THREADS 27
#define TRUE 1
#define FALSE 0

typedef int bool;

/* Global data structures, shared by all threads */
int sudokuPuzzle[9][9];  /* Matrix to store sudoku puzzle data */
bool columns[9];
int tid_columns[9];
bool rows[9];
int tid_rows[9];
bool subgrids[9];
int tid_subgrids[9];

/*  Structure to represent the index (row and column) range of the elements in the puzzle */
typedef struct{
	int topRow;
	int bottomRow;
	int leftColumn;
	int rightColumn;
} parameters;

/* Validate a column */
void *checkColumn(void* param)
{
	pthread_t self;
    parameters *params = (parameters*) param;
    int row = params->topRow;
    int column = params->rightColumn;

    int tRow = params->topRow;
    int bRow = params->bottomRow;
    int lCol = params->leftColumn;
    int rCol = params->rightColumn;

	if (row != 0 || column > 8)
	{
        fprintf(stderr, "Invalid row or column for column validation. Row: %d, Column: %d\n", row, column);
        pthread_exit(NULL);
    }

	self = pthread_self();
    int validColumns[9];
    int i;

    for (i = 0; i < 9; i++) 
	{
        int num = sudokuPuzzle[i][column];
        if (num < 1 || num > 9 || validColumns[num - 1] == 1) 
        {
            tid_columns[column] = self;
            columns[column] = FALSE;
            printf("%x T_Row: %d, B_Row: %d, L_Col: %d, R_Col: %d invalid!\n", self, tRow, bRow, lCol, rCol);
            pthread_exit(NULL);
        } else {
            validColumns[num - 1] = 1;
            
        }
    }
    
    printf("%x T_Row: %d, B_Row: %d, L_Col: %d, R_Col: %d valid!\n", self, tRow, bRow, lCol, rCol);
    tid_columns[column] = self;
    columns[column] = TRUE;    
    pthread_exit(NULL);
}

/* Validate a row */
void *checkRow(void* param)
{
	pthread_t self;
    parameters *params = (parameters*) param;
    int row = params->topRow;
    int column = params->leftColumn;

    int tRow = params->topRow;
    int bRow = params->bottomRow;
    int lCol = params->leftColumn;
    int rCol = params->rightColumn;
    
    if (column != 0 || row > 8) {
        fprintf(stderr, "Invalid row or column for row validation. Row: %d, Column: %d\n", row, column);
        pthread_exit(NULL);
    }
    
    self = pthread_self();
    int validRows[9];
    int i;

    for (i = 0; i < 9; i++) 
	{
        int num = sudokuPuzzle[row][i];
        if (num < 1 || num > 9 || validRows[num - 1] == 1) 
        {
            tid_rows[row] = self;
            rows[row] = FALSE;
            printf("%x T_Row: %d, B_Row: %d, L_Col: %d, R_Col: %d invalid!\n", self, tRow, bRow, lCol, rCol);
            pthread_exit(NULL);
        } else {
            validRows[num - 1] = 1;
        }
    }
    
    printf("%x T_Row: %d, B_Row: %d, L_Col: %d, R_Col: %d valid!\n", self, tRow, bRow, lCol, rCol);
    tid_rows[row] = self;
    rows[row] = TRUE;
    pthread_exit(NULL);
}

/* Validate a subgrid */
void *checkSubgrid(void* param)
{
	pthread_t self;
    parameters *params = (parameters*) param;
    int row = params->topRow;
    int column = params->rightColumn;

    int tRow = params->topRow;
    int bRow = params->bottomRow;
    int lCol = params->leftColumn;
    int rCol = params->rightColumn;
    
    if (row > 6 || row % 3 != 0 || column > 6 || column % 3 != 0) 
    {
        fprintf(stderr, "Invalid row or column for subgrid validation. Row: %d, Column: %d\n", row, column);
        pthread_exit(NULL);
    }
    self = pthread_self();
    int validGrid[9];
    int i, j;

    for (i = row; i < row + 3; i++) 
	{
        for (j = column; j < column + 3; j++) 
		{
            int num = sudokuPuzzle[i][j];
            if (num < 1 || num > 9 || validGrid[num - 1] == 1) 
            {
                tid_subgrids[row + column / 3] = self;
                subgrids[row + column / 3] = FALSE;
                printf("%x T_Row: %d, B_Row: %d, L_Col: %d, R_Col: %d invalid!\n", self, tRow, bRow, lCol, rCol);
                pthread_exit(NULL);
            } else {
                validGrid[num - 1] = 1;
            }
        }
    }
    
    printf("%x T_Row: %d, B_Row: %d, L_Col: %d, R_Col: %d valid!\n", self, tRow, bRow, lCol, rCol);
    tid_subgrids[row + column / 3] = self;
    subgrids[row + column / 3] = TRUE;
    pthread_exit(NULL);
}

int main()
{
	pthread_t tid[NUM_CHILD_THREADS]; /* the thread identifier for child threads */
	
	int i, j, threadIndex = 0;

	/* Get sudoku puzzle solution data from a text file */
	FILE *filePtr;
	filePtr = fopen("SudokuPuzzle.txt", "r");
	if(filePtr == NULL)	
    {
		printf("Error opening file for reading.");
		system("exit");
	}else { 
		printf("Sudoku Puzzle Solution:\n");
		/**
         * Couldn't get this working with fgets(), using fscanf instead!
         * 
        while(!feof(filePtr)){
	    	fgets(sudokuPuzzle, 9, filePtr);
		    printf("%s", sudokuPuzzle);
		}
		printf("\n");
		fclose(filePtr);
        */

        for(i = 0; i < 9; i++)
        {
            for(j = 0; j < 9; j++)
            {
                int k;
                if(fscanf(filePtr, " %d", &k) != 1)
                {
                    printf("Error");
                }else {
                    sudokuPuzzle[i][j] = k;
                }
            }
        }
        fclose(filePtr);
        printf("\n");
        for(i = 0; i < 9; i++)
        {
            for(j = 0; j < 9; j++)
            {
                printf("%d\t", sudokuPuzzle[i][j]);
            }
            printf("\n");
        }
        printf("\n");
	}

    /* Create 9 threads to validate columns */
    for(i = 0; i < 9; i++)
    {
		parameters *position = (parameters*) malloc(sizeof(parameters));
		position -> topRow = 0;
        position -> bottomRow = 8;
        position -> leftColumn = (i);
        position -> rightColumn = (i);
		pthread_create(&tid[threadIndex++], NULL, checkColumn, position);
    }

    /* Create 9 threads to validate rows */
    for(i = 0; i < 9; i++)
    {
		parameters *position = (parameters*) malloc(sizeof(parameters));
        position -> topRow = (i);
        position -> bottomRow = (i);
        position -> leftColumn = (0);
        position -> rightColumn = (8);
        pthread_create(&tid[threadIndex++], NULL, checkRow, position);
    }

    /* Create 9 threads to validate subgrids */
    for(i = 0; i < 9; i++)
    {
		for (j = 0; j < 9; j++) 
		{
            if (i % 3 == 0 && j % 3 == 0) 
			{
                parameters *position = (parameters *) malloc(sizeof(parameters));
                position->topRow = i;
                position->bottomRow = i + 3;
                position->rightColumn = j;
                position->leftColumn = j + 3;
                pthread_create(&tid[threadIndex++], NULL, checkSubgrid, position);
            }
        }
    }

	/* Wait for child threads to exit */
	for (i = 0; i < NUM_CHILD_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    /* Output results */
    printf("\n\tRESULTS:\n\n");

	int valid = TRUE;
    
	for (i = 0; i < 9; i++) 
	{
        if (columns[i] == TRUE) 
        {
            printf("\tColumn:  <%x> is valid.\n", tid_columns[i]);
        }else {
            valid = FALSE;
            printf("\tColumn: <%x> is invalid.\n", tid_columns[i]);
        }
        if (rows[i] == TRUE) 
        {
            printf("\tRow: <%x> is valid.\n", tid_rows[i]);
        }else {
            valid = FALSE;
            printf("\tRow: <%x> is invalid.\n", tid_rows[i]);
        }
        if (subgrids[i] == TRUE) 
        {
            printf("\tGrid: <%x> is valid.\n\n", tid_subgrids[i]);
        }else {
            valid = FALSE;
            printf("\tGrid: <%x> is invalid.\n\n", tid_subgrids[i]);
        }
    }
    
    if (valid == TRUE) 
    {
        printf("\tSudoku puzzle solution is valid!\n");
    }else {
        printf("\tSudoku puzzle solution is invalid!\n");
    }
    
    return EXIT_SUCCESS;
}