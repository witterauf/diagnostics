#pragma once

#include "diagnostics/DiagnosticSnippet.h"

namespace diagnostics {

class StandaloneSnippet : public DiagnosticSnippet
{
public:
    explicit StandaloneSnippet(const std::string& snippet, size_t cursor);
    explicit StandaloneSnippet(const std::string& snippet, const Range& range, size_t cursor);

    auto get() const -> std::string override;
    auto markedRange() const -> std::optional<Range> override;
    auto cursor() const -> size_t override;

private:
    std::string m_snippet;
    std::optional<Range> m_markedRange;
    size_t m_cursor;
};

}