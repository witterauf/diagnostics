#include "support/LineColumnDecoder.h"
#include "Contracts.h"
#include "implementation/Utf8LineColumnDecoder.h"

namespace diagnostics {

auto LineColumnDecoder::make(const uint8_t* source, size_t size, Encoding encoding)
    -> std::unique_ptr<LineColumnDecoder>
{
    switch (encoding)
    {
    case Encoding::Ascii:
    case Encoding::Utf8: return std::make_unique<Utf8LineColumnDecoder>(source, size);
    }
    return nullptr;
}

LineColumnDecoder::LineColumnDecoder(const uint8_t* source, size_t size)
    : m_source{ source }, m_size{ size } {}

auto LineColumnDecoder::decode(size_t offset) const -> LineAndColumn
{
    Expects(m_source);
    Expects(offset < m_size);

    LineAndColumn position;
    if (m_offsetCache.empty())
    {
        position = doDecoding(offset, Hint{ 0, m_basePosition });
    }
    else
    {
        auto iter = m_offsetCache.lower_bound(offset);
        if (iter == m_offsetCache.cbegin())
        {
            position = doDecoding(offset, Hint{ 0, m_basePosition });
        }
        else
        {
            --iter;
            position = doDecoding(offset, Hint{ iter->first, iter->second });
        }
    }

    m_offsetCache.insert(std::make_pair(offset, position));
    m_positionCache.insert(std::make_pair(position, offset));
    return position;
}

auto LineColumnDecoder::offset(const LineAndColumn& position) const -> std::optional<size_t>
{
    Expects(m_source);
    Expects(position.isValid());

    std::optional<OffsetAndPosition> result;
    if (m_positionCache.empty())
    {
        result = doDecoding(position, Hint{ 0, m_basePosition });
    }
    else
    {
        auto iter = m_positionCache.lower_bound(position);
        if (iter == m_positionCache.cbegin())
        {
            result = doDecoding(position, Hint{ 0, m_basePosition });
        }
        else
        {
            --iter;
            result = doDecoding(position, Hint{ iter->second, iter->first });
        }
    }

    if (result)
    {
        m_offsetCache.insert(std::make_pair(result->offset, result->position));
        m_positionCache.insert(std::make_pair(result->position, result->offset));
        return result->offset;
    }
    else
    {
        return{};
    }
}

auto LineColumnDecoder::lineOffset(size_t line) const -> std::optional<size_t>
{
    return offset(LineAndColumn{ line, 1 });
}

auto LineColumnDecoder::lastColumn(size_t line) const -> std::optional<size_t>
{
    if (auto maybeOffset = endOfLineOffset(line))
    {
        auto position = decode(*maybeOffset);
        return position.column;
    }
    else
    {
        return{};
    }
}

auto LineColumnDecoder::endOfLineOffset(size_t line) const -> std::optional<size_t>
{
    return offset(LineAndColumn{line, LineAndColumn::EndOfLine});
}

auto LineColumnDecoder::size() const -> size_t
{
    return m_size;
}

auto LineColumnDecoder::buffer() const -> const uint8_t*
{
    return m_source;
}

void LineColumnDecoder::addHint(const Hint& hint)
{
    m_offsetCache.insert(std::make_pair(hint.offset, hint.position));
    m_positionCache.insert(std::make_pair(hint.position, hint.offset));
}

void LineColumnDecoder::addHints(std::initializer_list<Hint> hints)
{
    for (auto hint : hints)
    {
        addHint(hint);
    }
}

auto LineColumnDecoder::hints() const -> std::vector<Hint>
{
    std::vector<Hint> result;
    for (auto hintPair : m_offsetCache)
    {
        result.push_back(Hint{ hintPair.first, hintPair.second });
    }
    return result;
}

}