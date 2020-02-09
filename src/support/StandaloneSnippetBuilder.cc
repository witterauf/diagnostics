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
        line.mark = markLine(number);
        line.cursor = putLineCursor(number);
        snippet->append(line);
    }

    return std::move(snippet);
}

auto StandaloneSnippetBuilder::putLineCursor(size_t number) -> std::optional<size_t>
{
    if (number == m_realCursor.line)
    {
        return m_realCursor.column;
    }
    else
    {
        return {};
    }
}

auto StandaloneSnippetBuilder::markLine(size_t number) -> std::optional<DiagnosticSnippet::Range>
{
    if (m_realMarkedRange && m_realMarkedRange->containsLine(number))
    {
        DiagnosticSnippet::Range range;
        if (number == m_realMarkedRange->start.line)
        {
            range.start = m_realMarkedRange->start.column;
        }
        else
        {
            range.start = 1;
        }
        if (number == m_realMarkedRange->end.line)
        {
            range.end = m_realMarkedRange->end.column;
        }
        else
        {
            if (auto maybeColumn = m_decoder->lastColumn(number))
            {
                range.end = *maybeColumn;
            }
            else
            {
                throw std::runtime_error{"invalid line"};
            }
        }
        return range;
    }
    return {};
}

static auto tabsToSpaces(const std::string& line, size_t width) -> std::string
{
    std::string withSpaces;
    auto const spaces = std::string(width, ' ');
    for (auto const c : line)
    {
        if (c == '\t')
        {
            withSpaces += spaces;
        }
        else
        {
            withSpaces += c;
        }
    }
    return withSpaces;
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
        return tabsToSpaces(
            std::string{reinterpret_cast<const char*>(m_source + startOffset), length},
            m_decoder->indentationWidth());
    }
    else
    {
        throw std::runtime_error{"invalid line"};
    }
}

void StandaloneSnippetBuilder::translateSourceRange()
{
    if (std::holds_alternative<DiagnosticSnippet::Range>(*m_sourceRange))
    {
        auto const range = std::get<DiagnosticSnippet::Range>(*m_sourceRange);
        auto const start = m_decoder->decode(range.start);
        auto const end = m_decoder->decode(range.end);
        m_realSourceRange = {start, end};
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
            m_realMarkedRange = {start, end};
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