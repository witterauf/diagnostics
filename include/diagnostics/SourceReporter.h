#pragma once

#include "DiagnosticLocation.h"
#include "DiagnosticSnippet.h"
#include "DiagnosticsReporter.h"
#include "support/LineColumnDecoder.h"
#include "support/StandaloneSnippetBuilder.h"

namespace diagnostics {

class SourceReporter : public DiagnosticsReporter
{
public:
    void setSource(const uint8_t* source, size_t size,
                   LineColumnDecoder::Encoding encoding = LineColumnDecoder::Encoding::Utf8);
    using DiagnosticsReporter::report;
    auto report(const std::string& message, size_t offset) -> DiagnosticsBuilder;
    auto snippetBuilder() -> StandaloneSnippetBuilder&;

    virtual auto makeLocation(size_t offset) -> DiagnosticLocation = 0;

protected:
    auto decoder() -> LineColumnDecoder&;

private:
    std::unique_ptr<LineColumnDecoder> m_decoder;
    StandaloneSnippetBuilder m_builder;
};

} // namespace diagnostics