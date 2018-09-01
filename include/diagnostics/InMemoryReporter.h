#pragma once

#include "SourceReporter.h"
#include <filesystem>
#include <optional>

namespace diagnostics {

class InMemoryReporter : public SourceReporter
{
public:
    void setComment(const std::string& comment);
    void unsetComment();

    auto makeLocation(size_t offset) -> DiagnosticLocation override;

private:
    std::optional<std::string> m_comment;
};

}