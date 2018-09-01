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

    // building options
    auto onlyFullLines(bool isActive) -> StandaloneSnippetBuilder&;

    auto build() -> std::shared_ptr<StandaloneSnippet>;

private:
    void reset();
    void findSourceOffsets();
    void findSourceOffsetFullLines();
    void findSourceOffsetsPartialLines();
    void findCursorOffset();
    void extractSnippetCode();

    const uint8_t* m_source = nullptr;
    size_t m_size = 0;
    LineColumnDecoder* m_decoder = nullptr;

    struct LineColumnRange
    {
        LineAndColumn start, end;
    };

    std::optional<std::variant<DiagnosticSnippet::Range, LineColumnRange>> m_sourceRange;
    std::optional<std::variant<DiagnosticSnippet::Range, LineColumnRange>> m_markedRange;
    std::optional<std::variant<size_t, LineAndColumn>> m_cursor;
    bool m_onlyFullLines = true;

    size_t m_startOffset, m_endOffset;
    size_t m_cursorOffset;
    std::string m_snippetCode;
};

} // namespace diagnostics