#pragma once

#include "SourceReporter.h"
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>

namespace diagnostics {

class FileReporter : public SourceReporter
{
public:
    FileReporter() = default;
    explicit FileReporter(const std::filesystem::path& path);

    auto loadSource(const std::filesystem::path& path,
                    LineColumnDecoder::Encoding encoding = LineColumnDecoder::Encoding::Utf8)
        -> std::pair<std::unique_ptr<uint8_t[]>, size_t>;
    void setFileName(const std::filesystem::path& path);
    void setComment(const std::string& comment);
    void unsetComment();

    auto makeLocation(size_t offset) -> DiagnosticLocation override;

private:
    std::filesystem::path m_path;
    std::optional<std::string> m_comment;
};

} // namespace diagnostics