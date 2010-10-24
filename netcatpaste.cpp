
#include <QTcpSocket>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QtCrypto>
#include <QUuid>

#include "netcatpaste.h"

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
    qDebug()<<"New connection...";
    QTcpSocket *socket = server.nextPendingConnection();
    QByteArray array;
    QString string;
    if(!socket->waitForReadyRead()) {
        return;
    }
    qint64 left_to_read;
    int counter = 100;
    forever {
        if(counter <= 0) {
            break;
        }
        if(!socket->bytesAvailable()) {
            counter--;
            usleep(100);
            continue;
        }
        char buffer[1024];
        left_to_read = socket->readLine(buffer, 1024);
        QString data = buffer;
        string.append(data);
        if(left_to_read <= 0) {
            usleep(10);
            counter--;
        }
    }

    QString file_name = createPaste(string);

    QString url = paste_site + "/" + file_name;
    //QString message = "Your paste can be found at:\n" + url + "\n";
    QString message = url + "\n";
    socket->write(message.toUtf8().data());
    socket->close();
}

QString netcatpaste::createPaste(QString string) {
    QCA::Hash hash = QCA::Hash("sha1");
    QByteArray byte_array(string.toUtf8());
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
    out << string;
    return result_string;
}


netcatpaste::~netcatpaste() {
    //nya
}

#include "netcatpaste.moc"
