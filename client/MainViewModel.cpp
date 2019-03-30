#include <QtQuick>
#include <QUrl>
#include <QTimer>


#include "MainViewModel.hpp"
#include "Tile.hpp"


MainViewModel::MainViewModel(){
    // Register Tile to be available in QML
    qmlRegisterType<Tile>("gameCore", 1, 0, "Tile");

    // Setup context
    engine_.rootContext()->setContextProperty("mainWidget", this);
    engine_.rootContext()->setContextProperty("gameData", &m_gameData);

//    // Set view optimizations not already done for QDeclarativeView
//    setAttribute(Qt::WA_OpaquePaintEvent);
//    setAttribute(Qt::WA_NoSystemBackground);

    // Make QDeclarativeView use OpenGL backend
//    QGLWidget *glWidget = new QGLWidget(this);
//    setViewport(glWidget);
//    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Open root QML file
    engine_.load( QUrl( QStringLiteral("qrc:/qml/MainView.qml") ) );
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
