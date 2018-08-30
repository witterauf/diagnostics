#include "LineColumnDecoder.h"
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
    if (m_cache.empty())
    {
        position = doDecoding(offset, 0, m_basePosition);
    }
    else
    {
        auto iter = m_cache.lower_bound(offset);
        if (iter == m_cache.cbegin())
        {
            position = doDecoding(offset, 0, m_basePosition);
        }
        else
        {
            --iter;
            position = doDecoding(offset, iter->first, iter->second);
        }
    }

    m_cache.insert(std::make_pair(offset, position));
    return position;
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
    m_cache.insert(std::make_pair(hint.offset, hint.position));
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
    for (auto hintPair : m_cache)
    {
        result.push_back(Hint{ hintPair.first, hintPair.second });
    }
    return result;
}

}