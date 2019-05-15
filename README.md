# Simple test application that demonstrates boost based async chat server.

Server starts, listens to incoming connections, reads incoming requests and sends "Accepted" string as response.
Currently it works as ping/pong only

# tools:
- c++14
- boost asio 1-58
- boost system 1-58
- qt 5.9.12
- gcc 5.4.0
- ubuntu 18

# how to run

- cd src_dir
- qmake .
- make
- ./server
- cd client
- qmake
- make
- ./client username

