#include "InMemoryReporter.h"
#include "Contracts.h"

namespace diagnostics {

void InMemoryReporter::setComment(const std::string& comment)
{
    m_comment = comment;
}

void InMemoryReporter::unsetComment()
{
    m_comment = {};
}

auto InMemoryReporter::makeLocation(size_t offset) -> DiagnosticLocation
{
    auto const position = decoder().decode(offset);
    DiagnosticLocation location{ position };
    if (m_comment.has_value())
    {
        location.setComment(*m_comment);
    }
    return location;
}

}