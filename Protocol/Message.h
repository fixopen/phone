#pragma once

#include <vector>
#include <string>
#include <map>

class Message {
public:
    virtual ~Message();
    std::map<std::string, std::string> HeaderFields;
    std::string Body;
    std::string const ToString() const;
};
