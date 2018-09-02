#include "SourceReporter.h"
#include "Contracts.h"

namespace diagnostics {

void SourceReporter::setSource(const uint8_t* source, size_t size, LineColumnDecoder::Encoding encoding)
{
    m_decoder = LineColumnDecoder::make(source, size, encoding);
    m_builder.setSource(source, size);
    m_builder.setDecoder(m_decoder.get());
}

auto SourceReporter::report(const std::string& message, size_t offset) -> DiagnosticsBuilder
{
    auto location = makeLocation(offset);
    return DiagnosticsBuilder{ this, location, message };
}

auto SourceReporter::snippetBuilder() -> StandaloneSnippetBuilder&
{
    Expects(m_decoder);
    return m_builder;
}

auto SourceReporter::decoder() -> LineColumnDecoder&
{
    Expects(m_decoder);
    return *m_decoder;
}

}