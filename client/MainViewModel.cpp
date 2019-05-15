#include <QtQuick>
#include <QUrl>
#include <QTimer>

#include "MainViewModel.hpp"
#include "Tile.hpp"


namespace FiveInRow {
MainViewModel::MainViewModel(){
    // Register Tile to be available in QML
    qmlRegisterType<Tile>("gameCore", 1, 0, "Tile");

    // Setup context
    engine_.rootContext()->setContextProperty("mainWidget", this);
    engine_.rootContext()->setContextProperty("gameData", &gameData_);

    // Open root QML file
    engine_.load( QUrl( QStringLiteral("qrc:/qml/MainView.qml") ) );

    connect( &gameData_, &GameViewModel::remoteStartChanged, [ this ](){
        qDebug()<<"Set QML property: remoteStart -> " << gameData_.getRemoteStart();
        auto wnd = qobject_cast<QQuickWindow*>( engine_.rootObjects().value( 0 ) );
        QQmlProperty::write( wnd, "remoteStart", gameData_.getRemoteStart());
    } );
}

MainViewModel::~MainViewModel()
{
}

void MainViewModel::minimizeWindow(){
}

void MainViewModel::exitApplication()
{
    QGuiApplication::quit();
}

void MainViewModel::show(){
  QObject* topLevel = engine_.rootObjects().value( 0 );
  QQuickWindow* wnd = qobject_cast<QQuickWindow*>( topLevel );
  wnd->show();
}

void MainViewModel::init( const QString& name ){
    gameData_.init( name );
}

}// namespace FiveInRow
