```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // For getch() on Windows
#include <math.h>  // For abs()
#include <string.h> // For strcpy (for maze copying)

// Define maze dimensions
#define WIDTH 10
#define HEIGHT 6

// Maze map templates - Level 1
// '#' Wall, ' ' Empty space, 'E' Exit, 'G' Treasure
char mazeLevel1_template[HEIGHT][WIDTH + 1] = {
    "##########",
    "#        #",
    "#        #",
    "#        #",
    "#   E    #",
    "##########"
};

// Maze map templates - Level 2 (Harder)
// ? 已修正：設計為比第一關更難，但仍可從 (1,1) 到達出口 (4,4)
char mazeLevel2_template[HEIGHT][WIDTH + 1] = {
    "##########",
    "#        #",
    "#  #     #",
    "#   #    #",
    "#   E    #",
    "##########"
};

// Maze map templates - Level 3 (Even harder)
char mazeLevel3_template[HEIGHT][WIDTH + 1] = {
    "##########",
    "# G # #  #",
    "## ## # ##",
    "# # # #  #",
    "# E # # M#",
    "##########"
};

char currentMaze[HEIGHT][WIDTH + 1];

int playerX, playerY;
int monsterX, monsterY;
int hp;
int score = 0;
int highScore = 0;
int monster_move_counter = 0;
int currentLevel = 1;

void loadHighScore() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    } else {
        highScore = 0;
    }
}

void saveHighScore() {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}

void loadLevel(int level) {
    if (level == 1) {
        for (int y = 0; y < HEIGHT; y++) {
            strcpy(currentMaze[y], mazeLevel1_template[y]);
        }
        playerX = 1; playerY = 1;
        monsterX = 8; monsterY = 4;
        hp = 3;
    } else if (level == 2) {
        for (int y = 0; y < HEIGHT; y++) {
            strcpy(currentMaze[y], mazeLevel2_template[y]);
        }
        playerX = 1; playerY = 1;
        monsterX = 8; monsterY = 3;
        hp = 3;
    } else if (level == 3) {
        for (int y = 0; y < HEIGHT; y++) {
            strcpy(currentMaze[y], mazeLevel3_template[y]);
        }
        playerX = 1; playerY = 1;
        monsterX = 8; monsterY = 4;
        hp = 3;
    }
    monster_move_counter = 0;
}

void printMazeWithUI() {
    system("cls");
    printf("Maze Adventure v4.0  | Controls: W A S D\n");
    printf("===========================================\n");
    printf("LEVEL: %d\n", currentLevel);
    printf("===========================================\n");

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY) {
                printf("@");
            } else if (x == monsterX && y == monsterY) {
                printf("X");
            } else {
                char c = currentMaze[y][x];
                if (c == '#') printf("#");
                else if (c == 'E') printf("*");
                else if (c == 'G') printf("$");
                else printf(" ");
            }
        }
        printf("\n");
    }

    int distance = abs(playerX - monsterX) + abs(playerY - monsterY);
    printf("\nPlayer: ( %d, %d )   ", playerX, playerY);
    printf("Monster Distance: %d   ", distance);
    printf("HP: %d   Score: %d   High Score: %d\n", hp, score, highScore);
    printf("===========================================\n");
}

int canMoveTo(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;
    return (currentMaze[y][x] != '#');
}

int movePlayer(int dx, int dy) {
    int newX = playerX + dx;
    int newY = playerY + dy;
    if (!canMoveTo(newX, newY)) return 0;

    char nextCellContent = currentMaze[newY][newX];
    if (nextCellContent == 'G') {
        score++;
        currentMaze[newY][newX] = ' ';
    }

    playerX = newX;
    playerY = newY;
    return 1;
}

void moveMonster() {
    monster_move_counter++;
    if (monster_move_counter % 2 != 0) return;

    int dx = 0, dy = 0;
    if (playerX < monsterX) dx = -1;
    else if (playerX > monsterX) dx = 1;
    else if (playerY < monsterY) dy = -1;
    else if (playerY > monsterY) dy = 1;

    int newX = monsterX + dx;
    int newY = monsterY + dy;

    if (canMoveTo(newX, newY) && currentMaze[newY][newX] != 'E') {
        monsterX = newX;
        monsterY = newY;
    }
}

int checkMonsterAttack() {
    if (playerX == monsterX && playerY == monsterY) {
        hp--;
        if (hp <= 0) {
            printMazeWithUI();
            printf("You were eaten by the monster! Game Over!\n");
            return 1;
        } else {
            printf("You were bitten by the monster! Remaining HP: %d\n", hp);
            getch();
        }
    }
    return 0;
}

int checkWin() {
    if (currentMaze[playerY][playerX] == 'E') {
        printMazeWithUI();
        printf("You escaped the maze!\n");
        printf("Score: %d\n", score);
        if (score > highScore) {
            highScore = score;
            saveHighScore();
            printf("New record! You set a new high score!\n");
        } else {
            printf("Current high score: %d\n", highScore);
        }
        return 1;
    }
    return 0;
}

int main() {
    loadHighScore();
    loadLevel(1);

    while (1) {
        printMazeWithUI();
        char move = getch();
        int dx = 0, dy = 0;

        if (move == 'w' || move == 'W') dy = -1;
        else if (move == 's' || move == 'S') dy = 1;
        else if (move == 'a' || move == 'A') dx = -1;
        else if (move == 'd' || move == 'D') dx = 1;

        movePlayer(dx, dy);

        if (checkWin()) {
            if (currentLevel < 3) {
                currentLevel++;
                printf("\n關卡 %d 完成！準備進入關卡 %d...\n", currentLevel - 1, currentLevel);
                getch();
                loadLevel(currentLevel);
                continue;
            } else {
                printf("恭喜！您完成了所有關卡！你超強!\n");
                getch();
                break;
            }
        }

        moveMonster();
        if (checkMonsterAttack()) break;
    }

    return 0;
}
```
