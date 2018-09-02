#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace diagnostics {

struct LineAndColumn;
struct LineAndColumnRange;

class DiagnosticSnippet
{
public:
    struct Range
    {
        size_t start, end;
    };

    struct Line
    {
        size_t number;
        std::optional<Range> mark;
        std::optional<size_t> cursor;
        std::string text;
    };

    virtual ~DiagnosticSnippet() = default;

    virtual auto lineCount() const -> size_t = 0;
    virtual auto line(size_t line) const -> Line = 0;
    virtual auto sourceRange() const -> LineAndColumnRange = 0;
    virtual auto cursor() const -> LineAndColumn = 0;
};

}