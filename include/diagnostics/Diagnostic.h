#pragma once

#include <cstddef>
#include <optional>
#include <string>

namespace diagnostics {
    
class DiagnosticLocation
{
public:
    enum class Kind
    {
        None,
        File,
        Memory
    };

    DiagnosticLocation() = default;
    explicit DiagnosticLocation(const std::string& filename, size_t offset);
    explicit DiagnosticLocation(const uint8_t* buffer, size_t offset);

    auto kind() const -> Kind;
    bool isValid() const;
    bool isFileLocation() const;
    bool isMemoryLocation() const;

    auto filename() const -> const std::string&;
    auto buffer() const -> const uint8_t*;
    auto offset() const -> size_t;

private:
    const uint8_t* m_buffer = nullptr;
    std::string m_filename;
    size_t m_offset = 0;
};

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
    explicit Diagnostic(
        DiagnosticLevel level,
        const std::string& message,
        const DiagnosticLocation& = {});

    bool hasTag() const;
    void setTag(const std::string& tag);
    auto tag() const -> const std::string&;
    auto level() const -> DiagnosticLevel;
    void setLevel(DiagnosticLevel level);
    bool isFileSpecific() const;
    void setLocation(const DiagnosticLocation& info);
    auto location() const -> const DiagnosticLocation&;
    void setMessage(const std::string& message);
    auto message() const -> const std::string&;

private:
    std::optional<std::string> m_tag;
    DiagnosticLevel m_level = DiagnosticLevel::Note;
    std::string m_message;
    DiagnosticLocation m_location;
};

}
