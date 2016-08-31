#ifndef STONE_H
#define STONE_H

#include <QString>

enum class Stone: unsigned char {
    None,
    Black,
    White
};

/*
 *  Gets the opponent stone
 */
Stone opponent(Stone stone);

/*
 *  Returns the color name of the stone
 */
const QString& to_string(Stone stone);


#endif // STONE_H
