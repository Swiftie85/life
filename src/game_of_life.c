#include <ncurses.h>
#include <stdio.h>

#define ROWS 25
#define COLS 80

int wrap(int index, int size)
{
    int result;

    if (index < 0) {
        result = size - 1;
    } else if (index >= size) {
        result = 0;
    } else {
        result = index;
    }
    return result;
}

void readGrid(int grid[ROWS][COLS])
{
    int row, col, symbol;

    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            symbol = getchar();
            while (symbol == '\n' || symbol == '\r')
                symbol = getchar();
            if (symbol == '1' || symbol == '*')
                grid[row][col] = 1;
            else
                grid[row][col] = 0;
        }
    }
}

int countNeighbors(int grid[ROWS][COLS], int row, int col)
{
    int total, dr, dc, r, c;

    total = 0;
    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            if (dr != 0 || dc != 0) {
                r = wrap(row + dr, ROWS);
                c = wrap(col + dc, COLS);
                total = total + grid[r][c];
            }
        }
    }
    return total;
}

void updateGrid(int grid[ROWS][COLS])
{
    int next[ROWS][COLS];
    int row, col, alive, neighbors;

    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            alive = grid[row][col];
            neighbors = countNeighbors(grid, row, col);
            if (alive) {
                if (neighbors == 2 || neighbors == 3)
                    next[row][col] = 1;
                else
                    next[row][col] = 0;
            } else {
                if (neighbors == 3)
                    next[row][col] = 1;
                else
                    next[row][col] = 0;
            }
        }
    }
    for (row = 0; row < ROWS; row++)
        for (col = 0; col < COLS; col++)
            grid[row][col] = next[row][col];
}

void drawGrid(int grid[ROWS][COLS], long generation, int delay_ms)
{
    int row, col;

    erase();
    mvprintw(0, 0, "Generation: %ld  Delay: %dms  [a/z speed, space quit]",
              generation, delay_ms);
    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            if (grid[row][col])
                mvaddch(row + 1, col, 'O');
            else
                mvaddch(row + 1, col, '.');
        }
    }
    refresh();
}

int handleKey(int key, int *delay_ms)
{
    int running;

    running = 1;
    if (key == ' ') {
        running = 0;
    } else {
        if ((key == 'a' || key == 'A') && *delay_ms > 20)
            *delay_ms = *delay_ms - 20;
        if ((key == 'z' || key == 'Z') && *delay_ms < 1000)
            *delay_ms = *delay_ms + 20;
        timeout(*delay_ms);
    }
    return running;
}

int main(void)
{
    int grid[ROWS][COLS];
    long generation;
    int delay_ms, key, running;

    readGrid(grid);
    freopen("/dev/tty", "r", stdin);

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    generation = 0;
    delay_ms = 200;
    running = 1;
    timeout(delay_ms);

    while (running) {
        drawGrid(grid, generation, delay_ms);
        updateGrid(grid);
        generation = generation + 1;

        key = getch();
        if (key != ERR)
            running = handleKey(key, &delay_ms);
    }

    endwin();
    return 0;
}
