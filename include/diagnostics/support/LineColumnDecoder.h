#pragma once

#include "diagnostics/LineAndColumn.h"
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <map>
#include <memory>
#include <optional>
#include <vector>

namespace diagnostics {

class LineColumnDecoder
{
public:
    enum class Encoding
    {
        Ascii,
        Utf8
    };

    static auto make(const uint8_t* source, size_t size, Encoding encoding = Encoding::Ascii) -> std::unique_ptr<LineColumnDecoder>;

    virtual ~LineColumnDecoder() = default;

    explicit LineColumnDecoder(const uint8_t* source, size_t size);

    auto decode(size_t offset) const -> LineAndColumn;
    auto offset(const LineAndColumn& position) const -> std::optional<size_t>;
    auto lineOffset(size_t line) const -> std::optional<size_t>;

    struct Hint
    {
        size_t offset;
        LineAndColumn position;
    };

    void addHint(const Hint& hint);
    void addHints(std::initializer_list<Hint> hints);
    auto hints() const -> std::vector<Hint>;

protected:
    auto size() const -> size_t;
    auto buffer() const -> const uint8_t*;

private:
    virtual auto doDecoding(size_t offset, const Hint& hint) const -> LineAndColumn = 0;
    virtual auto doDecoding(const LineAndColumn& position, const Hint& hint) const -> std::optional<size_t> = 0;

    const uint8_t* m_source = nullptr;
    size_t m_size = 0;
    mutable std::map<size_t, LineAndColumn> m_offsetCache;
    mutable std::map<LineAndColumn, size_t> m_positionCache;
    LineAndColumn m_basePosition{ 1, 1 };
};

}