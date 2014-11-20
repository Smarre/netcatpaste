#ifndef netcatclient_H
#define netcatclient_H

#include <QTcpSocket>
#include <QByteArray>
#include <QThread>

#include "netcatpaste.h"

class netcatclient : public QObject {
    Q_OBJECT

    public:
        netcatclient(netcatpaste *server, QTcpSocket *socket);
        virtual ~netcatclient();

        void run();

    private slots:
        void readyRead();
        void readChannelFinished();
        void aboutToClose();

    protected:
        netcatpaste *server;

    private:
        void close();

        int fd_dupe;
        QTcpSocket *socket;
        QByteArray data;
};

class netcatclientthread : public QThread, public netcatclient {

    public:
        netcatclientthread(netcatpaste *server, QTcpSocket *socket) : netcatclient(server, socket) { }
        void run();
};

#endif // netcatclient_H
