#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class network : public QObject
{
    Q_OBJECT
public:
    explicit network(QString ipAddr = 0, QObject *parent = 0);
    ~network();

signals:
    void connected(int player);
    void sent(int value);
    void received(int value);

public slots:
    void sendValue(int value);

private slots:
    void newClient();
    void clientConnected();
    void clientDisconnected();
    void readIncomingData();

private:
    QTcpServer *server;
    QTcpSocket *client;

};

#endif // NETWORK_H
