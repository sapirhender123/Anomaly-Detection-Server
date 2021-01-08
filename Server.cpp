// 208414573 Sapir Hender
#include "Server.h"
#include <ostream>
#include <sys/socket.h>
#include <thread>

Server::Server(int port) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        throw std::runtime_error("socket failed");
    }
    // initialize struct
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // Check if there is error in bind or listen
    if (bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        throw std::runtime_error("bind failure");
    }
    if (listen(fd, 1) < 0) {
        throw std::runtime_error("listen failure");
    }
}

void Server::start(ClientHandler& ch) {
    // Creat new thread
    t = new thread([&ch, this](){
        // TODO: make server:stop set flag for this to exit
        struct timeval tv{};
        while (!stopServer /* running */) {
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(fd, &rfds);

            tv.tv_sec = 5;
            tv.tv_usec = 0;

//            cout << "server: waiting for a client" << endl;
            sockaddr_in client{};
            socklen_t clientSize = sizeof(client);

            int aClient;
            // Wait until the socket will be ready for reading, and stop it if it's necessary
            int res = select(fd + 1, &rfds, nullptr, nullptr, &tv);
            if (-1 == res) {
                throw std::runtime_error("select failure");
            } else if (FD_ISSET(fd, &rfds)) {
                aClient = accept(fd, (struct sockaddr *) &client, &clientSize);
                if (aClient < 0) { throw std::runtime_error("accept failure"); }
            } else {
//                cout << "server: select timeout" << endl;
                continue;
            }

//            cout << "server: client connected" << endl;
            // TODO: Handle in different thread - add thread to list of clients
            ch.handle(aClient);
//            cout << "server: client disconnected" << endl;
        }
    });
}

void Server::stop(){
    stopServer = true;
    t->join(); // do not delete this!
}

Server::~Server() {
    // If we haven't stopped the server, do it now
    if (!stopServer) {
        stop();
    }
}
