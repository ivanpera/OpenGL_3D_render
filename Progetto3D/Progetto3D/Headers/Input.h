#pragma once
#include <string>
#include <map>

enum class State {
    PRESSED,
    HELD,
    RELEASED,
    NOT_PRESSED
};

class Input {
private:
    std::map<std::string, State> keys;
public:
    void setKey(std::string const& key);
    void setKeyUp(std::string const& key);
    bool getKeyDown(std::string const& key);
    bool getKey(std::string const& key);
    bool getKeyUp(std::string const& key);
    void update();
};