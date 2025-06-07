```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#define WIDTH 10
#define HEIGHT 6

char maze[HEIGHT][WIDTH + 1] = {
    "##########",
    "#P   G # #",
    "# ### #  #",
    "# G # ## #",
    "###   E M#",
    "##########"
};

int playerX = 1, playerY = 1;
int monsterX = 8, monsterY = 4;
int hp = 3;
int score = 0;
int highScore = 0;

// 讀取最高分（存在 highscore.txt）
void loadHighScore() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    } else {
        highScore = 0;
    }
}

// 寫入最高分
void saveHighScore() {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}

void printMazeWithUI() {
    system("cls");

    printf("迷宮冒險 v4.0  | 控制：W A S D\n");
    printf("===========================================\n");

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            char c = maze[y][x];
            if (c == '#')       printf("█");
            else if (c == 'P')  printf("@");
            else if (c == 'M')  printf("X");
            else if (c == 'E')  printf("*");
            else if (c == 'G')  printf("$");
            else                printf(" ");
        }
        printf("\n");
    }

    int distance = abs(playerX - monsterX) + abs(playerY - monsterY);
    printf("\n玩家位置：( %d, %d )   ", playerX, playerY);
    printf("怪物距離：%d   ", distance);
    printf("生命值：%d ♥   得分：%d 💎   最高分：%d\n", hp, score, highScore);
    printf("===========================================\n");
}

void moveMonster() {
    int dx = 0, dy = 0;
    if (playerX < monsterX) dx = -1;
    else if (playerX > monsterX) dx = 1;
    else if (playerY < monsterY) dy = -1;
    else if (playerY > monsterY) dy = 1;

    int newX = monsterX + dx;
    int newY = monsterY + dy;

    if (maze[newY][newX] == ' ' || maze[newY][newX] == 'P') {
        maze[monsterY][monsterX] = ' ';
        monsterX = newX;
        monsterY = newY;
        maze[monsterY][monsterX] = 'M';
    }
}

int main() {
    char move;

    loadHighScore(); // 一開始載入最高分

    while (1) {
        printMazeWithUI();
        move = getch();

        int newX = playerX;
        int newY = playerY;

        if (move == 'w' || move == 'W') newY--;
        else if (move == 's' || move == 'S') newY++;
        else if (move == 'a' || move == 'A') newX--;
        else if (move == 'd' || move == 'D') newX++;

        char next = maze[newY][newX];

        if (next == ' ' || next == 'E' || next == 'G') {
            if (next == 'G') {
                score++;
            }
            maze[playerY][playerX] = ' ';
            playerX = newX;
            playerY = newY;
            maze[playerY][playerX] = 'P';
        }

        moveMonster();

        if (playerX == monsterX && playerY == monsterY) {
            hp--;
            if (hp <= 0) {
                printMazeWithUI();
                printf("你被怪物吃掉了！遊戲結束！\n");
                break;
            } else {
                printf("你被怪物咬到！剩餘生命：%d\n", hp);
                getch();
            }
        }

        if (maze[playerY][playerX] == 'E') {
            printMazeWithUI();
            printf("🎉 你成功逃出迷宮！\n");
            printf("得分：%d 💎\n", score);

            if (score > highScore) {
                highScore = score;
                saveHighScore();
                printf("🌟 新紀錄！你創下最高分！🌟\n");
            } else {
                printf("目前最高分：%d 💎\n", highScore);
            }
            break;
        }
    }

    return 0;
}
```
