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

// state, essentially an enum
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

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if(grid[i][j].contains != MINE)
                grid[i][j].state = NOT_HIDDEN;
        }
    }

    printGrid();

    // enter game loop
    int won = 0;
    int running = 1;
    while(running) {
        // get instruction from the player
        char cmd;
        int row, col;

        printf("Enter ‘c’ for check cell, ‘f’ for flag cell.\n");
        printf("Enter command & cell row col: ");
        scanf("%c %d %d", &cmd, &row, &col);
        printf("\n");

        if (cmd == 'c') {
            running = check(row, col);
        } else if (cmd == 'f') {
            flag(row, col);
        }

        printGrid();
        won = checkWin();
        if (won)
            running = 0;
    }

    printf(won ? "Congratulations! You win!" : "You hit a mine, game over.");

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
    while(mines > 0) {
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

    // update surrounding cells based on mine placement
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            struct cell c = grid[i][j];

            if (c.contains == MINE) {
                // update all adjacent cells
                // determine valid adjacent cells
                for(int g = i-1; g < 3-1 + i; g++) {
                    for(int h = j-1; h < 3-1 + j; h++) {
                        if(g==i && h == j)
                            continue;

                        if (g >= 0 && g < SIZE && h >= 0 && h < SIZE) {
                            if(grid[g][h].contains == MINE)
                                continue;

                            grid[g][h].contains++;
                        }
                    }
                }
            }
        }
    }
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

            printf("%c  ", cellVal);
        }
        printf("\n");
    }
}

void flag(int row, int col) {
    if(grid[row][col].state == NOT_HIDDEN)
        return;

    grid[row][col].state = grid[row][col].state == HIDDEN ? FLAGGED : HIDDEN;
}

int check(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE)
        return 1;

    struct cell c = grid[row][col];

    if (c.state != HIDDEN)
        return 1; // if the mine is not already visible e.g flagged or revealed, then don't update it

    // update the checked cell
    grid[row][col].state = NOT_HIDDEN; // NOTE: can't just update cell "c" b/c it wont change the actual grid array, just a copy

    if (c.contains == 0){
        // check adjacent tiles
        check(row + 1, col + 1);
        check(row + 1, col    );
        check(row + 1, col - 1);
        check(row    , col + 1);
        check(row    , col - 1);
        check(row - 1, col + 1);
        check(row - 1, col + 0);
        check(row - 1, col - 1);
    }

    return c.contains != MINE;
}

int checkWin() {
    int cellsRevealed = 0;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(grid[i][j].state == NOT_HIDDEN)
                cellsRevealed++;
        }
    }

    return cellsRevealed == 100 - 10;
}