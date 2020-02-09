#include "Utf8LineColumnDecoder.h"
#include "Contracts.h"

namespace diagnostics {

Utf8LineColumnDecoder::Utf8LineColumnDecoder(const uint8_t* source, size_t size)
    : LineColumnDecoder{source, size}
{
}

auto Utf8LineColumnDecoder::doDecoding(size_t offset, const Hint& hint) const -> LineAndColumn
{
    m_offset = hint.offset;
    m_position = hint.position;
    while (m_offset < size())
    {
        if (m_offset >= offset)
        {
            return m_position;
        }
        advance();
    }
    throw std::logic_error{"cannot reach"};
}

auto Utf8LineColumnDecoder::doDecoding(const LineAndColumn& position, const Hint& hint) const
    -> std::optional<OffsetAndPosition>
{
    m_offset = hint.offset;
    m_position = hint.position;
    LineAndColumn oldPosition = {0, 0};
    size_t oldOffset = 0;
    while (m_offset < size())
    {
        if (m_position == position)
        {
            return OffsetAndPosition{m_offset, m_position};
        }
        if (m_position.line > position.line)
        {
            break;
        }

        oldOffset = m_offset;
        oldPosition = m_position;
        advance();
    }
    if (position.isEndOfLine())
    {
        if (oldPosition.isValid() && oldPosition.line == position.line)
        {
            return OffsetAndPosition{oldOffset, oldPosition};
        }
    }
    return {};
}

void Utf8LineColumnDecoder::advance() const
{
    if (buffer()[m_offset] == '\n')
    {
        m_position = m_position.nextLine();
    }
    else if (buffer()[m_offset] == '\r')
    {
        m_position = m_position.nextLine();

        if (m_offset + 1 < size() && buffer()[m_offset + 1] == '\n')
        {
            ++m_offset;
        }
    }
    else if (buffer()[m_offset] == '\t')
    {
        m_position = m_position.skipColumns(indentationWidth());
    }
    else
    {
        m_position = m_position.nextColumn();
    }
    ++m_offset;
}

} // namespace diagnostics