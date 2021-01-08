// 208414573 Sapir Hender

#ifndef SERVER_H_
#define SERVER_H_

#include <netinet/in.h>
#include <thread>

#include "CLI.h"
using namespace std;

class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class AnomalyDetectionHandler:public ClientHandler {
	public:
    void handle(int clientID) override {
        SocketIO sio = SocketIO(clientID);
        CLI clientCLI = CLI(&sio);
        clientCLI.start(); // "client loop"
    }
};

class Server {
	thread* t; // The thread to run the start() method in
    int fd; // File description
    bool stopServer = false;

public:
	explicit Server(int port);
	virtual ~Server();
	void start(ClientHandler& ch);
	void stop();
};

#endif /* SERVER_H_ */
