#ifndef netcatpaste_H
#define netcatpaste_H

#include <QTcpServer>
#include <QList>

class netcatclientthread;	// fwd decl

class netcatpaste : public QObject {
    Q_OBJECT

    public:
        netcatpaste();
        virtual ~netcatpaste();

        void clientthreadTerminate(netcatclientthread *clientthread);
        //! returns resulted file name
        QString createPaste(QByteArray byte_array);
        QString buildUrl(const QString &file_name);
        void checkPasteDir();

    private slots:
        void newConnection();

    private:
        QTcpServer server;
        QString paste_dir;
        QString paste_site;
        QList<netcatclientthread *> clientthread_list;
};

#endif // netcatpaste_H
