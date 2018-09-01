#include "support/StandaloneSnippetBuilder.h"
#include "Contracts.h"

namespace diagnostics {

void StandaloneSnippetBuilder::reset()
{
    m_markedRange = {};
    m_sourceRange = {};
    m_cursor = {};
}

void StandaloneSnippetBuilder::setSource(const uint8_t* source, size_t size)
{
    Expects(source);
    Expects(size > 0);
    m_source = source;
    m_size = size;
}

void StandaloneSnippetBuilder::setDecoder(LineColumnDecoder* decoder)
{
    Expects(decoder);
    m_decoder = decoder;
}

auto StandaloneSnippetBuilder::setCursor(size_t offset) -> StandaloneSnippetBuilder&
{
    Expects(offset < m_size);
    m_cursor = offset;
    return *this;
}

auto StandaloneSnippetBuilder::setCursor(const LineAndColumn& position) -> StandaloneSnippetBuilder&
{
    Expects(position.isValid());
    m_cursor = position;
    return *this;
}

auto StandaloneSnippetBuilder::setSourceRange(size_t start, size_t end) -> StandaloneSnippetBuilder&
{
    Expects(start < m_size);
    Expects(end < m_size);
    m_sourceRange = DiagnosticSnippet::Range{start, end};
    return *this;
}

auto StandaloneSnippetBuilder::setSourceRange(const LineAndColumn& start, const LineAndColumn& end)
    -> StandaloneSnippetBuilder&
{
    Expects(start.isValid());
    Expects(end.isValid());
    m_sourceRange = LineColumnRange{start, end};
    return *this;
}

auto StandaloneSnippetBuilder::setMarkedRange(size_t start, size_t end) -> StandaloneSnippetBuilder&
{
    Expects(start < m_size);
    Expects(end < m_size);
    m_markedRange = DiagnosticSnippet::Range{start, end};
    return *this;
}

auto StandaloneSnippetBuilder::setMarkedRange(const LineAndColumn& start, const LineAndColumn& end)
    -> StandaloneSnippetBuilder&
{
    Expects(start.isValid());
    Expects(end.isValid());
    m_markedRange = LineColumnRange{start, end};
    return *this;
}

auto StandaloneSnippetBuilder::onlyFullLines(bool isActive) -> StandaloneSnippetBuilder&
{
    m_onlyFullLines = isActive;
    return *this;
}

auto StandaloneSnippetBuilder::build() -> std::shared_ptr<StandaloneSnippet>
{
    Expects(m_sourceRange.has_value());
    Expects(m_cursor.has_value());

    findSourceOffsets();
    findCursorOffset();
    extractSnippetCode();

    auto snippet = std::make_shared<StandaloneSnippet>(m_snippetCode, m_cursorOffset);

    reset();
    return nullptr;
}

void StandaloneSnippetBuilder::findSourceOffsets()
{
    if (m_onlyFullLines)
    {
        findSourceOffsetFullLines();
    }
    else
    {
        findSourceOffsetsPartialLines();
    }
}

void StandaloneSnippetBuilder::findSourceOffsetFullLines()
{
    LineColumnRange lcRange;

    if (std::holds_alternative<DiagnosticSnippet::Range>(*m_sourceRange))
    {
        auto const range = std::get<DiagnosticSnippet::Range>(*m_sourceRange);
        auto const start = m_decoder->decode(range.start);
        auto const end = m_decoder->decode(range.end);
        lcRange = {start, end};
    }
    else
    {
        lcRange = std::get<LineColumnRange>(*m_sourceRange);
    }

    auto const startLine = lcRange.start.line;
    auto const endLine = lcRange.end.line + 1;

    if (auto maybeOffset = m_decoder->lineOffset(startLine))
    {
        m_startOffset = *maybeOffset;
    }
    else
    {
        throw std::runtime_error{"invalid start line"};
    }
    if (auto maybeOffset = m_decoder->lineOffset(endLine))
    {
        m_endOffset = *maybeOffset;
    }
    else
    {
        throw std::runtime_error{"invalid end line"};
    }
}

void StandaloneSnippetBuilder::findSourceOffsetsPartialLines()
{
    if (std::holds_alternative<DiagnosticSnippet::Range>(*m_sourceRange))
    {
        auto const range = std::get<DiagnosticSnippet::Range>(*m_sourceRange);
        m_startOffset = range.start;
        m_endOffset = range.end;
    }
    else
    {
        auto const range = std::get<LineColumnRange>(*m_sourceRange);
        if (auto maybeOffset = m_decoder->offset(range.start))
        {
            m_startOffset = *maybeOffset;
        }
        else
        {
            throw std::runtime_error{"invalid start line"};
        }
        if (auto maybeOffset = m_decoder->offset(range.end))
        {
            m_endOffset = *maybeOffset;
        }
        else
        {
            throw std::runtime_error{"invalid end line"};
        }
    }
}

void StandaloneSnippetBuilder::findCursorOffset()
{
    size_t absoluteOffset;
    if (std::holds_alternative<size_t>(*m_cursor))
    {
        absoluteOffset = std::get<size_t>(*m_cursor);
    }
    else
    {
        auto const position = std::get<LineAndColumn>(*m_cursor);
        if (auto maybeOffset = m_decoder->offset(position))
        {
            absoluteOffset = *maybeOffset;
        }
        else
        {
            throw std::runtime_error{"invalid cursor offset"};
        }
    }

    if (absoluteOffset < m_startOffset || absoluteOffset >= m_endOffset)
    {
        throw std::runtime_error{"cursor not within source range"};
    }
    m_cursorOffset = absoluteOffset - m_startOffset;
}

void StandaloneSnippetBuilder::extractSnippetCode()
{
    m_snippetCode = std::string{reinterpret_cast<const char*>(m_source + m_startOffset),
                                m_endOffset - m_startOffset};
}

} // namespace diagnostics