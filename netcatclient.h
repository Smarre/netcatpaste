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

    protected:
        netcatpaste *server;

    private:
        void close();

        /**
         * We need to duplicate the socket, as QTcpSocket does not support TCP half-close.
         *
         * When data is send to netcatpaste, after EOF of the data, the client closes the connection,
         * but keeps receiving stream still open. QTcpSocket can’t recognize that the stream is still
         * open, hence refuses to send data to the server.
         */
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
