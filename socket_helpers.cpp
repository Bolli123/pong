#include "socket_helpers.h"

using namespace std;

bool check_and_start_if_windows(){
    #ifdef ON_WINDOWS
        WSADATA wsa_data;
        if(WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0){
            cout << "failed at WSAStartup()" << endl;
            return false;
        }
    #endif
    return true;
}

void receive_from_client(SOCKET_TYPE recv_socket, bool is_host, Pong *pong){
    //cout << "NEW CONNECTION" << endl;    
    char buffer[1024];
    int pos;
    while(true){
        int act_len = recv(recv_socket, buffer, 1024, 0);
        if (act_len <= 0){
            cout << "DISCONNECT" << endl;
            pong->end_game();
        } 
        buffer[act_len] = '\0';
        DataClass* tmp = new DataClass;
        deserialize(buffer, tmp);
        pos = tmp->paddle_y;
        if (is_host) {
            pong->paddle2.set_y_pos(pos);
        }
        else {
            pong->paddle1.set_y_pos(pos);
            if (pong->ball_ang != tmp->ball_ang){
                pong->ball_ang = tmp->ball_ang;
                pong->ball_x = tmp->ball_x;
                pong->ball_y = tmp->ball_y;
                pong->ball_frame = 1;
                pong->paddle1.score = tmp->p1_points;
                pong->paddle2.score = tmp->p2_points;
            }
        }
        memset(buffer, 0, sizeof(buffer));
        delete(tmp);
    }
}

void serialize(DataClass* dataPacket, char *data)
{
    int *q = (int*)data;    
    *q = dataPacket->paddle_y;   q++;    
    *q = dataPacket->ball_ang;   q++;    
    *q = dataPacket->ball_x;     q++;
    *q = dataPacket->ball_y;     q++;
    *q = dataPacket->p1_points;     q++;
    *q = dataPacket->p2_points;     q++;
}

void deserialize(char *data, DataClass* dataPacket)
{
    int *q = (int*)data;    
    dataPacket->paddle_y = *q;   q++;    
    dataPacket->ball_ang = *q;   q++;    
    dataPacket->ball_x = *q;     q++;
    dataPacket->ball_y = *q;     q++;
    dataPacket->p1_points = *q;     q++;
    dataPacket->p2_points = *q;     q++;
}


void make_socket_address(sockaddr *address, int port, std::string ip_address){
    sockaddr_in *socket_address = (sockaddr_in*)address;
    socket_address->sin_family = AF_INET;
    socket_address->sin_port = htons(port);
    if(ip_address == ""){
        socket_address->sin_addr.s_addr = INADDR_ANY;
    }
    else{
        socket_address->sin_addr.s_addr = inet_addr(ip_address.c_str());
    }
}

bool bind_socket_to_address(SOCKET_TYPE &my_socket, sockaddr *p_address){

    if(bind(my_socket, p_address, sizeof(*p_address)) < 0){
        cout << "bind() failed" << endl;
        return false;
    }
    return true;
}

bool listen_at_socket(SOCKET_TYPE &my_socket){

    if(listen(my_socket, 1) < 0){
        cout << "listen() failed: " << endl;
        return false;
    }
    return true;
}

bool accept_connection(SOCKET_TYPE &new_socket, SOCKET_TYPE &my_socket, sockaddr &socket_address){
    ADDRESS_SIZE addr_size = sizeof(socket_address);

    new_socket = accept(my_socket, (sockaddr *)&socket_address, &addr_size);
    if(new_socket == -1 || new_socket == ~0){
        cout << "invalid socket" << endl;
        return false;
    }
    return true;
}

bool connect_to_server(SOCKET_TYPE &my_socket, sockaddr &socket_address){
        if(connect(my_socket, (sockaddr *)&socket_address, sizeof(socket_address)) < 0){
            cout << "connect() failed: " << endl;
            return false;
        }
        return true;
}
