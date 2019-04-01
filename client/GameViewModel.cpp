#include "GameViewModel.hpp"

#include <QDebug>
#include <QThread>
#include "rapidjson/pointer.h"

namespace FiveInRow {

const int TILE_ROWS = 19;
const int TILE_COLS = 19;
namespace  rj = rapidjson;

GameViewModel::GameViewModel(QObject *parent)
    : QObject(parent)
    , gameOn_(false)
    , player1Turn_(true)
    , moves_(0)
    , gameTimeSeconds_(0)
    , gameTime_(0,0)
    , time_("0:00")
    , transport_( "127.0.0.1", "2504" )
{
    // Create tiles
    for(int i = 0; i < TILE_ROWS * TILE_COLS; ++i) {
        tiles_ << new Tile;
    }

    // Timer to update the game time
    gameTimer_ = new QTimer(this);
    connect(gameTimer_, SIGNAL(timeout()), this, SLOT(updateTime()));
    gameTimer_->setInterval(1000);
    transport_.registerRif( this );
    defaultHandler_ = []( const rj::Document& ){};
}

GameViewModel::~GameViewModel()
{
    transport_.shutdown();
    transport_.join();
}

void GameViewModel::resetGame(bool remote){
    setMoves(0);
    foreach(Tile *t, tiles_) {
        t->setHasButton1(false);
        t->setHasButton2(false);
        t->setHighlighted(false);
    }
    selectedTiles_.clear();
    gameTimeSeconds_ = 0;
    setGameTime("0:00");
    setPlayer1Turn(true);
    setGameOn(true);

    if( false == remote ){
        auto inviteReq = std::make_shared<rj::Document>();
        rj::SetValueByPointer( *inviteReq, "/request/invite/from", rj::Value( userName_.toLatin1(), inviteReq->GetAllocator() ) );
        rj::SetValueByPointer( *inviteReq, "/request/invite/to", rj::Value( rhs_.toLatin1(), inviteReq->GetAllocator() ) );
        defaultHandler_ = []( const rj::Document& doc ){
            const rj::Value* v = rj::GetValueByPointer( doc, "/status" );

            if( v && v->GetString() == "Ok" ){
                qDebug() << "invite ok";
            }else{
                qCritical() << "Invite failed ";
            }
        };

        transport_.send( inviteReq );
    }
}

void GameViewModel::pauseGame(bool state)
{
    if (state)
        gameTimer_->stop();
    else
        gameTimer_->start();
}



void GameViewModel::flip(int index){
    if( false == remoteStart_ and false == player1Turn() )
        qWarning() << "Wait for player 2";
    else if( remoteStart_ and player1Turn() )
        qWarning() << "wait for player 1";
    else {
        flipInternal( index );
        auto msg = std::make_shared<rj::Document>();
        rj::SetValueByPointer( *msg, "/request/message/from", rj::Value( userName_.toLatin1(), msg->GetAllocator() ) );
        rj::SetValueByPointer( *msg, "/request/message/to", rj::Value( rhs_.toLatin1(), msg->GetAllocator() ) );
        rj::SetValueByPointer( *msg, "/request/message/index", index );
        transport_.send( msg );
    }
}

void GameViewModel::undoTile()
{
    if (!selectedTiles_.empty())
    {
        // Clear button and unset highlight
        selectedTiles_.last()->setHasButton1(false);
        selectedTiles_.last()->setHasButton2(false);
        selectedTiles_.last()->setHighlighted(false);
        selectedTiles_.pop();

        // Set new last tile highlighted
        if (!selectedTiles_.empty()) selectedTiles_.last()->setHighlighted(true);

        setMoves( moves_ - 1 );
        setPlayer1Turn(!player1Turn());
    }
}

void GameViewModel::updateTime()
{
    if ( gameOn_ ) gameTimeSeconds_++;
    setGameTime( gameTime_.addSecs( gameTimeSeconds_ ).toString("m:ss"));
}

bool GameViewModel::checkWin(int index, int dx, int dy, QList<Tile*> &winningTiles)
{
    int count = 0;
    for (int i=-4; i<5; ++i) {
        Tile *t = tile(index + TILE_COLS*i*dx + i*dy);
        if (t && ((player1Turn()&&t->hasButton1()) || (!player1Turn()&&t->hasButton2()))) {
            count++;
            winningTiles.append(t);
        } else {
            count = 0;
            winningTiles.clear();
        }
        if (count==5) return true;
    }
    return false;
}
//-----------------------------------------------------------------------
// TransportResponseIterface
void GameViewModel::onReadCompleted( const rapidjson::Document& doc ){
    if( rapidjson::GetValueByPointer( doc, "/request/invite" ) )
        dispatchInvite( doc );
    else if( rapidjson::GetValueByPointer( doc, "/request/message" ) )
        dispatchMessage( doc );
    else if( rapidjson::GetValueByPointer( doc, "/participants" ) )
        dispatchQuery( doc );
    else if( rapidjson::GetValueByPointer( doc, "/request/register" ) )
        dispatchRegister( doc );
    else
        defaultHandler_( doc );
}

void GameViewModel::onWriteError( const QString &err ){
    qCritical() << err;
}

void GameViewModel::onParseError(const QString &err){
    qCritical() << err;
}

void GameViewModel::selectUser( int idx ){
    qDebug() << "Selected user " << users_[ idx ];
    rhs_ = users_[ idx ];
}

void GameViewModel::dispatchInvite( const rapidjson::Document& doc ){
    const rj::Value* from = rj::GetValueByPointer( doc, "/request/invite/from" );
    const rj::Value* to = rj::GetValueByPointer( doc, "/request/invite/to" );
    assert( userName_ == to->GetString() );
    rhs_ = from->GetString();
    setRemoteStart( true );//TODO: show gameboard
}

void GameViewModel::dispatchMessage( const rapidjson::Document& msg ){
    int idx = msg[ "request" ][ "message" ][ "index" ].GetInt();
    QString from = msg[ "request" ][ "message" ][ "from" ].GetString();
    QString to = msg[ "request" ][ "message" ][ "to" ].GetString();
    qDebug() << "From = " << from << ", to = " << to << ", idx = " << idx;
    assert( to == userName_ && from == rhs_ );
    flipInternal( idx );
}

void GameViewModel::onConnected(){
    auto registerReq = std::make_shared<rj::Document>();
    rj::SetValueByPointer( *registerReq, "/request/register/from", rj::Value( userName_.toLatin1(), registerReq->GetAllocator() ) );
    transport_.send( registerReq );
}

void GameViewModel::dispatchQuery( const rapidjson::Document& doc ){
    const rj::Value* usersV = rj::GetValueByPointer( doc, "/participants" );

    if( usersV && usersV->IsArray() ){
        const rj::Value& userRef = *usersV;

        for( rj::SizeType i = 0; i < usersV->Size(); i++ ){
           QString u( userRef[ i ].GetString() );
           qDebug() << ">>>> " << u;

            if( u != userName_ && !users_.contains( u ) )
                users_.append( u );
        }

        emit usersChanged();
    }
}

void GameViewModel::dispatchRegister( const rapidjson::Document& doc ){
    const rj::Value* v = rj::GetValueByPointer( doc, "/request/register/from" );

    if( v ){
        QString user = v->GetString();
        qDebug() << "New registration from " << user;

        if( userName_ != user ){
            users_.append( user );
            emit usersChanged();
        }else{
            qWarning() << userName_  << " == " << user;
        }
    }

    auto qReq = std::make_shared<rj::Document>();
    rj::SetValueByPointer( *qReq, "/request/query", rj::Value( rj::kNullType ) );
    transport_.send( qReq );
}

void GameViewModel::init( const QString& name ){
    userName_ = name;
    transport_.run();
}

void GameViewModel::flipInternal( int index ){
    if (!gameOn_)
        return;

    // TODO: implement message
    // TODO: implement show board from remote request

    Tile *t = tile(index);
    if (!t || t->hasButton1() || t->hasButton2())
        return;

    setMoves(moves_ + 1);

    if (player1Turn()) {
        t->setHasButton1(true);
    } else {
        t->setHasButton2(true);
    }

    // Check for winning
    QList<Tile *> winningTiles;
    if (checkWin(index, 1, 0, winningTiles) || checkWin(index, 0, 1, winningTiles) ||
        checkWin(index, 1, 1, winningTiles) || checkWin(index, 1, -1, winningTiles)) {
        // Player whose turn it was won

        // Highlight the winning tiles
        selectedTiles_.last()->setHighlighted(false);
        for(int i=0 ; i<winningTiles.count() ; ++i) {
            winningTiles.at(i)->setHighlighted(true);
        }

        setGameOn(false);
        return;
    }

    // Set only last tile highlighted
    if (!selectedTiles_.empty()) selectedTiles_.last()->setHighlighted(false);
    t->setHighlighted(true);

    // Add tile into selected list, for undo
    selectedTiles_ << t;

    setPlayer1Turn(!player1Turn());
}

}// namespace FiveInnRow
