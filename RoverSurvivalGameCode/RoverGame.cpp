#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
using namespace std;

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void moveCursorToTop() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void showTitleScreen() {
    system("cls");
    cout << "  ================================================" << endl;
    cout << "  |         ROVER  SURVIVAL                      |" << endl;
    cout << "  |                                              |" << endl;
    cout << "  |   WASD      = Move                           |" << endl;
    cout << "  |   SPACE     = Shoot laser                    |" << endl;
    cout << "  |   Hide near # rocks during storms            |" << endl;
    cout << "  |   Collect * energy to survive                |" << endl;
    cout << "  |   Shoot <E> enemies before they reach you    |" << endl;
    cout << "  |                                              |" << endl;
    cout << "  |         Press ENTER to start                 |" << endl;
    cout << "  ================================================" << endl;
    cin.get();
}

bool showGameOver(int minutes, int seconds) {
    system("cls");
    cout << endl;
cout << endl;
cout << "##############################" << endl;
cout << "#                            #" << endl;
cout << "#      G A M E  O V E R      #" << endl;
cout << "#                            #" << endl;
cout << "##############################" << endl;
    cout << endl;
    cout << "  You survived: " << minutes << "m " << seconds << "s" << endl;
    cout << endl;
    cout << "  Press R to play again or Q to quit! ";
    while(true) {
        char c = _getch();
        if(c == 'r' || c == 'R') return true;
        if(c == 'q' || c  == 'Q') return false;
    }
}

class Grid {
private:
    int width, height;
    bool rocks[13][26];
    bool energy[13][26];

public:
    Grid(int w, int h) {
        width = w;
        height = h;
        for (int row = 0; row < height; row++)
            for (int col = 0; col < width; col++) {
                rocks[row][col] = false;
                energy[row][col] = false;
            }
    }

    void placeRock(int x, int y)     { rocks[y][x] = true; }
    bool isRock(int x, int y)        { return rocks[y][x]; }
    void placeEnergy(int x, int y)   { energy[y][x] = true; }
    bool hasEnergy(int x, int y)     { return energy[y][x]; }
    void collectEnergy(int x, int y) { energy[y][x] = false; }
    int getWidth()                   { return width; }
    int getHeight()                  { return height; }

    void draw(
        int roverX, int roverY, char direction,
        int enemyX[], int enemyY[], bool enemyAlive[], char enemyDir[],
        int laserX, int laserY, bool laserActive, char laserDir,
        int elX[], int elY[], bool elActive[], char elDir[],
        int enemyCount,
        int hitX, int hitY, bool showHit
    ) {
        for (int row = 0; row < height; row++) {
            for (int line = 0; line < 3; line++) {
                for (int col = 0; col < width; col++) {

                    bool isEnemyHere = false;
                    char ed = 'R';
                    for (int i = 0; i < enemyCount; i++) {
                        if (enemyAlive[i] && enemyX[i] == col && enemyY[i] == row) {
                            isEnemyHere = true;
                            ed = enemyDir[i];
                            break;
                        }
                    }

                    bool isEnemyLaserHere = false;
                    char eld = 'R';
                    for (int i = 0; i < enemyCount; i++) {
                        if (elActive[i] && elX[i] == col && elY[i] == row) {
                            isEnemyLaserHere = true;
                            eld = elDir[i];
                            break;
                        }
                    }

                    bool isLaserHere = laserActive && laserX == col && laserY == row;
                    bool isHitHere   = showHit && hitX == col && hitY == row;

                    if (col == roverX && row == roverY) {
                        if (direction == 'U') {
                            if (line == 0) cout << "  ^   ";
                            if (line == 1) cout << " |R|  ";
                            if (line == 2) cout << "  |   ";
                        }
                        else if (direction == 'D') {
                            if (line == 0) cout << "  |   ";
                            if (line == 1) cout << " |R|  ";
                            if (line == 2) cout << "  v   ";
                        }
                        else if (direction == 'L') {
                            if (line == 0) cout << " /-\\  ";
                            if (line == 1) cout << "<-|R| ";
                            if (line == 2) cout << " \\-/  ";
                        }
                        else if (direction == 'R') {
                            if (line == 0) cout << " /-\\  ";
                            if (line == 1) cout << " |R|->";  
                            if (line == 2) cout << " \\-/  ";
                        }
                    }

                    else if (isHitHere) {
                        if (line == 0) cout << "  *   ";
                        if (line == 1) cout << "*[X]* ";
                        if (line == 2) cout << "  *   ";
                    }

                    else if (isLaserHere) {
                        if (laserDir == 'L' || laserDir == 'R') {
                            if (line == 0) cout << "      ";
                            if (line == 1) cout << " ---  ";
                            if (line == 2) cout << "      ";
                        }
                        else {
                            if (line == 0) cout << "  |   ";
                            if (line == 1) cout << "  |   ";
                            if (line == 2) cout << "  |   ";
                        }
                    }

                    else if (isEnemyLaserHere) {
                        if (eld == 'L' || eld == 'R') {
                            if (line == 0) cout << "      ";
                            if (line == 1) cout << " ~~~  ";
                            if (line == 2) cout << "      ";
                        }
                        else {
                            if (line == 0) cout << "  !   ";
                            if (line == 1) cout << "  !   ";
                            if (line == 2) cout << "  !   ";
                        }
                    }

                    else if (isEnemyHere) {
                        if (ed == 'U') {
                            if (line == 0) cout << "  ^   ";
                            if (line == 1) cout << " |E|  ";
                            if (line == 2) cout << "  |   ";
                        }
                        else if (ed == 'D') {
                            if (line == 0) cout << "  |   ";
                            if (line == 1) cout << " |E|  ";
                            if (line == 2) cout << "  v   ";
                        }
                        else if (ed == 'L') {
                            if (line == 0) cout << " /-\\  ";
                            if (line == 1) cout << "<-|E| ";
                            if (line == 2) cout << " \\-/  ";
                        }
                        else if (ed == 'R') {
                            if (line == 0) cout << " /-\\  ";
                           if (line == 1) cout << " |E|->";  // 6 chars
                            if (line == 2) cout << " \\-/  ";
                        }
                    }

                    else if (isRock(col, row)) {
                        if (line == 0) cout << " /-\\  ";
                        if (line == 1) cout << "|###| ";
                        if (line == 2) cout << " \\-/  ";
                    }

                    else if (hasEnergy(col, row)) {
                        if (line == 0) cout << "      ";
                        if (line == 1) cout << "  *   ";
                        if (line == 2) cout << "      ";
                    }

                    else {
                        if (line == 0) cout << "      ";
                        if (line == 1) cout << "      ";
                        if (line == 2) cout << "      ";
                    }
                }
                cout << endl;
            }
        }
    }
};

class Rover {
private:
    int x, y;
    int gridWidth, gridHeight;
    char direction;
    int energy;

public:
    Rover(int startX, int startY, int gw, int gh) {
        x = startX;
        y = startY;
        gridWidth = gw;
        gridHeight = gh;
        direction = 'R';
        energy = 100;
    }

    void checkEnergy(Grid& grid) {
        if (grid.hasEnergy(x, y)) {
            grid.collectEnergy(x, y);
            energy += 30;
            if (energy > 100) energy = 100;
        }
    }

    void moveUp(Grid& grid) {
        if (y > 0 && !grid.isRock(x, y-1)) {
            y--; direction = 'U'; energy -= 1;
            checkEnergy(grid);
        }
    }
    void moveDown(Grid& grid) {
        if (y < gridHeight-1 && !grid.isRock(x, y+1)) {
            y++; direction = 'D'; energy -= 1;
            checkEnergy(grid);
        }
    }
    void moveLeft(Grid& grid) {
        if (x > 0 && !grid.isRock(x-1, y)) {
            x--; direction = 'L'; energy -= 1;
            checkEnergy(grid);
        }
    }
    void moveRight(Grid& grid) {
        if (x < gridWidth-1 && !grid.isRock(x+1, y)) {
            x++; direction = 'R'; energy -= 1;
            checkEnergy(grid);
        }
    }

    bool isNearRock(Grid& grid) {
        if (y > 0 && grid.isRock(x, y-1))            return true;
        if (y < gridHeight-1 && grid.isRock(x, y+1)) return true;
        if (x > 0 && grid.isRock(x-1, y))            return true;
        if (x < gridWidth-1 && grid.isRock(x+1, y))  return true;
        return false;
    }

    void takeDamage(int amount) {
        energy -= amount;
        if (energy < 0) energy = 0;
    }

    int getX()          { return x; }
    int getY()          { return y; }
    char getDirection() { return direction; }
    int getEnergy()     { return energy; }
};

class Enemy {
private:
    int x, y;
    bool alive;
    char direction;

public:
    Enemy(int startX, int startY) {
        x = startX;
        y = startY;
        alive = true;
        direction = 'R';
    }

    Enemy() {
        x = 0; y = 0;
        alive = false;
        direction = 'R';
    }

    void move(int roverX, int roverY, Grid& grid) {
        if (!alive) return;
        int newX = x, newY = y;

        if      (roverX > x) { newX++; direction = 'R'; }
        else if (roverX < x) { newX--; direction = 'L'; }
        else if (roverY > y) { newY++; direction = 'D'; }
        else if (roverY < y) { newY--; direction = 'U'; }

        if (!grid.isRock(newX, newY)) { x = newX; y = newY; }
    }

    void kill()         { alive = false; }
    bool isAlive()      { return alive; }
    int getX()          { return x; }
    int getY()          { return y; }
    char getDirection() { return direction; }
};

class Laser {
private:
    int x, y;
    char direction;
    bool active;

public:
    Laser() {
        x = 0; y = 0;
        direction = 'R';
        active = false;
    }

    void fire(int startX, int startY, char dir) {
        if (active) return;
        x = startX;
        y = startY;
        direction = dir;
        active = true;
    }

    void move(Grid& grid) {
        if (!active) return;

        if      (direction == 'U') y--;
        else if (direction == 'D') y++;
        else if (direction == 'L') x--;
        else if (direction == 'R') x++;

        if (x < 0 || x >= grid.getWidth() || y < 0 || y >= grid.getHeight()) {
            active = false;
            return;
        }
        if (grid.isRock(x, y)) active = false;
    }

    bool isActive()     { return active; }
    int getX()          { return x; }
    int getY()          { return y; }
    char getDirection() { return direction; }
};

class EnemyLaser {
private:
    int x, y;
    char direction;
    bool active;

public:
    EnemyLaser() {
        x = 0; y = 0;
        direction = 'R';
        active = false;
    }

    void fire(int startX, int startY, int roverX, int roverY) {
        if (active) return;
        x = startX;
        y = startY;
        active = true;
        if      (roverX > startX) direction = 'R';
        else if (roverX < startX) direction = 'L';
        else if (roverY > startY) direction = 'D';
        else                      direction = 'U';
    }

    void move(Grid& grid) {
        if (!active) return;

        if      (direction == 'U') y--;
        else if (direction == 'D') y++;
        else if (direction == 'L') x--;
        else if (direction == 'R') x++;

        if (x < 0 || x >= grid.getWidth() || y < 0 || y >= grid.getHeight()) {
            active = false;
            return;
        }
        if (grid.isRock(x, y)) active = false;
    }

    bool isActive()     { return active; }
    void deactivate()   { active = false; }
    int getX()          { return x; }
    int getY()          { return y; }
    char getDirection() { return direction; }
};

void drawEnergyBar(int energy) {
    int bars = energy / 10;
    cout << "Energy: [";
    for (int i = 0; i < 10; i++) {
        if (i < bars) cout << "#";
        else          cout << " ";
    }
    cout << "] " << energy << "   " << endl;
}

void drawSurvivalTime(time_t startTime) {
    int total   = (int)(time(0) - startTime);
    int minutes = total / 60;
    int seconds = total % 60;
    cout << "Survived: " << minutes << "m " << seconds << "s   " << endl;
}

void stormSweep(Grid& grid, Rover& rover, Enemy enemies[], int enemyCount) {
    int width  = grid.getWidth();
    int height = grid.getHeight();

    for (int sweepRow = 0; sweepRow < height; sweepRow++) {
        char c = sweepRow % 2 == 0 ? '/' : '\\';
        moveCursorToTop();

        for (int row = 0; row < height; row++) {
            for (int line = 0; line < 3; line++) {
                for (int col = 0; col < width; col++) {

                    if (row == sweepRow) {
                        if (grid.isRock(col, row)) {
                            if (line == 0) cout << " /-\\  ";
                            if (line == 1) cout << "|###| ";
                            if (line == 2) cout << " \\-/  ";
                        }
                        else {
                            if (line == 0) cout << "  " << c << "   ";
                            if (line == 1) cout << "  " << c << "   ";
                            if (line == 2) cout << "  " << c << "   ";
                        }
                    }

                    else if (col == rover.getX() && row == rover.getY()) {
                        char d = rover.getDirection();
                        if (d == 'U') {
                            if (line == 0) cout << "  ^   ";
                            if (line == 1) cout << " |R|  ";
                            if (line == 2) cout << "  |   ";
                        }
                        else if (d == 'D') {
                            if (line == 0) cout << "  |   ";
                            if (line == 1) cout << " |R|  ";
                            if (line == 2) cout << "  v   ";
                        }
                        else if (d == 'L') {
                            if (line == 0) cout << " /-\\  ";
                            if (line == 1) cout << "<-|R| ";
                            if (line == 2) cout << " \\-/  ";
                        }
                        else if (d == 'R') {
                            if (line == 0) cout << " /-\\  ";
                            if (line == 1) cout << " |R|->";  // 6 chars
                            if (line == 2) cout << " \\-/  ";
                        }
                    }

                    else if (grid.isRock(col, row)) {
                        if (line == 0) cout << " /-\\  ";
                        if (line == 1) cout << "|###| ";
                        if (line == 2) cout << " \\-/  ";
                    }

                    else if (grid.hasEnergy(col, row)) {
                        if (line == 0) cout << "      ";
                        if (line == 1) cout << "  *   ";
                        if (line == 2) cout << "      ";
                    }

                    else {
                        if (line == 0) cout << "      ";
                        if (line == 1) cout << "      ";
                        if (line == 2) cout << "      ";
                    }
                }
                cout << endl;
            }
        }
        cout << "!! STORM SWEEPING !!                 " << endl;
        Sleep(300);
    }
}

void setFullscreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // press F11 to trigger native fullscreen
    keybd_event(VK_F11, 0, 0, 0);
    Sleep(200);

    // set font size
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize       = sizeof(fontInfo);
    fontInfo.nFont        = 0;
   fontInfo.dwFontSize.X = 6;
fontInfo.dwFontSize.Y = 10;
    fontInfo.FontFamily   = FF_DONTCARE;
    fontInfo.FontWeight   = FW_NORMAL;
    wcscpy_s(fontInfo.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

    // large buffer so no scrollbar appears
    COORD bufferSize = {400, 400};
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    Sleep(100);
}
void runGame() {
    srand(time(0));

    Grid grid(26, 13);
    Rover rover(13, 7, grid.getWidth(), grid.getHeight());

    grid.placeRock(5,  5);
grid.placeRock(6,  5);
grid.placeRock(7,  5);
grid.placeRock(12, 10);
grid.placeRock(3,  10);
grid.placeRock(15, 3);
grid.placeRock(10, 12);

grid.placeEnergy(1,  1);
grid.placeEnergy(18, 2);
grid.placeEnergy(0,  7);
grid.placeEnergy(14, 4);
grid.placeEnergy(19, 14);
grid.placeEnergy(2,  13);
grid.placeEnergy(17, 8);
grid.placeEnergy(8,  11);
grid.placeEnergy(4,  6);
grid.placeEnergy(16, 1);



    const int enemyCount = 12;
    Enemy enemies[enemyCount];
   enemies[0] = Enemy(0,  0);
enemies[1] = Enemy(25, 0);
enemies[2] = Enemy(0,  12);
enemies[3] = Enemy(25, 12);
    Laser laser;
    EnemyLaser enemyLasers[enemyCount];

    int  enemyX[enemyCount];
    int  enemyY[enemyCount];
    bool enemyAlive[enemyCount];
    char enemyDir[enemyCount];
    int  elX[enemyCount];
    int  elY[enemyCount];
    bool elActive[enemyCount];
    char elDir[enemyCount];

    int   hitX    = -1, hitY = -1;
    DWORD hitTime = 0;

    bool   stormWarning       = false;
    time_t warningTime;
    time_t lastEnemyMove      = time(0);
    time_t lastStorm          = time(0);
    time_t lastEnemySpawn     = time(0);
    time_t lastEnergySpawn    = time(0);
    time_t lastEnemyShoot     = time(0);
    time_t startTime          = time(0);
    DWORD  lastLaserMove      = GetTickCount();
    DWORD  lastEnemyLaserMove = GetTickCount();

    char input;

    while (true) {

        for (int i = 0; i < enemyCount; i++) {
            enemyX[i]     = enemies[i].getX();
            enemyY[i]     = enemies[i].getY();
            enemyAlive[i] = enemies[i].isAlive();
            enemyDir[i]   = enemies[i].getDirection();
            elX[i]        = enemyLasers[i].getX();
            elY[i]        = enemyLasers[i].getY();
            elActive[i]   = enemyLasers[i].isActive();
            elDir[i]      = enemyLasers[i].getDirection();
        }

        bool showHit = (hitX != -1 && (int)(GetTickCount() - hitTime) < 300);
        if (!showHit) { hitX = -1; hitY = -1; }

        moveCursorToTop();
        grid.draw(
            rover.getX(), rover.getY(), rover.getDirection(),
            enemyX, enemyY, enemyAlive, enemyDir,
            laser.getX(), laser.getY(), laser.isActive(), laser.getDirection(),
            elX, elY, elActive, elDir,
            enemyCount,
            hitX, hitY, showHit
        );

        drawEnergyBar(rover.getEnergy());
        drawSurvivalTime(startTime);

        if (stormWarning) {
            int secondsLeft = 10 - (int)(time(0) - warningTime);
            char spin = (time(0) % 2 == 0) ? '/' : '\\';
            if (secondsLeft > 0) {
                cout << spin << spin << " STORM INCOMING IN " << secondsLeft << " SECONDS " << spin << spin << "   " << endl;
            } else {
                stormWarning = false;
                stormSweep(grid, rover, enemies, enemyCount);

                for (int i = 0; i < enemyCount; i++) {
                    enemies[i].kill();
                    enemyLasers[i].deactivate();
                }

             enemies[0] = Enemy(0,  0);
enemies[1] = Enemy(25, 0);
enemies[2] = Enemy(0,  12);
enemies[3] = Enemy(25, 12);

                int spawned = 0;
                while (spawned < 3) {
                    int ex = rand() % 10;
                    int ey = rand() % 10;
                    if (!grid.isRock(ex, ey) && !grid.hasEnergy(ex, ey) &&
                        !(ex == rover.getX() && ey == rover.getY())) {
                        grid.placeEnergy(ex, ey);
                        spawned++;
                    }
                }

                if (!rover.isNearRock(grid)) {
                    rover.takeDamage(75);
                    cout << "!! STORM HIT! LOST 75 ENERGY !!      " << endl;
                } else {
                    cout << "!! STORM HIT! ROCK PROTECTED YOU !!  " << endl;
                }
            }
        } else {
            cout << "                                     " << endl;
        }

        cout << "Move WASD | SPACE to shoot | Q to quit:   " << endl;

        if (rover.getEnergy() <= 0) break;

        if ((int)(time(0) - lastEnemyMove) >= 1) {
            for (int i = 0; i < enemyCount; i++) {
                enemies[i].move(rover.getX(), rover.getY(), grid);
                if (enemies[i].isAlive() &&
                    enemies[i].getX() == rover.getX() &&
                    enemies[i].getY() == rover.getY()) {
                    rover.takeDamage(20);
                    hitX    = rover.getX();
                    hitY    = rover.getY();
                    hitTime = GetTickCount();
                }
            }
            lastEnemyMove = time(0);
        }

        if ((int)(GetTickCount() - lastLaserMove) >= 100) {
            if (laser.isActive()) {
                laser.move(grid);
                for (int i = 0; i < enemyCount; i++) {
                    if (enemies[i].isAlive() &&
                        enemies[i].getX() == laser.getX() &&
                        enemies[i].getY() == laser.getY()) {
                        enemies[i].kill();
                        hitX    = laser.getX();
                        hitY    = laser.getY();
                        hitTime = GetTickCount();
                    }
                }
            }
            lastLaserMove = GetTickCount();
        }

        if ((int)(time(0) - lastEnemyShoot) >= 3) {
            for (int i = 0; i < enemyCount; i++) {
                if (enemies[i].isAlive() && !enemyLasers[i].isActive()) {
                    enemyLasers[i].fire(
                        enemies[i].getX(), enemies[i].getY(),
                        rover.getX(), rover.getY()
                    );
                }
            }
            lastEnemyShoot = time(0);
        }

        if ((int)(GetTickCount() - lastEnemyLaserMove) >= 150) {
            for (int i = 0; i < enemyCount; i++) {
                enemyLasers[i].move(grid);
                if (enemyLasers[i].isActive() &&
                    enemyLasers[i].getX() == rover.getX() &&
                    enemyLasers[i].getY() == rover.getY()) {
                    rover.takeDamage(2);
                    hitX    = rover.getX();
                    hitY    = rover.getY();
                    hitTime = GetTickCount();
                    enemyLasers[i].deactivate();
                }
            }
            lastEnemyLaserMove = GetTickCount();
        }

        bool allDead = true;
        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i].isAlive()) { allDead = false; break; }
        }
        if (allDead) {
            if (allDead) {
    enemies[0] = Enemy(0,  0);
    enemies[1] = Enemy(25, 0);
    enemies[2] = Enemy(0,  12);
    enemies[3] = Enemy(25, 12);
}
        }

        if (!stormWarning && (int)(time(0) - lastStorm) >= 60) {
            stormWarning = true;
            warningTime  = time(0);
            lastStorm    = time(0);
        }

        if ((int)(time(0) - lastEnemySpawn) >= 20) {
            int ex = rand() % 10;
            int ey = rand() % 10;
            if (!grid.isRock(ex, ey) && !(ex == rover.getX() && ey == rover.getY())) {
                for (int i = 0; i < enemyCount; i++) {
                    if (!enemies[i].isAlive()) {
                        enemies[i] = Enemy(ex, ey);
                        break;
                    }
                }
            }
            lastEnemySpawn = time(0);
        }

        if ((int)(time(0) - lastEnergySpawn) >= 15) {
            int ex = rand() % 10;
            int ey = rand() % 10;
            if (!grid.isRock(ex, ey) && !grid.hasEnergy(ex, ey) &&
                !(ex == rover.getX() && ey == rover.getY())) {
                grid.placeEnergy(ex, ey);
            }
            lastEnergySpawn = time(0);
        }

        if (_kbhit()) {
            input = _getch();
            if      (input == 'w' || input == 'W') rover.moveUp(grid);
            else if (input == 's' || input == 'S') rover.moveDown(grid);
            else if (input == 'a' || input == 'A') rover.moveLeft(grid);
            else if (input == 'd' || input == 'D') rover.moveRight(grid);
            else if (input == ' ') laser.fire(rover.getX(), rover.getY(), rover.getDirection());
            else if (input == 'q' || input == 'Q') return;
        }

        Sleep(50);
    }

    int total   = (int)(time(0) - startTime);
    int minutes = total / 60;
    int seconds = total % 60;

    if (showGameOver(minutes, seconds)) {
        runGame();
    }
}

int main() {
    setFullscreen();
    hideCursor();
    showTitleScreen();
    runGame();
    system("cls");
    cout << "Thanks for playing Rover Survival!" << endl;
    cout<<" "<<endl;
    cout << "Press F11 to Exit Fullscreen Mode" << endl;
    cout<<" "<<endl;
    cout<<"Developed By Ahnaf Muttaqui Anuprov, ETE'24, RUET"<<endl;
    return 0;
}