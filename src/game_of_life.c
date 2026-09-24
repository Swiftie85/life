#include <ncurses.h>
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define INITIAL_SPEED 100

void makeGrid(int grid[HEIGHT][WIDTH]);
void drawGrid(int grid[HEIGHT][WIDTH], int speed);
int countNeighbors(int grid[HEIGHT][WIDTH], int i, int j);
void updateGrid(int grid[HEIGHT][WIDTH]);

int main() {
    int grid[HEIGHT][WIDTH];

    makeGrid(grid);

    if (freopen("/dev/tty", "r", stdin)) {
        initscr();
        cbreak();
        noecho();
        ;
        keypad(stdscr, TRUE);
        curs_set(0);
        int speed = INITIAL_SPEED;
        int result = 0;

        while (result == 0) {
            drawGrid(grid, speed);
            updateGrid(grid);
            timeout(speed);

            int c = getch();
            if ((c == 'a' || c == 'A') && speed > 10) {
                speed -= 10;
            } else if ((c == 'z' || c == 'Z') && speed < 1000) {
                speed += 10;
            } else if (c == ' ') {
                result = 1;
            }
        }
        endwin();
    }
    return 0;
}

void makeGrid(int grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            (scanf("%1d", &grid[i][j]));
        }
    }
}

void drawGrid(int grid[HEIGHT][WIDTH], int speed) {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            mvaddch(i, j, grid[i][j] ? '#' : ' ');
        }
    }
    mvprintw(HEIGHT, 0, "Delay %d ms | A: faster, Z: slower, Space: exit", speed);
    refresh();
}

int countNeighbors(int grid[HEIGHT][WIDTH], int i, int j) {
    int count = 0;
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di != 0 || dj != 0) {
                int neighbor_i = (di + i + HEIGHT) % HEIGHT;
                int neighbor_j = (dj + j + WIDTH) % WIDTH;
                count += grid[neighbor_i][neighbor_j];
            }
        }
    }
    return count;
}

void updateGrid(int grid[HEIGHT][WIDTH]) {
    int nextgrid[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int n = countNeighbors(grid, i, j);
            if (grid[i][j] == 1 && ((n == 2) || (n == 3))) {
                nextgrid[i][j] = 1;
            } else if (grid[i][j] == 0 && n == 3) {
                nextgrid[i][j] = 1;
            } else {
                nextgrid[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = nextgrid[i][j];
        }
    }
}