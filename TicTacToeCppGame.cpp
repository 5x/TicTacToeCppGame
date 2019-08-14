#include "CMakeCPPDemo.h"

using namespace std;


HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursor_pos;
char player[2][100] = { "Player1", "Player2" };

void start_game(int, int, int);

void view(int** mas, const int NM) {
    cout << "\xDA\xC4\xC4";

    for (int i = 0; i < NM - 1; i++) {
        cout << "\xC4\xC2\xC4\xC4";
    }

    cout << "\xC4\xBF\n";

    for (int i = 0; i < NM; i++) {
        for (int j = 0; j < NM; j++) {
            cout << "\xB3 " << (mas[i][j] == 0 ? ' ' : mas[i][j] == 1 ? 'O' : 'X') << ' ';
        }

        if (i < NM - 1) {
            cout << "\xB3\n\xC3\xC4\xC4";

            for (int k = 0; k < NM - 1; k++) {
                cout << "\xC4\xC5\xC4\xC4";
            }

            cout << "\xC4\xB4\n";
        }
    }

    cout << "\xB3\n\xC0\xC4\xC4";

    for (int i = 0; i < NM - 1; i++) {
        cout << "\xC4\xC1\xC4\xC4";
    }

    cout << "\xC4\xD9\n";
}

void end(const int n, const int NM, int** mas, const int type) {
    char t[100];

    system("cls");
    view(mas, NM);

    cursor_pos.X = 0;
    cursor_pos.Y = short(NM * 2.5);

    SetConsoleCursorPosition(console_output, cursor_pos);

    cout << (n > 0 ? "Win: " : "Draw") << (n > 0 ? player[n - 1] : " ");
    cout << endl << "Try again?(y/n): ";

    cin.getline(t, 100);
    if (t[0] == 'y' || t[0] == 'Y' || t[0] == '1') {
        start_game(NM, type, 1);
    } else {
        exit(0);
    }
}

int check_winner(int** mas, const int NM, const int type) {
    int rs[4] = { 1, 1, 1, 1 };
    int winner[4] = { 0, 0, 0, 0 };
    bool free = false;

    for (int i = 0; i < NM || rs[0] == NM || rs[1] == NM || rs[2] == NM || rs[3] == NM; i++) {
        for (int j = 1; j < NM; j++) {
            if (mas[i][j - 1] == 0 || mas[i][j] == 0) {
                free = true;
            } else {
                if (mas[i][j] == mas[i][j - 1] && mas[i][j] != 0) {
                    rs[0]++;
                    winner[0] = mas[i][i];
                }
            }

            if (i == 0 && mas[j][j] == mas[j - 1][j - 1] && mas[j][j] != 0) {
                rs[1]++;
                winner[1] = mas[j][j];
            }

            if (mas[j][i] == mas[j - 1][i] && mas[j][i] != 0) {
                rs[2]++;
                winner[2] = mas[j][i];
            }
        }

        if (i == 0 && rs[1] == NM) {
            end(winner[1], NM, mas, type);
        }

        if (rs[0] == NM) {
            end(winner[0], NM, mas, type);
        } else {
            rs[0] = 1;
        }

        if (rs[2] == NM) {
            end(winner[2], NM, mas, type);
        } else {
            rs[2] = 1;
        }
    }

    for (int i = NM - 1, j = 0; i > 0; i--, j++) {
        if (mas[i][j] == mas[i - 1][j + 1] && mas[i][j] != 0) {
            rs[3]++;
            winner[3] = mas[i][j];
        }
    }

    if (rs[3] == NM) {
        end(winner[3], NM, mas, type);
    }

    if (!free) {
        end(0, NM, mas, type);
    }

    return 0;
}

int move(int* n) {
    char symbol = NULL;

    cursor_pos.X = 2;
    cursor_pos.Y = 1;

    SetConsoleCursorPosition(console_output, cursor_pos);

    while (symbol != ' ') {
        symbol = _getch();

        switch (symbol) {
        case 'w': cursor_pos.Y -= 2; break;
        case 'a': cursor_pos.X -= 4; break;
        case 's': cursor_pos.Y += 2; break;
        case 'd': cursor_pos.X += 4; break;
        default:break;
        }

        SetConsoleCursorPosition(console_output, cursor_pos);
    }

    if (cursor_pos.Y / 2 < 0 || cursor_pos.X / 4 < 0) {
        move(n);
    }

    n[0] = cursor_pos.Y / 2;
    n[1] = cursor_pos.X / 4;

    return *n;
}

void game_tick(int** mas, const int NM, const int st, const int type) {
    bool nl = true;
    int n[2];

    system("cls");
    view(mas, NM);

    while (nl) {
        if (type == 1 && st == 2) {
            n[0] = rand() % NM;
            n[1] = rand() % NM;
        } else {
            move(n);

            if (n[0] > NM || n[1] > NM) {
                return game_tick(mas, NM, st, type);
            }
        }

        if (mas[n[0]][n[1]] == 0) {
            mas[n[0]][n[1]] = st;
            nl = false;
        }
    }

    if (check_winner(mas, NM, type) == 0) {
        const int next_st = (st == 1) ? 2 : 1;
        game_tick(mas, NM, next_st, type);
    }
}

void start_game(const int NM, const int type, const int st) {
    int** mas = new int* [NM];
    
    for (int i = 0; i < NM; i++) {
        mas[i] = new int[NM];

        for (int j = 0; j < NM; j++) {
            mas[i][j] = 0;
        }
    }

    game_tick(mas, NM, st, type);
}

int user_handle_int(const char* out, const int min, const int max) {
    const unsigned int buffer_size = 100;
    char buffer[buffer_size];

    cout << out;
    cin.getline(buffer, buffer_size);

    if (!isdigit(*buffer)) {
        return user_handle_int(out, min, max);
    }

    const int value = atoi(buffer);

    if (value >= min && value <= max) {
        return value;
    }

    return user_handle_int(out, min, max);
}

int main() {
    srand((unsigned)time(NULL));

    const int NM = user_handle_int("Type size of board: ", 3, 16);

    cout << "Select type of game_tick:" << endl;
    cout << "1 - One player - game_tick with computer" << endl
        << "2 - Two player - Multi player" << endl;

    const int type = user_handle_int("Enter number of type: ", 1, 2);
    const int st = user_handle_int("Player # going first(1/2): ", 1, 2);

    cout << "Use WASD to move cursor. Enter <SPACE> to apply cell." << endl
        << "Press Enter if you ready to go!" << endl;

    _getch();
    start_game(NM, type, st);
    _getch();

    return 0;
}
