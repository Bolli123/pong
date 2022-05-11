#include "socket_helpers.h"
#include "pong.h"

#define PACKETSIZE sizeof(DataClass)
#define W_HEIGHT 31
#define W_WIDTH 99
#define TICKRATE 20
#define FRAMERATE 60


typedef chrono::high_resolution_clock::duration duration_t;

using namespace std;

int main(int argc, char *argv[]){
    
    if(!check_and_start_if_windows()){ return 0; }
    SOCKET_TYPE send_socket;
    srand(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

    //Scale Console to the size of the game
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect); 
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, W_WIDTH * 10, W_HEIGHT * 20, TRUE);

    
    float tickrate_freq = (1.0/TICKRATE) * 1000;
    duration_t tickrate_time_dif;
    int tickrate_millisec;
    
    
    float frame_freq = (1.0/FRAMERATE) * 1000;
    duration_t frame_time_dif;
    int frame_millisec;
    int nr_skipped_frame;
    
    bool is_host = true;
    
    system("cls");

    cout << "Waiting for connection..." << endl;

    if (argc > 1){ // SERVER
        if(strcmp(argv[1], "-s") == 0){
            SOCKET_TYPE the_socket = socket(AF_INET, SOCK_STREAM, 0);
            sockaddr socket_address;
            make_socket_address(&socket_address, TCP_PORT);
            bind(the_socket, &socket_address, sizeof(socket_address));
            listen(the_socket, 1);
            ADDRESS_SIZE addr_size = sizeof(socket_address);
            send_socket = accept(the_socket, &socket_address, &addr_size);

        }
        else{ // CLIENT
            is_host = false;
            cout << argv[1] << endl;
            send_socket = socket(AF_INET, SOCK_STREAM, 0);
            sockaddr socket_address;
            make_socket_address(&socket_address, TCP_PORT, argv[1]);
            connect_to_server(send_socket, socket_address);
        }

        Pong *pong = new Pong();
        pong->is_host = is_host;
        pong->init_game(45);
        thread *recv_thread = new thread(receive_from_client, send_socket, is_host, pong);
        // Beep(523,500);
        
        system("cls");
        //Hide cursor
        CONSOLE_CURSOR_INFO lpCursor;
        lpCursor.bVisible = false;
        lpCursor.dwSize = 1;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), & lpCursor);
        auto last_tickrate_time = chrono::high_resolution_clock::now();
        auto last_frame_time = chrono::high_resolution_clock::now();
        
        while(true){
            // GAME
            frame_time_dif = (chrono::high_resolution_clock::now() - last_frame_time);
            frame_millisec = chrono::duration_cast<chrono::milliseconds>(frame_time_dif).count();
            
            if (frame_millisec > frame_freq) {
                // update ball position accounted for frame drops
                nr_skipped_frame = frame_millisec / frame_freq - 1;
                for (int i = 0; i < nr_skipped_frame; i++) {
                    pong->ball_frame++;
                    if (GetKeyState('W') & 0x8000) {
                        if (is_host) {
                            pong->paddle1.up();
                        }
                        else {
                            pong->paddle2.up();
                        }
                    }
                    //S
                    else if (GetKeyState('S') & 0x8000) {
                        if (is_host) {
                            pong->paddle1.down();
                        }
                        else {
                            pong->paddle2.down();
                        }
                    }
                }

                //Check inputs

                //ESC
                if (GetKeyState(27) & 0x8000) {
                    pong->end_game();
                }
                //W
                else if (GetKeyState('W') & 0x8000) {
                    if (is_host) {
                        pong->paddle1.up();
                    }
                    else {
                        pong->paddle2.up();
                    }
                }
                //S
                else if (GetKeyState('S') & 0x8000) {
                    if (is_host) {
                        pong->paddle1.down();
                    }
                    else {
                        pong->paddle2.down();
                    }
                }

                pong->draw_game();
                last_frame_time = chrono::high_resolution_clock::now();
            }

            // NETWORKING
            tickrate_time_dif = (chrono::high_resolution_clock::now() - last_tickrate_time);
            tickrate_millisec = chrono::duration_cast<chrono::milliseconds>(tickrate_time_dif).count();

            if (tickrate_millisec > tickrate_freq) {
                DataClass* data = new DataClass();
                //Create packet to send
                char packet[PACKETSIZE];
                if (is_host) {
                    data->paddle_y = pong->paddle1.y;
                    data->ball_ang = pong->ball_ang;
                    data->ball_x = pong->ball_x;
                    data->ball_y = pong->ball_y;
                    data->p1_points = pong->paddle1.score;
                    data->p2_points = pong->paddle2.score;
                }
                else {
                    data->paddle_y = pong->paddle2.y;
                }
                serialize(data, packet);
                send(send_socket, packet, sizeof(packet), 0);
                last_tickrate_time = chrono::high_resolution_clock::now();
                delete(data);
            }
        }
        delete recv_thread;
    }
    return 0;
}