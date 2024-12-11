#pragma once

#include "jsontypes.h"
#include <sstream>



class JsonTokenizer
{
public:
    explicit JsonTokenizer(const std::string& _jsonStr) : jsonStr_(_jsonStr) {}

    JsonToken NextToken();

private:
    std::string jsonStr_;
    uint32_t pos_ = 0;


private:
    void SkipWhiteSpace();
    JsonToken ParseString();
    JsonToken ParseNumber();
    bool IsMatchLiteral(const std::string& literal);
};