#pragma once
#include "iostream"

enum class ENTITIES_CHARACTERS {MARIO = '@',  BARREL = 'O', DONKEY_KONG = '&', PRINCESS = '$' };
enum class BOARD_CHARACTERS { LADDER = 'H', RIGHT_FLOOR = '>', LEFT_FLOOR = '>', FLOOR = '=', AIR = ' ' };
enum class KEYS {UP = 'w', DOWN = 'x', STAY = 's', RIGHT = 'd', LEFT = 'a', ESC = 27, INVALID };
enum class BOARD_BOUNDRIES {MAX_X = 80, MIN_X = 0, MAX_Y = 25, MIN_Y = 0};

inline bool operator==(char ch, BOARD_CHARACTERS bCh) {
    return ch == static_cast<char>(bCh);
}

inline bool operator==(BOARD_CHARACTERS bCh, char ch) {
    return ch == bCh;
}

inline bool operator==(char ch, ENTITIES_CHARACTERS bCh) {
    return ch == static_cast<char>(bCh);
}

inline bool operator==(ENTITIES_CHARACTERS bCh, char ch) {
    return ch == bCh;
}

inline bool operator==(char ch, KEYS key) {
    return ch == static_cast<char>(key);
}

inline bool operator==(KEYS key, char ch) {
    return ch == key;
}

inline KEYS charToKey(char key) {
    switch (std::tolower(key)) {
    case 'w': return KEYS::UP;
    case 'x': return KEYS::DOWN;
    case 'a': return KEYS::LEFT;
    case 'd': return KEYS::RIGHT;
    case 's': return KEYS::STAY;
    case 27: return KEYS::ESC;
    default: return KEYS::INVALID;
    }
}