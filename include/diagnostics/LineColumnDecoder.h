#pragma once

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
    struct LineAndColumn
    {
        size_t line = 0;
        size_t column = 0;
    };

    enum class Encoding
    {
        Ascii,
        Utf8
    };

    static auto make(const uint8_t* source, size_t size, Encoding encoding = Encoding::Ascii) -> std::unique_ptr<LineColumnDecoder>;

    virtual ~LineColumnDecoder() = default;

    explicit LineColumnDecoder(const uint8_t* source, size_t size);

    auto decode(size_t offset) const -> LineAndColumn;

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
    virtual auto doDecoding(size_t offset, size_t hint, const LineAndColumn& start) const -> LineAndColumn = 0;

    const uint8_t* m_source = nullptr;
    size_t m_size = 0;
    mutable std::map<size_t, LineAndColumn> m_cache;
    LineAndColumn m_basePosition{ 1, 1 };
};

}