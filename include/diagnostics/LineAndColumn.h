#pragma once

#include <cstddef>

namespace diagnostics {

struct LineAndColumn
{
    size_t line = 1;
    size_t column = 1;

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
};

}