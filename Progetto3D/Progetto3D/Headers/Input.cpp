#include "Input.h"
void Input::setKey(std::string const& key)
{
    auto const it = keys.find(key);
    if (it == keys.end()) {
        keys.insert({ key, State::PRESSED});
    }
    else {
        if (it->second == State::PRESSED) {
            it->second = State::HELD;
        }
        else if (it->second != State::HELD) {
            it->second = State::PRESSED;
        }
    }
}

void Input::setKeyUp(std::string const& key)
{
    keys.insert_or_assign(key, State::RELEASED);
}

bool Input::getKeyDown(std::string const& key)
{
    auto const it = keys.find(key);
    if (it == keys.end()) {
        keys.insert({ key, State::NOT_PRESSED });
        return false;
    }
    return it->second == State::PRESSED;
}

bool Input::getKey(std::string const& key)
{
    auto const it = keys.find(key);
    if (it == keys.end()) {
        keys.insert({ key, State::NOT_PRESSED });
        return false;
    }
    return it->second == State::HELD;
}

bool Input::getKeyUp(std::string const& key)
{
    auto const it = keys.find(key);
    if (it == keys.end()) {
        keys.insert({ key, State::NOT_PRESSED });
        return false;
    }
    return it->second == State::RELEASED;
}

void Input::update()
{
    for (auto& k : keys) {
        if (k.second == State::RELEASED) {
            k.second = State::NOT_PRESSED;
        }
        if (k.second == State::PRESSED) {
            k.second = State::HELD;
        }
    }
}
