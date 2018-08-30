#include "Utf8LineColumnDecoder.h"

namespace diagnostics {

Utf8LineColumnDecoder::Utf8LineColumnDecoder(const uint8_t* source, size_t size)
    : LineColumnDecoder{ source, size } {}

auto Utf8LineColumnDecoder::doDecoding(size_t offset, size_t hint, const LineAndColumn& start) const -> LineAndColumn
{
    size_t currentOffset = hint;
    LineAndColumn currentPosition = start;
    while (currentOffset < size())
    {
        if (currentOffset == offset)
        {
            return currentPosition;
        }

        if (buffer()[currentOffset] == '\n')
        {
            ++currentPosition.line; currentPosition.column = 1;
        }
        else if (buffer()[currentOffset] == '\r')
        {
            ++currentPosition.line; currentPosition.column = 1;

            if (currentOffset + 1 < size() && buffer()[currentOffset + 1] == '\n')
            {
                ++currentOffset;
            }
        }
        else
        {
            ++currentPosition.column;
        }
        ++currentOffset;
    }
    throw std::logic_error{ "cannot reach" };
}

}