#ifndef netcatpaste_H
#define netcatpaste_H

#include <QTcpServer>

class netcatpaste : public QObject {
    Q_OBJECT

    public:
        netcatpaste();
        virtual ~netcatpaste();

    private slots:
        void newConnection();
        //! returns resulted file name
        QString createPaste(QString string);
        void checkPasteDir();

    private:
        QTcpServer server;
        QString paste_dir;
        QString paste_site;
};

#endif // netcatpaste_H
