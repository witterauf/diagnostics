#pragma once

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <map>
#include <memory>
#include <optional>
#include <vector>

namespace diagnostics {

struct LineAndColumn
{
    size_t line = 1;
    size_t column = 1;

    bool isValid() const
    {
        return line >= 1 && column >= 1;
    }

    auto operator<(const LineAndColumn& rhs) const
    {
        return line < rhs.line || (line == rhs.line && column < rhs.column);
    }

    auto operator==(const LineAndColumn& rhs) const
    {
        return line == rhs.line && column == rhs.column;
    }

    auto nextLine() const -> LineAndColumn
    {
        return { line + 1, 1 };
    }

    auto nextColumn() const -> LineAndColumn
    {
        return { line, column + 1 };
    }
};

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