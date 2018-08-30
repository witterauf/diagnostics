#pragma once

#include "diagnostics/LineColumnDecoder.h"

namespace diagnostics {

class Utf8LineColumnDecoder : public LineColumnDecoder
{
public:
    explicit Utf8LineColumnDecoder(const uint8_t* source, size_t size);

protected:
    auto doDecoding(size_t offset, size_t hint, const LineAndColumn& start) const -> LineAndColumn override;
};

}