#pragma once

#include <string>

struct TitleInfo
{
    enum class Type { Title, Req, Counter };
    
    struct Req
    {
        bool isSuited;
    };

    struct Counter
    {
        int current;
        int maximum;
    };

    Type type{Type::Title};

    std::string title{""};

    union
    {
        Req req;
        Counter counter;
    };  
    
    void makeTitle(const std::string& _title)
    {
        type = Type::Title;
        this->title = _title;
    }
    void makeReq(const std::string& title, bool isSuited)
    {
        this->title = title;
        type = Type::Req;
        req.isSuited = isSuited;
    }
    void makeCounter(const std::string& title, int current, int maximum)
    {
        this->title = title;
        type = Type::Counter;
        counter.current = current;
        counter.maximum = maximum;
    }
};