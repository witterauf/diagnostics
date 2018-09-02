#pragma once

#include "LineColumnDecoder.h"
#include "StandaloneSnippet.h"
#include <memory>
#include <optional>
#include <variant>

namespace diagnostics {

class StandaloneSnippetBuilder
{
public:
    void setSource(const uint8_t* source, size_t size);
    void setDecoder(LineColumnDecoder* decoder);

    auto setCursor(size_t offset) -> StandaloneSnippetBuilder&;
    auto setCursor(const LineAndColumn& position) -> StandaloneSnippetBuilder&;
    auto setSourceRange(size_t start, size_t end) -> StandaloneSnippetBuilder&;
    auto setSourceRange(const LineAndColumn& start, const LineAndColumn& end)
        -> StandaloneSnippetBuilder&;
    auto setMarkedRange(size_t start, size_t end) -> StandaloneSnippetBuilder&;
    auto setMarkedRange(const LineAndColumn& start, const LineAndColumn& end)
        -> StandaloneSnippetBuilder&;

    auto build() -> std::shared_ptr<StandaloneSnippet>;

private:
    void reset();
    void translateSourceRange();
    void translateMarkedRange();
    void findCursorOffset();
    auto extractLine(size_t line) -> std::string;
    auto makeSnippet() -> std::shared_ptr<StandaloneSnippet>;

    const uint8_t* m_source = nullptr;
    size_t m_size = 0;
    LineColumnDecoder* m_decoder = nullptr;

    std::optional<std::variant<DiagnosticSnippet::Range, LineAndColumnRange>> m_sourceRange;
    std::optional<std::variant<DiagnosticSnippet::Range, LineAndColumnRange>> m_markedRange;
    std::optional<std::variant<size_t, LineAndColumn>> m_cursor;

    std::optional<LineAndColumnRange> m_realMarkedRange;
    LineAndColumnRange m_realSourceRange;
    LineAndColumn m_realCursor;
};

} // namespace diagnostics