This project is a multithreaded multiplayer networked pong.

This only works on Windows due to how the game is drawn.
Multithreaded compiler for pthreads is needed to compile this program.

To compile use either the makefile or this command:
g++ -std=c++11 .\main.cpp .\socket_helpers.cpp .\pong.cpp -o pong.exe -lws2_32

To run:
The game runs automatically on port 4037 using TCP, so that port might need to be port forwarded for the server if using separate networks.
Start by running the server with the following flag:
./pong.exe -s
Connect to the server using the following command:
./pong.exe <ip_address>
Controls are 'W' and 'S' for up and down, respectively and 'ESC' to quit the game.