#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtQuick>
#include "GameViewModel.hpp"

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

private:
    QQmlApplicationEngine engine_;
    GameViewModel m_gameData;
};

#endif // MAINWIDGET_H
