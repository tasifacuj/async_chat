#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStack>
#include <QTime>
#include <QTimer>
#include <QQmlListProperty>

#include "Tile.hpp"
#include "TransportResponseInterface.hpp"
#include "Transport.hpp"

namespace FiveInRow {
    class GameViewModel : public QObject
            , public TransportResponseInterface
    {
        Q_OBJECT
    private: // == Members ==
        QList<Tile *>           tiles_;
        QStack<Tile *>          selectedTiles_;
        bool                    gameOn_;
        bool                    player1Turn_;
        int                     moves_;
        int                     gameTimeSeconds_;
        QTime                   gameTime_;
        QTimer*                 gameTimer_;
        QString                 time_;
        transport::Transport    transport_;
        QStringList             users_;
        QString                 userName_;
    public:
//        static constexpr char* Name = "User 0";
    public:
        GameViewModel(QObject *parent=0);
        ~GameViewModel();
    public: // == Properties ==
        Q_PROPERTY(int moves READ moves WRITE setMoves NOTIFY movesChanged);
        int moves() const {return moves_;}
        void setMoves(int moves) {if(moves==moves_) return; moves_ = moves; emit movesChanged();}

        Q_PROPERTY(QString gameTime READ gameTime WRITE setGameTime NOTIFY gameTimeChanged);
        QString gameTime() const {return time_;}
        void setGameTime(const QString &time) {if(time==time_) return; time_ = time; emit gameTimeChanged();}

        Q_PROPERTY(bool player1Turn READ player1Turn WRITE setPlayer1Turn NOTIFY player1TurnChanged);
        bool player1Turn() const {return player1Turn_;}
        void setPlayer1Turn(bool state) {if(state==player1Turn_) return; player1Turn_ = state; emit player1TurnChanged();}

        Q_PROPERTY(bool gameOn READ gameOn WRITE setGameOn NOTIFY gameOnChanged);
        bool gameOn() const {return gameOn_;}
        void setGameOn(bool on) {if(on==gameOn_) return; gameOn_ = on; emit gameOnChanged();}

        Q_PROPERTY(QQmlListProperty<Tile> tiles READ tiles CONSTANT);
        QQmlListProperty<Tile> tiles() {return QQmlListProperty<Tile>(this, tiles_);}

        Q_PROPERTY( QStringList users READ getUsers WRITE setUsers NOTIFY usersChanged);
        QStringList getUsers(){ return users_; }
        void setUsers( const QStringList& users ){ users_ = users; }


        Q_INVOKABLE void selectUser( int idx );

        void init( const QString& name );
    public: // == TransportResponseInterface ==
        virtual void onReadCompleted( const rapidjson::Document& d ) override;
        virtual void onWriteError( const QString& err ) override;
        virtual void onParseError( const QString& err ) override;
        virtual void onConnected() override;
    public slots:
        void resetGame();
        void pauseGame(bool state);
        void flip(int index);
        void undoTile();
        void updateTime();

    signals:
        void movesChanged();
        void gameTimeChanged();
        void player1TurnChanged();
        void gameOnChanged();
        void usersChanged();

    private: //== internals ==
        Tile *tile(int index) const {return (index >= 0 && index < tiles_.count()) ? tiles_.at(index) : 0;}
        bool checkWin(int index, int dx, int dy, QList<Tile *> &winningTiles);
        void dispatchInvite( const rapidjson::Document& doc );
        void dispatchMessage( const rapidjson::Document& doc );
        void dispatchQuery( const rapidjson::Document& doc );
        void dispatchRegister( const rapidjson::Document& doc );
    };
}

#endif // GAMEDATA_H
