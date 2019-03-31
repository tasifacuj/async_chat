#ifndef TRANSPORTRESPONSEINTERFACE_HPP
#define TRANSPORTRESPONSEINTERFACE_HPP

#include "rapidjson/document.h"

#include <QString>
namespace FiveInRow {
  class TransportResponseInterface{
  public:
    virtual ~TransportResponseInterface(){}
    virtual void onConnected() = 0;
    virtual void onReadCompleted( const rapidjson::Document& d ) = 0;
    virtual void onWriteError( const QString& err ) = 0;
    virtual void onParseError( const QString& err ) = 0;
  };

}

#endif // TRANSPORTRESPONSEINTERFACE_HPP
