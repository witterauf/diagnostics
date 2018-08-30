#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>

namespace diagnostics {

class DiagnosticLocation
{
public:
    explicit DiagnosticLocation(const std::filesystem::path& path, size_t line, size_t column);
    explicit DiagnosticLocation(size_t line, size_t column);

    auto line() const -> size_t;
    auto column() const -> size_t;

    bool isInFile() const;
    bool isInMemory() const;
    auto path() const -> const std::filesystem::path&;

    bool hasComment() const;
    void setComment(const std::string& comment);
    auto comment() const -> const std::string&;

private:
    std::optional<std::filesystem::path> m_path;
    std::optional<std::string> m_comment;
    size_t m_line = 0, m_column = 0;
};

}