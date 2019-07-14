#pragma once

#include <cstddef>

namespace diagnostics {

struct LineAndColumn
{
    size_t line = 1;
    size_t column = 1;

    static constexpr size_t EndOfLine = ~0UL;
    static constexpr size_t EndOfSource = ~0UL;

    bool isSpecial() const
    {
        return isEndOfLine() || isEnd();
    }

    bool isEndOfLine() const
    {
        return column == EndOfLine;
    }

    bool isEnd() const
    {
        return line == EndOfSource && isEndOfLine();
    }

    bool isValid() const
    {
        return line >= 1 && column >= 1;
    }

    auto operator<(const LineAndColumn& rhs) const
    {
        return line < rhs.line || (line == rhs.line && column < rhs.column);
    }

    auto operator==(const LineAndColumn& rhs) const
    {
        return line == rhs.line && column == rhs.column;
    }

    auto nextLine() const -> LineAndColumn
    {
        return { line + 1, 1 };
    }

    auto nextColumn() const -> LineAndColumn
    {
        return { line, column + 1 };
    }

    static auto endOfLine(size_t line) -> LineAndColumn
    {
        return { line, EndOfLine };
    }
};

struct LineAndColumnRange
{
    LineAndColumn start, end;

    bool isEmpty() const
    {
        return start == end || end < start;
    }

    bool containsLine(size_t line) const
    {
        return line >= start.line && line <= end.line;
    }
};

}