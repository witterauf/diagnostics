#pragma once

#include "SourceReporter.h"
#include <filesystem>
#include <optional>

namespace diagnostics {

class FileReporter : public SourceReporter
{
public:
    void setFileName(const std::filesystem::path& path);
    void setComment(const std::string& comment);
    void unsetComment();

    auto makeLocation(size_t offset) -> DiagnosticLocation override;

private:
    std::filesystem::path m_path;
    std::optional<std::string> m_comment;
};

}