#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtQuick>
#include "GameViewModel.hpp"

namespace FiveInRow {
    class MainViewModel : public QObject
    {
        Q_OBJECT

    public:
        MainViewModel();
        ~MainViewModel();

    public slots:
        void minimizeWindow();
        void exitApplication();
        void show();
        void init( const QString& name );
    private:
        QQmlApplicationEngine engine_;
        GameViewModel gameData_;
    };
}

#endif // MAINWIDGET_H
