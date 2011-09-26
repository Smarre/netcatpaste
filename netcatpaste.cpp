
#include <QTcpSocket>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QtCrypto>
#include <QUuid>

#include "netcatpaste.h"
#include "netcatclient.h"

netcatpaste::netcatpaste() :
        paste_dir("paste"),
        paste_site("http://p.smar.fi") {

    checkPasteDir();

    bool succeeded = server.listen(QHostAddress::Any, 1234);
    if(!succeeded) {
        qDebug()<<"Failed to bind the server to port 1234";
        return;
    }
    connect(&server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void netcatpaste::checkPasteDir() {
    QDir dir(".");
    dir.mkdir(paste_dir);
}


void netcatpaste::newConnection() {
    QTcpSocket *socket = server.nextPendingConnection();
    qDebug()<<"New connection from " << socket->peerAddress().toString() << ":" << int(socket->peerPort());
    if(socket == NULL)
        return;

    netcatclientthread *clientthread = new netcatclientthread(this, socket);
    if(clientthread == NULL) {
        socket->close();
        return;
    }
    // FIXME: Locking!
    clientthread_list.append(clientthread);
    clientthread->start();
}

void netcatpaste::clientthreadTerminate(netcatclientthread *clientthread) {
    // FIXME: Locking!
    clientthread_list.removeAll(clientthread);
}

QString netcatpaste::createPaste(QByteArray byte_array) {
    QCA::Hash hash = QCA::Hash("sha1");
    QCA::SecureArray secure_array(byte_array);
    hash.update(secure_array);
    QCA::SecureArray result_array = hash.final();
    QString result_string = QCA::arrayToHex(result_array.toByteArray());
    hash.clear();
    QString file_name = paste_dir + "/" + result_string;
    QFile file(file_name);
    if(!file.open(QFile::WriteOnly)) {
        qDebug() << "Weren’t able to open file" << file_name;
    }
    QTextStream out(&file);
    out << byte_array;
    return result_string;
}

QString netcatpaste::buildUrl(const QString &file_name) {
     return paste_site + "/" + file_name;
}

netcatpaste::~netcatpaste() {
    //nya
}

#include "netcatpaste.moc"
