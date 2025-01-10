#pragma once
#include <iostream>

/**
 * @brief Enumerations for game characters, board elements, and input keys.
 */

 // ------------------- Enumerations -------------------
enum class ENTITIES_CHARACTERS { MARIO = '@', BARREL = 'O', DONKEY_KONG = '&', PAULINA = '$', GHOST = 'X' };
enum class BOARD_CHARACTERS { LADDER = 'H', RIGHT_FLOOR = '>', LEFT_FLOOR = '<', FLOOR = '=', AIR = ' ' };
enum class KEYS { UP = 'w', DOWN = 'x', STAY = 's', RIGHT = 'd', LEFT = 'a', ESC = 27, INVALID };

// ------------------- Operators -------------------
/**
 * @brief Overloaded operators for comparing enumerations with characters.
 * These operators allow direct comparison between a character (`char`)
 * and the values of BOARD_CHARACTERS, ENTITIES_CHARACTERS, or KEYS enumerations,
 * avoiding the need for explicit casting.
 */

inline bool operator==(char ch, BOARD_CHARACTERS bCh) {
    return ch == static_cast<char>(bCh);
}

inline bool operator==(BOARD_CHARACTERS bCh, char ch) {
    return ch == bCh;
}

inline bool operator!=(char ch, BOARD_CHARACTERS bCh) {
    return ch != static_cast<char>(bCh);
}

inline bool operator!=(BOARD_CHARACTERS bCh, char ch) {
    return ch != bCh;
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

// ------------------- Utility Functions -------------------
/**
 * @brief Converts a character input into a KEYS enumeration.
 * @param key The character to convert.
 * @return The corresponding KEYS value.
 */
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
