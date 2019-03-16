# Simple test application that demonstrates boost based async chat server.

Server starts, listens to incoming connections, reads incoming requests and sends "Accepted" string as response.
Currently it works as ping/pong only

# tools:
- c++14
- boost asio 1-58
- boost system 1-58
- cmake 3.5.1
- gcc 5.4.0
- ubuntu 16

# how to run

- cd src_dir
- cmake .
- make
- ./chat_server
- ./chat_client

