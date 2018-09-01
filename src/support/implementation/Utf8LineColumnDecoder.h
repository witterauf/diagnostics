#pragma once

#include "diagnostics/support/LineColumnDecoder.h"

namespace diagnostics {

class Utf8LineColumnDecoder : public LineColumnDecoder
{
public:
    explicit Utf8LineColumnDecoder(const uint8_t* source, size_t size);

protected:
    auto doDecoding(size_t offset, const Hint& hint) const -> LineAndColumn override;
    auto doDecoding(const LineAndColumn& position, const Hint& hint) const -> std::optional<OffsetAndPosition> override;

private:
    void advance() const;

    mutable size_t m_offset;
    mutable LineAndColumn m_position;
};

}