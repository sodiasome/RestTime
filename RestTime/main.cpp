#include "MainDlg.h"

#include <QApplication>
#include <QDebug>
#include <QTextCodec>
#include <string>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     //a.setWindowIcon(QIcon("C:\\Users\\sodia\\Desktop\\title.ico"));
    a.setWindowIcon(QIcon("img\\title.ico"));

    Dialog w;
    //w.show();
    return a.exec();
}
