#include <QCoreApplication>
#include <QDebug>
#include <QProcess>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "QT_SQLite_Test\n";

    QProcess process;
    process.start("cmd.exe < D:\\Menters-and-Mentees-Matching-Program\\build-QT_SQLite_Test-Desktop_Qt_5_12_3_MinGW_32_bit-Debug\\debug\\cmd.bat");


    //process.setProgram("cmd");
    //QStringList argument("D:\\Menters-and-Mentees-Matching-Program\\build-QT_SQLite_Test-Desktop_Qt_5_12_3_MinGW_32_bit-Debug\\debug\\cmd.bat");
    //process.setArguments(argument);

    process.start("cmd.exe < D:\\Menters-and-Mentees-Matching-Program\\build-QT_SQLite_Test-Desktop_Qt_5_12_3_MinGW_32_bit-Debug\\debug\\cmd.bat");

    QString temp = QString::fromLocal8Bit(process.readAllStandardOutput()); //程序输出信息
    qDebug() << temp;

    return a.exec();
}


