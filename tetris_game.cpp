#include <iostream>

#include <conio.h>

#include <windows.h>

#include <vector>

#include <ctime>



using namespace std;



const int WIDTH = 10;

const int HEIGHT = 20;

const char EMPTY = '.';

const char FILLED = '#';



char board[HEIGHT][WIDTH];



struct Tetromino {

    vector<vector<int>> shape;

    int x, y;

};



vector<Tetromino> tetrominoes = {

    {{ {1, 1, 1, 1} }},

    {{ {1, 1}, {1, 1} }},

    {{ {0, 1, 0}, {1, 1, 1} }},

    {{ {1, 1, 0}, {0, 1, 1} }},

    {{ {0, 1, 1}, {1, 1, 0} }},

    {{ {1, 0, 0}, {1, 1, 1} }},

    {{ {0, 0, 1}, {1, 1, 1} }}

};



Tetromino current;



void InitializeBoard() {

    for (int i = 0; i < HEIGHT; i++) {

        for (int j = 0; j < WIDTH; j++) {

            board[i][j] = EMPTY;

        }

    }

}



void DrawBoard() {

    system("cls");

    char tempBoard[HEIGHT][WIDTH];

    memcpy(tempBoard, board, sizeof(board));

    

    for (int i = 0; i < current.shape.size(); i++) {

        for (int j = 0; j < current.shape[i].size(); j++) {

            if (current.shape[i][j] == 1)

                tempBoard[current.y + i][current.x + j] = FILLED;

        }

    }

    

    for (int i = 0; i < HEIGHT; i++) {

        for (int j = 0; j < WIDTH; j++) {

            cout << tempBoard[i][j] << " ";

        }

        cout << endl;

    }

}



bool IsValidMove(int dx, int dy, const vector<vector<int>>& shape) {

    for (int i = 0; i < shape.size(); i++) {

        for (int j = 0; j < shape[i].size(); j++) {

            if (shape[i][j] == 1) {

                int newX = current.x + j + dx;

                int newY = current.y + i + dy;

                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || (newY >= 0 && board[newY][newX] == FILLED))

                    return false;

            }

        }

    }

    return true;

}



void RotateTetromino() {

    vector<vector<int>> rotated;

    int rows = current.shape.size();

    int cols = current.shape[0].size();

    rotated.resize(cols, vector<int>(rows, 0));

    for (int i = 0; i < rows; i++) {

        for (int j = 0; j < cols; j++) {

            rotated[j][rows - i - 1] = current.shape[i][j];

        }

    }

    if (IsValidMove(0, 0, rotated)) {

        current.shape = rotated;

    }

}



void PlaceTetromino() {

    for (int i = 0; i < current.shape.size(); i++) {

        for (int j = 0; j < current.shape[i].size(); j++) {

            if (current.shape[i][j] == 1)

                board[current.y + i][current.x + j] = FILLED;

        }

    }

}



void SpawnTetromino() {

    current = tetrominoes[rand() % tetrominoes.size()];

    current.x = WIDTH / 2 - current.shape[0].size() / 2;

    current.y = 0;

    if (!IsValidMove(0, 0, current.shape)) {

        cout << "Game Over!" << endl;

        exit(0);

    }

}



void MoveTetromino(int dx, int dy) {

    if (IsValidMove(dx, dy, current.shape)) {

        current.x += dx;

        current.y += dy;

    }

}



void HandleInput() {

    while (_kbhit()) {

        switch (_getch()) {

            case 75: MoveTetromino(-1, 0); break;

            case 77: MoveTetromino(1, 0); break;

            case 80: MoveTetromino(0, 1); break;

            case 72: RotateTetromino(); break;

            case 32: while (IsValidMove(0, 1, current.shape)) MoveTetromino(0, 1); break;

            case 27: exit(0);

        }

    }

}



void ClearLines() {

    for (int i = HEIGHT - 1; i >= 0; i--) {

        bool full = true;

        for (int j = 0; j < WIDTH; j++) {

            if (board[i][j] == EMPTY) {

                full = false;

                break;

            }

        }

        if (full) {

            for (int k = i; k > 0; k--) {

                for (int j = 0; j < WIDTH; j++) {

                    board[k][j] = board[k - 1][j];

                }

            }

            for (int j = 0; j < WIDTH; j++) {

                board[0][j] = EMPTY;

            }

            i++;

        }

    }

}



void GameLoop() {

    InitializeBoard();

    SpawnTetromino();

    while (true) {

        DrawBoard();

        HandleInput();

        Sleep(500);

        if (IsValidMove(0, 1, current.shape))

            MoveTetromino(0, 1);

        else {

            PlaceTetromino();

            ClearLines();

            SpawnTetromino();

        }

    }

}



int main() {

    srand(time(0));

    GameLoop();

    return 0;

}