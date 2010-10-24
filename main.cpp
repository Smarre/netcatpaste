
#include <QCoreApplication>
#include <QtCrypto>

#include "netcatpaste.h"

int main(int argc, char** argv) {
    QCA::Initializer init;
    if(!QCA::isSupported("sha1")) {
        qWarning()<<QString::fromUtf8(
                "MD2 isn't supported, the program won't work!");
        exit(1);
    }

    QCoreApplication app(argc, argv);
    netcatpaste foo;
    return app.exec();
}
