#include <stdio.h>
#include <stdlib.h>

// MINESWEEPER
// Written in C by Ben Potter for APSC143
// November 2021

// PROJECT NOTES
// (4, 5) fifth index of fourth row
// 10X10 grid with 10 mines

// Required Functions
// void printGrid()
// int check(int row, int col)
//      check indicated cell, reveal all necessary cells, returns 0 if mine, 1 if OK
// void flag(int row, int col)
// int checkWin()
//      check if the game has been won, return 1 if won, 0 if not

#define SIZE 10
#define MINE_COUNT 10

// state
#define HIDDEN 0
#define NOT_HIDDEN 1
#define FLAGGED 2

#define MINE -1

struct cell {
    int state;
    int contains;
};

struct cell** grid;

void constructGrid();
void destructGrid();
void printGrid();
int check(int row, int col);
void flag(int row, int col);
int checkWin();

int main() {
    srand(2);

    constructGrid();

//    for (int i = 0; i < SIZE; i++) {
//        for (int j = 0; j < SIZE; j++) {
//            grid[i][j].state = NOT_HIDDEN;
//        }
//    }

    printGrid();

    // enter game loop
    int running = 1;
    while(running) {
        // get instruction from the player
        char cmd;
        int row, col;

        printf("Enter ‘c’ for check cell, ‘f’ for flag cell.\n");
        printf("Enter command & cell row col: ");
        scanf("%c %d %d", &cmd, &row, &col);
        printf("\n");

        if (cmd == 'c')
            running = check(row, col);

        printGrid();
    }


    // update win ,lose condition
    printf("You hit a mine, game over.");

    destructGrid();

    return 0;
}

void constructGrid() {

    // create rows
    grid = malloc(SIZE * sizeof(struct cell*));
    for (int i = 0; i < SIZE; i++) {
        // create cols
        grid[i] = malloc(SIZE * sizeof(struct cell));
    }

    // randomly assign mines
    int mines = MINE_COUNT;
    while(mines >= 0) {
        // get random position
        int row = rand() % SIZE;
        int col = rand() % SIZE;

        // if this position already contains a mine, try again
        if (grid[row][col].contains == MINE)
            continue;

        // make this grid cell contain a mine
        grid[row][col].contains = MINE;
        mines--; // placed a mine so subtract from the total
    }

    // TODO update cell values based on amount of adjacent mines
}

void destructGrid() {
    // free the cols
    for (int i = 0; i < SIZE; i++) {
        free(grid[i]);
    }

    // free the rows
    free(grid);
}

void printGrid() {
    // loop over all rows and columns
    for(int i = 0; i < SIZE; i++) {
        char line[11]; // include spaces
        for(int j = 0; j < SIZE; j ++) {
            struct cell c = grid[i][j];
            char cellVal = '*';

            if (c.state == NOT_HIDDEN) {
                if(c.contains == MINE) {
                    cellVal = 'M';
                } else {
                    cellVal = '0' + c.contains;
                }
            } else if (c.state == FLAGGED) {
                cellVal = 'F';
            }

            line[j] = cellVal; // TODO update grid printing to maintain two spaces between cells
            //line[j + 1] = ' ';
            //line[j + 2] = ' ';
        }
        printf("%s \n", line);
    }
}

int check(int row, int col) {
    struct cell c = grid[row][col];

    if (c.state != HIDDEN)
        return 1; // if the mine is not already visible e.g flagged or revealed, then don't update it

    if (c.contains == 0){
        // check adjacent tiles
    }

    // update the checked cell
    grid[row][col].state = NOT_HIDDEN; // NOTE: can't just update cell "c" b/c it wont change the actual grid array, just a copy

    return c.contains != MINE;
}