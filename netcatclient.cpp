
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

#include <QTcpSocket>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QtCrypto>
#include <QUuid>

#include "netcatclient.h"
#include "netcatpaste.h"

netcatclient::netcatclient(netcatpaste *server, QTcpSocket *socket) :
        server(server),
        socket(socket) {

    Q_ASSERT(socket);
    int fd = socket->socketDescriptor();
    fd_dupe = dup(fd);

    if(fd_dupe < 0) {
        qWarning("dup(%d): %s", fd, strerror(errno));
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(readChannelFinished()), this, SLOT(readChannelFinished()));
    connect(socket, SIGNAL(aboutToClose()), this, SLOT(aboutToClose()));

}

void netcatclient::aboutToClose() {
    fprintf(stdout, "netcatclient::aboutToClose()\n");
    fflush(stdout);
}


void netcatclient::readyRead() {
    fprintf(stdout, "netcatclient::readyRead()\n");
    fflush(stdout);

    if(!socket->atEnd()) {
        while(true) {
            QByteArray received = socket->read(4096);
            if(received.isNull()) {
                break;
            }
            fprintf(stdout, "netcatclient::readyRead() got=%d\n", received.length());
            data.append(received);
        }

        fprintf(stdout, "netcatclient::readyRead() DONE\n");
    } else {
        fprintf(stdout, "netcatclient::readyRead() ATEND\n");
        fflush(stdout);
    }
}

void netcatclient::readChannelFinished() {
    fprintf(stdout, "netcatclient::readChannelFinished()\n");
    fflush(stdout);
    if(fd_dupe >= 0) {
        shutdown(fd_dupe, SHUT_RD);
    }

    QString file_name = server->createPaste(data);
    QString url = server->buildUrl(file_name);
    //QString message = "Your paste can be found at:\n" + url + "\n";
    QString message = url + "\n";

    if(fd_dupe >= 0) {
        //socket->write(message.toUtf8().data()); // broken Qt socket shutdown() handling
        //socket->flush();
        //socket->close();
        QByteArray ba = message.toUtf8();
        int len = ba.length();
        const void *data = ba.data();
        if(write(fd_dupe, data, len) != len)
            qWarning("write(%d, ..., %d): %s", fd_dupe, len, strerror(errno));
        shutdown(fd_dupe, SHUT_WR);
    }
    close();
}

void netcatclient::close() {
fprintf(stdout, "netcatclient::close()\n");
fflush(stdout);
    if(fd_dupe >= 0) {
        ::close(fd_dupe);
        fd_dupe = -1;
    }
    if(socket != NULL) {
        socket->close();
        //delete socket;
        socket = NULL;
    }
}

netcatclient::~netcatclient() {
    close();
}

void netcatclientthread::run() {
    exec();
    server->clientthreadTerminate(this);
}

#include "netcatclient.moc"
