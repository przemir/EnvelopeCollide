/// Author: Przemysław Bągard

#include <QApplication>
#include "MainWindow.h"
#include "ModelData.h"
#include "QFile"
#include "QTextStream"
#include "Geometry/Point.h"
#include "QTextCodec"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    a.setApplicationName(QObject::tr("CollisionExample"));
    a.setApplicationVersion(QObject::tr("1.0"));
    a.setOrganizationName(QObject::tr("Przemysław Bągard"));

    MainWindow w;
    w.show();
    
    return a.exec();
}
