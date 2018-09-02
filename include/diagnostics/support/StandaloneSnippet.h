#pragma once

#include "diagnostics/DiagnosticSnippet.h"
#include "diagnostics/LineAndColumn.h"

namespace diagnostics {

class StandaloneSnippet : public DiagnosticSnippet
{
public:
    explicit StandaloneSnippet(const LineAndColumnRange& sourceRange, const LineAndColumn& cursor);

    void append(const Line& line);

    auto lineCount() const -> size_t override;
    auto line(size_t line) const -> Line override;
    auto sourceRange() const -> LineAndColumnRange override;
    auto cursor() const -> LineAndColumn override;

private:
    std::vector<Line> m_lines;
    LineAndColumnRange m_sourceRange;
    LineAndColumn m_cursor;
};

}