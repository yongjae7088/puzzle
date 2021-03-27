#define _CRT_SECURE_NO_WARNINGS
#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

SceneID scene;
ObjectID piece[9], original_piece[9], start;
int blank = 8;

TimerID timer;
int mixCount;

bool game = false;

int indexToX(int i) {
    return 292 + 200 * (i % 3);
}
int indexToY(int i) {
    return 471 - 200 * (i / 3);
}
int game_index(ObjectID object) {
    for (int i = 0 ; i < 9; i++) {
        if (piece[i] == object) return i;
    }
    return -1;
}
void game_move(int i) {
    ObjectID object = piece[i];
    piece[i] = piece[blank];
    locateObject(piece[i], scene, indexToX(i), indexToY(i));
    piece[blank] = object; 
    locateObject(piece[blank], scene, indexToX(blank), indexToY(blank));

    blank = i;
}

bool possible_move(int i) {
    if (i % 3 > 0 && blank == i - 1) return true;
    if (i % 3 < 2 && blank == i + 1) return true;
    if (i / 3 > 0 && blank == i - 3) return true;
    if (i / 3 < 2 && blank == i + 3) return true;

    return false;
}
int random_move() {
    int i = -1;
   
    while (i == -1) {
        switch (rand() % 4) {
        case 0:
            if (blank % 3 > 0) {
                i = blank - 1;      
            }
            break;
        case 1:
            if (blank % 3 < 2) {
                i = blank + 1;              
            }
            break;
        case 2:
            if (blank / 3 > 0) {
                i = blank - 3;               
            }
            break;
        case 3:
            if (blank / 3 < 2) {
                i = blank + 1;               
            }
            break;
        }
    }
    return i;
}
bool game_end() {
    for (int i = 0; i < 9; i++) {
        if (piece[i] != original_piece[i]) return false;
    }
    return true;
}
void game_start() {
    blank = 8;
    hideObject(piece[blank]);

    mixCount = 50;

    setTimer(timer, 0.01f);
    startTimer(timer);
}
void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
    if (game) {
        int i = game_index(object);
        if (possible_move(i)) {
            game_move(i);

            if (game_end()) {
                game = false;
                showObject(start);
                showMessage("completed!");
                showObject(piece[blank]);
            }
        }
    }
    else {
        if (object == start) {
            game_start();
        }
    }
}
ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true) {
    ObjectID object = createObject(image);
    locateObject(object, scene, x, y);
    if (shown) showObject(object);

    return object;
}
void timerCallback(TimerID timer) {
    game_move(random_move());
    mixCount--;

    if (mixCount > 0) {
        setTimer(timer, 0.01f);
        startTimer(timer);
    }
    else {
        game = true;
        hideObject(start);
    }
}

SceneID game_init() {
    scene = createScene("강아지 퍼즐", "puppypuz.png");

    char buf[50];
    for (int i = 0; i < 9; i++) {
        sprintf(buf, "p%d.png", i + 1);
        piece[i] = createObject(buf, scene, indexToX(i), indexToY(i));
        original_piece[i] = piece[i];
    }

    start = createObject("startButton.png", scene, 590, 100);

    timer = createTimer(0.1f);

    return scene;
}
int main()
{
    srand(time(NULL));

    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);

    setMouseCallback(mouseCallback);
    setTimerCallback(timerCallback);

    game_init();

    startGame(scene);
}