#pragma once

struct BoardPosition
{
    int row = 0;
    int column = 0;
};

inline bool operator==(const BoardPosition& left, const BoardPosition& right)
{
    return left.row == right.row && left.column == right.column;
}
