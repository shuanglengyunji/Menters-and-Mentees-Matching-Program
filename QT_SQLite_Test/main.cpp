#include <QCoreApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "QT_SQLite_Test\n";

    return a.exec();
}
