#ifndef STONE_H
#define STONE_H

#include <QString>

enum class Stone: unsigned char {
    None,
    Black,
    White
};

Stone opponent(Stone stone);
const QString& to_string(Stone stone);


#endif // STONE_H
