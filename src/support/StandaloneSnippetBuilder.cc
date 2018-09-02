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
    m_sourceRange = LineAndColumnRange{start, end};
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
    m_markedRange = LineAndColumnRange{start, end};
    return *this;
}

auto StandaloneSnippetBuilder::build() -> std::shared_ptr<StandaloneSnippet>
{
    Expects(m_sourceRange.has_value());
    Expects(m_cursor.has_value());

    translateSourceRange();
    translateMarkedRange();
    findCursorOffset();
    auto snippet = makeSnippet();

    reset();
    return std::move(snippet);
}

auto StandaloneSnippetBuilder::makeSnippet() -> std::shared_ptr<StandaloneSnippet>
{
    auto snippet = std::make_shared<StandaloneSnippet>(m_realSourceRange, m_realCursor);
    
    for (auto number = m_realSourceRange.start.line; number <= m_realSourceRange.end.line; ++number)
    {
        DiagnosticSnippet::Line line;
        line.number = number;
        line.text = extractLine(number);

        if (number == m_realCursor.line)
        {
            line.cursor = m_realCursor.column;
        }
        if (m_realMarkedRange)
        {
            DiagnosticSnippet::Range range;
            if (number == m_realMarkedRange->start.line)
            {
                if (auto maybeColumn = m_decoder->lastColumn(number))
                {
                    range.start = m_realMarkedRange->start.column;
                    range.end = *maybeColumn;
                }
                else
                {
                    throw std::runtime_error{ "invalid line" };
                }
            }
            else if (number > m_realMarkedRange->start.line && number < m_realMarkedRange->end.line)
            {
                if (auto maybeColumn = m_decoder->lastColumn(number))
                {
                    range.start = 1;
                    range.end = *maybeColumn;
                }
                else
                {
                    throw std::runtime_error{ "invalid line" };
                }
            }
            else if (number == m_realMarkedRange->end.line)
            {
                range.start = 1;
                range.end = m_realMarkedRange->end.column;
            }
            line.mark = range;
        }

        snippet->append(line);
    }

    return std::move(snippet);
}

auto StandaloneSnippetBuilder::extractLine(size_t line) -> std::string
{
    auto maybeStartOffset = m_decoder->lineOffset(line);
    auto maybeEndOffset = m_decoder->endOfLineOffset(line);
    if (maybeStartOffset && maybeEndOffset)
    {
        auto const startOffset = *maybeStartOffset;
        auto const endOffset = *maybeEndOffset;
        auto const length = endOffset - startOffset;
        return std::string{ reinterpret_cast<const char*>(m_source + startOffset), length };
    }
    else
    {
        throw std::runtime_error{ "invalid line" };
    }
}

void StandaloneSnippetBuilder::translateSourceRange()
{
    if (std::holds_alternative<DiagnosticSnippet::Range>(*m_sourceRange))
    {
        auto const range = std::get<DiagnosticSnippet::Range>(*m_sourceRange);
        auto const start = m_decoder->decode(range.start);
        auto const end = m_decoder->decode(range.end);
        m_realSourceRange = { start, end };
    }
    else
    {
        m_realSourceRange = std::get<LineAndColumnRange>(*m_sourceRange);
    }
}

void StandaloneSnippetBuilder::translateMarkedRange()
{
    if (m_markedRange)
    {
        if (std::holds_alternative<DiagnosticSnippet::Range>(*m_markedRange))
        {
            auto const range = std::get<DiagnosticSnippet::Range>(*m_markedRange);
            auto const start = m_decoder->decode(range.start);
            auto const end = m_decoder->decode(range.end);
            m_realMarkedRange = { start, end };
        }
        else
        {
            m_realMarkedRange = std::get<LineAndColumnRange>(*m_markedRange);
        }
    }
}

void StandaloneSnippetBuilder::findCursorOffset()
{
    if (std::holds_alternative<size_t>(*m_cursor))
    {
        m_realCursor = m_decoder->decode(std::get<size_t>(*m_cursor));
    }
    else
    {
        m_realCursor = std::get<LineAndColumn>(*m_cursor);
    }
}

} // namespace diagnostics