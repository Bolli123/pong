#include "pong.h"

using namespace std;

Paddle::Paddle() {
    this->x = 10;
    this->y = 10;
}
Paddle::Paddle(int x, int y) {
    this->x = x;
    this->y = y;
}

void Paddle::up() {
    if (y < 2) { return; }
    y -= 1;
}

void Paddle::down() {
    if (y + P_HEIGHT < W_HEIGHT - 1)
    y += 1;
} 
void Paddle::add_score(){
    score += 1;
}
void Paddle::set_y_pos(int y) {
    this->y = y;
}   


Pong::Pong() {}

void Pong::gotoxy(int column, int row) {
    COORD coord;
    coord.X = column;
    coord.Y = row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Pong::draw_skeleton() {
    for (int i = 1; i < W_HEIGHT-1; i++) {
        gotoxy(0, i);
        cout << (char)186u;
        gotoxy(W_WIDTH-1, i);
        cout << (char)186u;
    }

    for (int i = 1; i < W_HEIGHT-1; i++) {
        gotoxy(W_WIDTH/2, i);
        cout << ":";
    }

    for (int i = 0; i < W_WIDTH; i++) {
        gotoxy(i, 0);
        cout << (char)205u;
        gotoxy(i, W_HEIGHT-1);
        cout << (char)205u;
    }

    // Score
    gotoxy(W_WIDTH/2 - 4, 1);
    cout << this->paddle1.score;

    gotoxy(W_WIDTH/2 + 4, 1);
    cout << this->paddle2.score;
}

void Pong::draw_game() {
    // draw ball
    draw_skeleton();
    get_new_ball_position();

    gotoxy(this->cur_ball_x, this->cur_ball_y);
    cout << (char)254u;

    this->clear_paddles();
    for (int i = this->paddle1.y; i < this->paddle1.y + P_HEIGHT; i++) {
        gotoxy(2, i);
        cout << '#';
    }

    for (int i = this->paddle2.y; i < this->paddle2.y + P_HEIGHT; i++) {
        gotoxy(W_WIDTH-3, i);
        cout << '#';
    }
}

int Pong::random_angle() {
    return 20 + rand() % 49 + 135;
}

void Pong::init_game(int added_angle = 0) {
    this->ball_ang = random_angle() - added_angle;
    this->ball_x = W_WIDTH / 2;
    this->ball_y = W_HEIGHT / 2;
    this->cur_ball_x = W_WIDTH / 2;
    this->cur_ball_y = W_HEIGHT / 2;
    this->ball_frame = 0;
    draw_skeleton();
}

void Pong::clear_paddles() {
    for (int i = 1; i < W_HEIGHT -1; i++) {
        gotoxy(2, i);
        cout << " ";
    }

    for (int i = 1; i < W_HEIGHT -1; i++) {
        gotoxy(W_WIDTH-3, i);
        cout << " ";
    }
}

void Pong::calc_deflect() {
    int new_angle;
    //cout << '\a';
    
    if (this->cur_ball_x <= 2){
        this->cur_ball_x = 3;
        new_angle = 180 - this->ball_ang + rand() % 30 -15;   
        if (new_angle > 70 && new_angle < 100) {
            new_angle = 70;
        }
        else if (new_angle < 290 && new_angle > 180) {
            new_angle = 290;
        }
    }
    else if (this->cur_ball_x > W_WIDTH -4){
        this->cur_ball_x = W_WIDTH -4;
        new_angle = 180 - this->ball_ang + rand() % 30 -15;
        if (new_angle > 250 && new_angle < 280) {
            new_angle = 250;
        }
        else if (new_angle < 110 && new_angle > 80) {
            new_angle = 110;
        }
        
    }
    else if (this->cur_ball_y <= 1){
        this->cur_ball_y = 1;
        new_angle = 360 - this->ball_ang; 
    }
    else if (this->cur_ball_y > W_HEIGHT -3) {
        this->cur_ball_y = W_HEIGHT -2;
        new_angle = 360 - this->ball_ang; 
    }

    this->ball_ang = new_angle; //abs(this->ball_ang - 180);
    this->ball_x = this->cur_ball_x;
    this->ball_y = this->cur_ball_y;
    this->ball_frame = 0;

}

void Pong::get_new_ball_position() {
    // clean old
    gotoxy(this->cur_ball_x, this->cur_ball_y);
    cout << ' ';
    this->cur_ball_x = (this->ball_frame * cos(this->ball_ang * PI/180)*BALLSPEED) + this->ball_x;
    this->cur_ball_y = (this->ball_frame * sin(this->ball_ang * PI/180)*BALLSPEED) + this->ball_y;

    if (this->cur_ball_x < 3) {
        if (this->paddle1.y <= this->cur_ball_y && this->cur_ball_y <= this->paddle1.y + P_HEIGHT){
            calc_deflect();
        }
        else{
            this->paddle2.add_score();
            init_game();
            
        }
        
    }
    else if (this->cur_ball_x > W_WIDTH -4) {
        if (this->paddle2.y - 1 <= this->cur_ball_y && this->cur_ball_y <= this->paddle2.y + P_HEIGHT + 1){ // latency forgivness
            calc_deflect();
        }
        else{
            init_game(180);
            this->paddle1.add_score();                   
        }
    }
    else if (this->cur_ball_y < 1 ||this->cur_ball_y > W_HEIGHT -2) {
        calc_deflect();
    } 
    this->ball_frame++;
}
void Pong::end_game() {
    system("cls");
    string winner;
    int p1_score = this->paddle1.score;
    int p2_score = this->paddle2.score;
    printf("---Player one score: %d---\n---Player two score: %d---\n", p1_score, p2_score);
    if (p1_score > p2_score) {
        winner = "Player one";
    }
    else if (p2_score > p1_score) {
        winner = "Player two";
    }
    if (p2_score == p1_score) {
        printf("It's a tie!!");
    }
    else {
        cout << "---" << winner << " wins!!!---" << endl;
    }
    exit(0);
}
