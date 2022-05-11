#ifndef SOCKET_HELPERS_23874924
#define SOCKET_HELPERS_23874924

#include <iostream>
#include <string.h>
#include <thread>

#include "pong.h"

#ifdef _WIN32
    #define MY_OS "Windows"
    #include <WinSock2.h>
    typedef SOCKET SOCKET_TYPE;
    typedef int ADDRESS_SIZE;
    #define ON_WINDOWS
#endif

#define TCP_PORT 4037


typedef struct DataClass
{
    int paddle_y;
    int ball_x;
    int ball_y;
    int ball_ang;
    int p1_points;
    int p2_points;
} DataClass;

using namespace std;


bool check_and_start_if_windows();

void make_socket_address(sockaddr *address, int port, std::string ip_address = "");
bool bind_socket_to_address(SOCKET_TYPE &my_socket, sockaddr *p_address);
bool listen_at_socket(SOCKET_TYPE &my_socket);
bool accept_connection(SOCKET_TYPE &new_socket, SOCKET_TYPE &my_socket, sockaddr &socket_address);
bool connect_to_server(SOCKET_TYPE &my_socket, sockaddr &socket_address);
void receive_from_client(SOCKET_TYPE recv_socket, bool is_host, Pong *pong);
void serialize(DataClass* dataPacket, char *data);
void deserialize(char *data, DataClass* dataPacket);

#endif //SOCKET_HELPERS_23874924