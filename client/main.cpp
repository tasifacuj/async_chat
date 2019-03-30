#include <QtGui>
#include "MainViewModel.hpp"
#include <QtGui>
int main( int argc, char **argv ){
    
    QGuiApplication app(argc, argv);
    MainViewModel mvm;
//    mvm.resize( 1800, 900 );
    mvm.show();
    return app.exec();
}
