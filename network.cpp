#include "network.h"

network::network(QString ipAddr, QObject *parent) :
    QObject(parent)
{
    if(ipAddr.isEmpty()) {
        // we dont know where to connect => lets be a server
        this->server = new QTcpServer();
        this->client = NULL;

        connect(server, SIGNAL(newConnection()), this, SLOT(newClient()));

        this->server->listen(QHostAddress::Any, 1337);
    }
    else {
        // lets be a client
        this->server = NULL;
        this->client = new QTcpSocket();

        connect(client, SIGNAL(connected()), this, SLOT(clientConnected()));
        connect(client, SIGNAL(readyRead()), this, SLOT(readIncomingData()));

        this->client->connectToHost(QHostAddress(ipAddr), 1337);
    }
}

network::~network()
{
    if(server)
        this->server->close();
    if(client)
        this->client->close();
}

// server slot
void network::newClient()
{
    this->client = server->nextPendingConnection();

    connect(client, SIGNAL(readyRead()), this, SLOT(readIncomingData()));
    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));

    // since we have a connection now, we are in a network game. emit player 1 role
    emit connected(1);
}

//client slot
void network::clientConnected()
{
    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));

    // since we have a connection to a server, we are in a network game. emit player 2 role
    emit connected(2);
}

void network::clientDisconnected()
{
    // client disconnected, emit singleplayer game!
    emit connected(0);

    this->client->close();
    this->client = NULL;
}

void network::sendValue(int value)
{
    this->client->write(QByteArray::number(value));

    emit sent(value);
}

void network::readIncomingData()
{
    QByteArray value(client->readAll());

    emit received(value.toInt());
}
