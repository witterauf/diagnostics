#include "StandaloneSnippet.h"

namespace diagnostics {

StandaloneSnippet::StandaloneSnippet(const std::string& snippet, size_t cursor)
    : m_snippet{ snippet }, m_cursor{ cursor } {}

StandaloneSnippet::StandaloneSnippet(const std::string& snippet, const Range& range, size_t cursor)
    : m_snippet{ snippet }, m_markedRange{ range }, m_cursor{ cursor } {}

auto StandaloneSnippet::get() const -> std::string
{
    return m_snippet;
}

auto StandaloneSnippet::markedRange() const -> std::optional<Range>
{
    return m_markedRange;
}

auto StandaloneSnippet::cursor() const -> size_t
{
    return m_cursor;
}

}