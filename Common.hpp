#pragma once
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"

namespace rj = rapidjson;

namespace chat{
    inline std::shared_ptr<rj::Document> MakeResponse( int code, const std::string& status ){
        auto doc = std::make_shared<rj::Document>();
        rj::SetValueByPointer( *doc, "/response/code", code );
        rj::SetValueByPointer( *doc, "/response/status", rj::Value ( status.c_str(), status.size(), doc->GetAllocator() ) );
        return doc;
    }
}