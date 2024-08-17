#include <iostream>
#include <vector>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <utility>
using namespace std;

enum Dir { STOP = 0, RIGHT, LEFT, UP, DOWN };

class LogicGame;

class player {
private:
    int score;
    bool lose;
public:
    player() : score(0), lose(false) {}
    int getScore() const {
        return score;
    }

    void setScore(int newScore) {
        score = newScore;
    }

    bool hasLost() const {
        return lose;
    }

    void setLose(bool hasLost) {
        lose = hasLost;
    }
};

class board {
private:
    int height, width;
public:
    board() : height(14), width(28) {}
    int getHeight() const {
        return height;
    }

    void setHeight(int newHeight) {
        height = newHeight;
    }

    int getWidth() const {
        return width;
    }

    void setWidth(int newWidth) {
        width = newWidth;
    }
};

class snake {
private:
    int head_w, head_h, tail;
    vector<pair<int, int>> tail_chain;
    Dir dir;
public:
    snake(board &b) : head_w(b.getWidth() / 2), head_h(b.getHeight() / 2), tail(0), dir(LEFT) {
        tail_chain.resize(b.getHeight() * b.getWidth());
    }

    snake() : head_w(14), head_h(7), tail(0), dir(LEFT) {
        tail_chain.resize(14 * 28);
    }

    int getHeadW() const {
        return head_w;
    }

    void setHeadW(int newHeadW) {
        head_w = newHeadW;
    }

    int getHeadH() const {
        return head_h;
    }

    void setHeadH(int newHeadH) {
        head_h = newHeadH;
    }

    int getTail() const {
        return tail;
    }

    void setTail(int newTail) {
        tail = newTail;
    }

    Dir getDirection() const {
        return dir;
    }

    void setDirection(Dir newDir) {
        dir = newDir;
    }

    vector<pair<int, int>>& getTailChain() {
        return tail_chain;
    }

    void setTailChain(const vector<pair<int, int>>& newTailChain) {
        tail_chain = newTailChain;
    }

    void shift(vector<pair<int, int>> &arr) {
        int size = arr.size();
        for (int i = size - 2; i >= 0; i--) {
            arr[i + 1] = arr[i];
        }
    }
};

class LogicGame {
protected:
    board b;
    snake n;
    player p;
    int fruit_w, fruit_h, fruit_type = 0;
    int dif, increase;

    void clear_screen() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD cursorPosition;
        cursorPosition.X = 0;
        cursorPosition.Y = 0;
        SetConsoleCursorPosition(hConsole, cursorPosition);
    }

    void generate_f() {
        srand(static_cast<unsigned>(time(nullptr)));
        fruit_h = rand() % (b.getHeight() - 2) + 1;
        fruit_w = rand() % (b.getWidth() - 2) + 1;
        fruit_type++;
    }

    void setConsoleColors() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFOEX csbiex;
        csbiex.cbSize = sizeof(csbiex);
        GetConsoleScreenBufferInfoEx(hConsole, &csbiex);
        csbiex.wAttributes = FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
        csbiex.ColorTable[0] = RGB(255, 255, 255);
        csbiex.ColorTable[1] = RGB(255, 0, 0);
        SetConsoleScreenBufferInfoEx(hConsole, &csbiex);

        DWORD charsWritten;
        COORD coord = {0, 0};
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
        FillConsoleOutputAttribute(hConsole, csbiex.wAttributes, consoleSize, coord, &charsWritten);
        FillConsoleOutputCharacter(hConsole, ' ', consoleSize, coord, &charsWritten);
        SetConsoleCursorPosition(hConsole, coord);
    }

    void difficulty() {
        char c;
        cout << "1 : Easy  2 : Medium  3 : Hard\n";
        cin >> c;
        if (c == '1')
            dif = 100;
        else if (c == '2')
            dif = 50;
        else
            dif = 20;
    }

    void countof_increasing() {
        if (dif == 20)
            increase = 8;
        else if (dif == 50)
            increase = 5;
        else
            increase = 3;
    }

    bool game_over() { return p.hasLost(); }

public:
    LogicGame() : n(b) {
        difficulty();
        countof_increasing();
        fruit_type = 0;
        setConsoleColors();
        generate_f();
    }

    virtual void draw() {
        clear_screen();
        for (int i = 0; i < b.getHeight(); i++) {
            for (int j = 0; j < b.getWidth(); j++) {
                int t=0;
                if(p.hasLost()&&i==(b.getHeight()/2)&&t==0&&j==(b.getWidth()/2)-5)
                {
                   Sleep(500);
                    string c="Game Over";
                   for(int r:c)
                   {
                    cout<<char(r);
                   Sleep(500);
                   }
                    t++;
                                }

                else if (i == 0 || i == b.getHeight() - 1) cout << "-";
                else if (j == 0 || j == b.getWidth() - 1) cout << "|";
                else if (i == n.getHeadH() && j == n.getHeadW()) cout << "O";
                else if (i == fruit_h && j == fruit_w && fruit_type > 4) {
                    cout << "Q";
                }
                else if (i == fruit_h && j == fruit_w) {
                    cout << "@";
                              }
                                else {
                    bool g = false;
                    for (int z = 0; z < n.getTail(); z++) {
                        if (n.getTailChain()[z].first == i && n.getTailChain()[z].second == j) {
                            cout << "o";
                            g = true;
                            break;
                        }
                    }
                    if (!g) cout << " ";
                }
            }
            cout << endl;
        }
        cout << "Score: " << p.getScore();
    }

    void input() {
        if (_kbhit()) {
            int c = _getch();
            if (c == 224) {                 c = _getch();
                switch (c) {
                    case 72: if (n.getDirection() != DOWN) n.setDirection(UP); break;    // Up arrow
                    case 80: if (n.getDirection() != UP) n.setDirection(DOWN); break;    // Down arrow
                    case 77: if (n.getDirection() != LEFT) n.setDirection(RIGHT); break; // Right arrow
                    case 75: if (n.getDirection() != RIGHT) n.setDirection(LEFT); break; // Left arrow
                    default: break;
                }
            } else if (c == 'x') {
                exit(0);
            }
        }
    }

    virtual void move() = 0;

    virtual void run() {
        while (!game_over()) {
            draw();
            input();
            move();
            Sleep(dif);
        }
         draw();
    }
};

class GameWithWall : public LogicGame {
public:
    void move() override {
        n.shift(n.getTailChain());
        n.getTailChain()[0] = {n.getHeadH(), n.getHeadW()};

        switch (n.getDirection()) {
            case UP: n.setHeadH(n.getHeadH() - 1); break;
            case DOWN: n.setHeadH(n.getHeadH() + 1); break;
            case RIGHT: n.setHeadW(n.getHeadW() + 1); break;
            case LEFT: n.setHeadW(n.getHeadW() - 1); break;
            case STOP: return;
            default: break;
        }

        if (n.getHeadH() >= b.getHeight() || n.getHeadH() <= 0 || n.getHeadW() >= b.getWidth() || n.getHeadW() <= 0) {
            p.setLose(true);
            Beep(1000, 500);
        }

        for (int z = 0; z < n.getTail(); z++) {
            if (n.getHeadH() == n.getTailChain()[z].first && n.getHeadW() == n.getTailChain()[z].second) {
                p.setLose(true);
                Beep(1000, 500);
            }
        }

        if (n.getHeadH() == fruit_h && n.getHeadW() == fruit_w) {
            if (fruit_type > 4) {
                fruit_type = 0;
                p.setScore(p.getScore() + increase * 5);
                Beep(750, 200);
            } else {
                p.setScore(p.getScore() + increase);
                Beep(500, 200);
            }
            generate_f();

            n.setTail(n.getTail() + 1);
            n.getTailChain().push_back({-1, -1});
        }
    }
};

class GameWithoutWall : public LogicGame {
public:
    void move() override {
        n.shift(n.getTailChain());
        n.getTailChain()[0] = {n.getHeadH(), n.getHeadW()};

        switch (n.getDirection()) {
            case UP: n.setHeadH(n.getHeadH() - 1); break;
            case DOWN: n.setHeadH(n.getHeadH() + 1); break;
            case RIGHT: n.setHeadW(n.getHeadW() + 1); break;
            case LEFT: n.setHeadW(n.getHeadW() - 1); break;
            case STOP: return;
            default: break;
        }

        if (n.getHeadH() >= b.getHeight())
            n.setHeadH(0);
        else if (n.getHeadH() < 0)
            n.setHeadH(b.getHeight() - 1);
        if (n.getHeadW() >= b.getWidth())
            n.setHeadW(0);
        else if (n.getHeadW() < 0)
            n.setHeadW(b.getWidth() - 1);

        for (int z = 0; z < n.getTail(); z++) {
            if (n.getHeadH() == n.getTailChain()[z].first && n.getHeadW() == n.getTailChain()[z].second) {
                p.setLose(true);
                Beep(1000, 500);
            }
        }

        if (n.getHeadH() == fruit_h && n.getHeadW() == fruit_w) {
            if (fruit_type > 4) {
                fruit_type = 0;
                p.setScore(p.getScore() + increase * 5);
                Beep(750, 200);
            } else {
                p.setScore(p.getScore() + increase);
                Beep(500, 200);
            }
            generate_f();

            n.setTail(n.getTail() + 1);
            n.getTailChain().push_back({-1, -1});
        }
    }
};

void Snake_game(LogicGame* game) {
    game->run();
}

int main() {
    char choice;
    cout << "Select game mode:\n";
    cout << "1. Game With Wall\n";
    cout << "2. Game Without Wall\n";
    cin >> choice;

    LogicGame* game;
    if (choice == '1') {
        game = new GameWithWall();
    } else if (choice == '2') {
        game = new GameWithoutWall();
    } else {
        cout << "Invalid choice!\n";
        return 1;
    }

    Snake_game(game);

    delete game;
    return 0;
}
