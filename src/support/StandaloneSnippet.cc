#include "support/StandaloneSnippet.h"
#include "Contracts.h"

namespace diagnostics {

StandaloneSnippet::StandaloneSnippet(const LineAndColumnRange& sourceRange, const LineAndColumn& cursor)
    : m_sourceRange{ sourceRange }, m_cursor{ cursor }
{
    Expects(!sourceRange.isEmpty());
}

void StandaloneSnippet::append(const Line& line)
{
    Expects(m_lines.empty() || line.number == m_lines.back().number + 1);
    m_lines.push_back(line);
}

auto StandaloneSnippet::lineCount() const -> size_t
{
    Expects(m_lines.size() == m_sourceRange.end.line - m_sourceRange.start.line + 1);
    return m_lines.size();
}

auto StandaloneSnippet::line(size_t line) const -> Line
{
    Expects(line < lineCount());
    return m_lines[line];
}

auto StandaloneSnippet::sourceRange() const -> LineAndColumnRange
{
    return m_sourceRange;
}

auto StandaloneSnippet::cursor() const -> LineAndColumn
{
    return m_cursor;
}

}