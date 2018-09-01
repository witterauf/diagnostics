#include "Utf8LineColumnDecoder.h"

namespace diagnostics {

Utf8LineColumnDecoder::Utf8LineColumnDecoder(const uint8_t* source, size_t size)
    : LineColumnDecoder{ source, size } {}

auto Utf8LineColumnDecoder::doDecoding(size_t offset, const Hint& hint) const -> LineAndColumn
{
    m_offset = hint.offset;
    m_position = hint.position;
    while (m_offset < size())
    {
        if (m_offset == offset)
        {
            return m_position;
        }
        advance();
    }
    throw std::logic_error{ "cannot reach" };
}

auto Utf8LineColumnDecoder::doDecoding(const LineAndColumn& position, const Hint& hint) const -> std::optional<size_t>
{
    size_t currentOffset = hint.offset;
    LineAndColumn currentPosition = hint.position;
    while (currentOffset < size())
    {
        if (currentPosition == position)
        {
            return currentOffset;
        }
        if (currentPosition.line > position.line)
        {
            return{};
        }
        advance();
    }
    return{};
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
    else
    {
        m_position = m_position.nextColumn();
    }
    ++m_offset;
}

}