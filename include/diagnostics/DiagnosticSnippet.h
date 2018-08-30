#pragma once

#include <cstddef>
#include <optional>
#include <string>

namespace diagnostics {

class DiagnosticSnippet
{
public:
    struct Range
    {
        size_t start, end;
    };

    virtual ~DiagnosticSnippet() = default;

    virtual auto get() const -> std::string = 0;
    virtual auto markedRange() const -> std::optional<Range> = 0;
    virtual auto cursor() const -> size_t = 0;
};

}