#ifndef PONG_H
#define PONG_H

#include <iostream>
#include <chrono>
#include <winsock2.h>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <math.h>
#include <string>

#define W_HEIGHT 31
#define W_WIDTH 99
#define P_HEIGHT 4
#define BALLSPEED 2
#define PI 3.14159265

class Paddle {
public:
    Paddle();
    Paddle(int x, int y);
    int x;
    int y;
    int score = 0;
    void up();

    void down();
    void add_score();
    void set_y_pos(int y);
};

class Pong {
    public:
        Pong();
        int ball_x = W_WIDTH / 2;
        int ball_y = W_HEIGHT / 2;
        int cur_ball_x;
        int cur_ball_y;
        int ball_ang;
        int ball_frame;
        Paddle paddle1;
        Paddle paddle2;
        bool is_host = false;
        void gotoxy(int column, int row);

        void draw_skeleton();

        void draw_game();

        int random_angle();

        void init_game(int added_angle);

        void clear_paddles();

        void calc_deflect();
        
        void get_new_ball_position();

        void end_game();
};

#endif