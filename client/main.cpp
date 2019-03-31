#include <QtGui>
#include <QtGui>
#include <QDebug>

#include <memory>

#include "MainViewModel.hpp"

int main( int argc, char **argv ){
    
    QGuiApplication app(argc, argv);
    FiveInRow::MainViewModel mvm;
    qDebug() << "argc = " << argc;
    if( argc == 2 ){
        QString name = argv[ 1 ];
        qDebug() << "Name = " << name;
        mvm.init( argv[ 1 ] );
    }else{
        mvm.init( "NoName" );
    }

    mvm.show();
    return app.exec();
}
