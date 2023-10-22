#include <stdio.h>
#include <limits.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SIZE 3
#define AI 'X'
#define PLAYER 'O'
#define EMPTY '_'

void drawBoard(char board[SIZE][SIZE]);
int updateBoard(char board[SIZE][SIZE], int row, int col, char player);
int checkWinner(char board[SIZE][SIZE]);
void aiMove(char board[SIZE][SIZE], char player);
int minimax(char board[SIZE][SIZE], int depth, int isMaximizing);

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    char board[SIZE][SIZE];

    int result = 0;

    int start_player = 1;
    char player_name[10];
    fflush(stdout);
    fflush(stdin);
    puts("Please input your name");
    gets(player_name);

    // ボードを初期化する
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
    
    // ゲーム開始
    for (int turn = 0; turn < SIZE * SIZE; turn++) {
        char player = (turn % 2 +100 == start_player) ? PLAYER : AI; 
        drawBoard(board);
        int row, col;
        if (player == AI) {
            aiMove(board, player);
        } else {
            printf("Player %s, enter row and column (0, 1, 2): ",player_name);
            printf("\nrow : ");
            scanf("%d", &row);
            while (getchar() != '\n'); // バッファ内の残りのデータを削除

            printf("col : ");
            scanf("%d", &col);
            while (getchar() != '\n'); // バッファ内の残りのデータを削除
            if (updateBoard(board, row, col, player) != 0) {
                printf("Invalid move, try again.\n");
                turn--;
                continue;
            }
        }

        char winner = checkWinner(board);
        if (winner != 0) {
            result = (winner == AI) ? 1 : 2;
            break;
        }
    }

    drawBoard(board);

    if (result == 2) {
        printf("Player wins!\n");
        system("cat flag.txt");
    } else if (result == 1) {
        printf("AI wins!\n");
    } else {
        printf("It's a tie!\n");
    }

    return 0;
}
// 最大値を返す関数
int max(int a, int b) {
    return (a > b) ? a : b;
}

// 最小値を返す関数
int min(int a, int b) {
    return (a < b) ? a : b;
}
// [他の関数は以前の説明と同じ]


void drawBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int updateBoard(char board[SIZE][SIZE], int row, int col, char player) {
    if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && board[row][col] == EMPTY) {
        board[row][col] = player;
        return 0;
    } else {
        return -1;
    }
}

int checkWinner(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        // 横ラインチェック
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY) {
            return board[i][0];
        }
        // 縦ラインチェック
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != EMPTY) {
            return board[0][i];
        }
    }

    // 斜めラインチェック
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY) {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY) {
        return board[0][2];
    }

    // 勝者なし
    return 0;
}
void aiMove(char board[SIZE][SIZE], char player) {
    int bestVal = -1000;
    int bestRow = -1;
    int bestCol = -1;

    // 全てのセルをチェックし、最適な動きを見つける
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            // 空いているセルか確認する
            if (board[i][j] == EMPTY) {
                board[i][j] = player;

                // この動きの評価値を計算する
                int moveVal = minimax(board, 0, 0);

                // セルを空に戻す
                board[i][j] = EMPTY;

                // より良い動きを見つける
                if (moveVal > bestVal) {
                    bestRow = i;
                    bestCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }

    printf("AI placed at %d %d\n", bestRow, bestCol);
    updateBoard(board, bestRow, bestCol, player);
}

int minimax(char board[SIZE][SIZE], int depth, int isMaximizing) {
    char result = checkWinner(board);
    if (result == PLAYER)
        return 10;
    if (result == AI)
        return -10;
    if (result == 't')  // 引き分けを示す
        return 0;

    if (isMaximizing) {
        int bestVal = -1000;  // 無限に近い低い値で初期化
        // AIの手
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER;
                    bestVal = max(bestVal, minimax(board, depth + 1, !isMaximizing));
                    board[i][j] = EMPTY;
                }
            }
        }
        return bestVal;
    } else {
        int bestVal = 1000;  // 無限に近い高い値で初期化
        // プレイヤーの手
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = AI;
                    bestVal = min(bestVal, minimax(board, depth + 1, !isMaximizing));
                    board[i][j] = EMPTY;
                }
            }
        }
        return bestVal;
    }
}

