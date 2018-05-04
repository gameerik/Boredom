#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <thread>
//from net ---------------------------------------------
int getch()
{
    struct termios oldt,
        newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
//from net ----------------------------------------------
class Enemy
{
  public:
    Enemy(std::string name, int x, int y) : enemy_name(name), enemy_x(x), enemy_y(y) {}
    Enemy(const Enemy &) = default;
    Enemy &operator=(const Enemy &) = default;
    int get_enemy_x() { return enemy_x; }
    int get_enemy_y() { return enemy_y; }
    void inc_enemy_x() { enemy_x++; }
    void inc_enemy_y() { enemy_y++; }
    void dec_enemy_x() { enemy_x--; }
    void dec_enemy_y() { enemy_y--; }

  private:
    int enemy_x, enemy_y;
    std::string enemy_name;
};

class Game
{
  public:
    Game() {}
    void LoadLevel(int size = 20, int size2 = 30)
    {
        _sizex = size;
        _sizey = size2;
        current_level = new char *[_sizex];
        for (int i{0}; i < _sizex; i++)
        {
            current_level[i] = new char[_sizey];
            for (int j{0}; j < _sizey; j++)
                if (i == 0 || i == _sizex - 1 || j == 0 || j == _sizey - 1)
                    current_level[i][j] = '#';
                else
                    current_level[i][j] = ' ';
        }
    }
    void Display_Level()
    {
        for (int i{0}; i < _sizex; i++)
        {
            for (int j{0}; j < _sizey; j++)
                std::cout << current_level[i][j];
            std::cout << std::endl;
        }
    }
    void Spawn_Player()
    {
        current_level[1][1] = '@';
        player_x = 1;
        player_y = 1;
    }
    void Spawn_Enemy(int number_of_enemies = 1)
    {
        int ok = 0;
        enemy_count = number_of_enemies;

        while (ok < enemy_count)
        {
            srand((unsigned)time(0));
            for (int i = 0; i < enemy_count; i++)
                if (((i = rand() % (_sizex - 1)) - player_x >= 3) && (i - player_y >= 3) && current_level[i][i] == ' ')
                {
                    current_level[i][i] = '&';
                    enemies.push_back(Enemy(("Enemy" + (char)ok), i, i));
                    ok++;
                }
        }
    }

    void Enemy_Movement()
    {
        srand((unsigned)time(0));
        for (auto &i : enemies)
        {
            switch (int chance = (rand() % 8) + 1)
            {
            case 1:
                if (current_level[i.get_enemy_x() + 1][i.get_enemy_y()] != '#')
                {
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = ' ';
                    i.inc_enemy_x();
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = '&';
                }
                break;
            case 2:
                if (current_level[i.get_enemy_x() - 1][i.get_enemy_y()] != '#')
                {
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = ' ';
                    i.dec_enemy_x();
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = '&';
                }
                break;
            case 3:
                if (current_level[i.get_enemy_x()][i.get_enemy_y() + 1] != '#')
                {
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = ' ';
                    i.inc_enemy_y();
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = '&';
                }
                break;
            case 4:
                if (current_level[i.get_enemy_x()][i.get_enemy_y() - 1] != '#')
                {
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = ' ';
                    i.dec_enemy_y();
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = '&';
                }
                break;

            case 5:
                if (current_level[i.get_enemy_x() - 1][i.get_enemy_y() - 1] != '#')
                {
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = ' ';
                    i.dec_enemy_y();
                    i.dec_enemy_x();
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = '&';
                }
                break;
            case 6:
                if (current_level[i.get_enemy_x() + 1][i.get_enemy_y() + 1] != '#')
                {
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = ' ';
                    i.inc_enemy_y();
                    i.inc_enemy_x();
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = '&';
                }
                break;
            case 7:
                if (current_level[i.get_enemy_x() - 1][i.get_enemy_y() + 1] != '#')
                {
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = ' ';
                    i.inc_enemy_y();
                    i.dec_enemy_x();
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = '&';
                }
                break;
            case 8:
                if (current_level[i.get_enemy_x() + 1][i.get_enemy_y() - 1] != '#')
                {
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = ' ';
                    i.dec_enemy_y();
                    i.inc_enemy_x();
                    current_level[i.get_enemy_x()][i.get_enemy_y()] = '&';
                }
                break;
            };
        }
    }

    void Enemy_Update()
    {
        while (Player_status())
        {
            Enemy_Movement();
            system("clear");
            Display_Level();
            sleep(1);
        }
        _state = true;
    }
    bool Player_status()
    {
        for (auto &i : enemies)
            if ((player_x == i.get_enemy_x()) && (player_y == i.get_enemy_y()))
            {
                current_level[player_x][player_y] = ' ';
                return false;
            }
        return true;
    }
    void Multi_Thread()
    {
        std::thread t1(&Game::Enemy_Update, this);
        std::thread t2(&Game::Player_Movement, this);
        t1.join();
        t2.join();
    }
    void Player_Movement()
    {

        while (Player_status())
        {
            char input = getch();

            switch (input)
            {
            case 'w':
                if (current_level[player_x - 1][player_y] != '#')
                {
                    current_level[player_x--][player_y] = ' ';
                    current_level[player_x][player_y] = '@';
                }
                system("clear");
                Display_Level();
                break;
            case 'W':
                if (current_level[player_x - 1][player_y] != '#')
                {
                    current_level[player_x--][player_y] = ' ';
                    current_level[player_x][player_y] = '@';
                }
                system("clear");
                Display_Level();
                break;
            case 's':
                if (current_level[player_x + 1][player_y] != '#')
                {
                    current_level[player_x++][player_y] = ' ';
                    current_level[player_x][player_y] = '@';
                }
                system("clear");
                Display_Level();

                break;
            case 'S':
                if (current_level[player_x + 1][player_y] != '#')
                {
                    current_level[player_x++][player_y] = ' ';
                    current_level[player_x][player_y] = '@';
                }
                system("clear");
                Display_Level();

                break;
            case 'a':
                if (current_level[player_x][player_y - 1] != '#')
                {
                    current_level[player_x][player_y--] = ' ';
                    current_level[player_x][player_y] = '@';
                }
                system("clear");
                Display_Level();

                break;
            case 'A':
                if (current_level[player_x][player_y - 1] != '#')
                {
                    current_level[player_x][player_y--] = ' ';
                    current_level[player_x][player_y] = '@';
                }
                system("clear");
                Display_Level();

                break;
            case 'd':
                if (current_level[player_x][player_y + 1] != '#')
                {
                    current_level[player_x][player_y++] = ' ';
                    current_level[player_x][player_y] = '@';
                }
                system("clear");
                Display_Level();

                break;
            case 'D':
                if (current_level[player_x][player_y + 1] != '#')
                {
                    current_level[player_x][player_y++] = ' ';
                    current_level[player_x][player_y] = '@';
                }
                system("clear");
                Display_Level();

                break;
            }
        }
        _state = true;
    }
    ~Game()
    {
        system("clear");
        std::cout << "\nGame Over\n";
        for (int i{0}; i < _sizex; i++)
            delete current_level[i];
        delete current_level;
    }

  private:
    char **current_level;
    std::vector<Enemy> enemies;
    int _sizex, _sizey, player_x, player_y, enemy_count;
    bool _state = false;
};

void default_game()
{
    Game obj;
    obj.LoadLevel();
    obj.Spawn_Player();
    obj.Spawn_Enemy();
    obj.Display_Level();
    obj.Multi_Thread();
}

void custom_game(int map_x, int map_y, int enemies_count)
{
    Game obj;
    obj.LoadLevel(map_x, map_y);
    obj.Spawn_Player();
    obj.Spawn_Enemy(enemies_count);
    obj.Display_Level();
    obj.Multi_Thread();
}
void show_rules()
{
    std::cout << "You are controllong the ' @ ' symbol , your main objectives is to survive as long as possible , you do that by avoiding the ' & ' symbol\n";
    std::cout << "Controls are : W(up), S(down),A(left),D(right)\n";
}
void initialize()
{
    char input;
    std::cout << "Would you like to read the rules/controls ?(Y/N)\n";
    std::cin >> input;
    if (input == 'Y')
        show_rules();
    else if (input != 'N')
        std::cout << "Wrong input (the input is case sensitive)\nPlease restart the program ./WhatYouUsed\n";
    std::cout << "Would you like to use the default settings ?(Y/N)\n";
    std::cin >> input;
    if (input == 'Y')
    {
        std::cout << "Loading...\n";
        default_game();
    }
    else if (input == 'N')
    {
        int height, width, nof_enemies;
        std::cout << "Specify map height\n";
        std::cin >> height;
        std::cout << "Specify map width\n";
        std::cin >> width;
        std::cout << "Number of enemies\n";
        std::cin >> nof_enemies;
        std::cout << "Loading...\n";
        custom_game(height, width, nof_enemies);
    }
    else
        std::cout << "Wrong input (the input is case sensitive)\nPlease restart the program ./WhatYouUsed\n";
}

int main()
{
    initialize();
    std::cout << "Score : ...\n Well you lost, doesn't matter\n";
}