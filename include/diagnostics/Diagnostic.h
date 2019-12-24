#pragma once

#include "DiagnosticLocation.h"
#include "DiagnosticSnippet.h"
#include <cstddef>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

namespace diagnostics {

enum class DiagnosticLevel
{
    Note,
    Warning,
    Error,
    Fatal
};

class Diagnostic
{
public:
    Diagnostic() = default;
    explicit Diagnostic(DiagnosticLevel level, const std::string& message);

    auto level() const -> DiagnosticLevel;
    void setLevel(DiagnosticLevel level);
    bool hasTag() const;
    void setTag(const std::string& tag);
    auto tag() const -> const std::string&;
    bool hasLocation() const;
    void setLocation(const DiagnosticLocation& info);
    auto location() const -> const DiagnosticLocation&;
    void setMessage(const std::string& message);
    auto message() const -> const std::string&;
    bool hasSnippet() const;
    void setSnippet(std::shared_ptr<DiagnosticSnippet>&& snippet);
    auto snippet() const -> const DiagnosticSnippet&;
    auto shareSnippet() const -> std::shared_ptr<DiagnosticSnippet>;
    bool hasDetails() const;
    void setDetails(const std::string& details);
    auto details() const -> const std::string&;

private:
    DiagnosticLevel m_level = DiagnosticLevel::Note;
    std::optional<std::string> m_tag;
    std::optional<DiagnosticLocation> m_location;
    std::string m_message;
    std::string m_details;
    std::shared_ptr<DiagnosticSnippet> m_snippet;
};

} // namespace diagnostics
