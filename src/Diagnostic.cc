#include "Diagnostic.h"
#include "Contracts.h"

namespace diagnostics {

DiagnosticLocation::DiagnosticLocation(const std::string& filename, size_t offset)
    : m_filename{ filename }, m_offset{ offset } {}

DiagnosticLocation::DiagnosticLocation(const uint8_t* buffer, size_t offset)
    : m_buffer{ buffer }, m_offset{ offset } {}

auto DiagnosticLocation::kind() const -> Kind
{
    if (isFileLocation())
    {
        return Kind::File;
    }
    else if (isMemoryLocation())
    {
        return Kind::Memory;
    }
    else
    {
        return Kind::None;
    }
}

bool DiagnosticLocation::isValid() const
{
    return !(m_filename.empty() && !m_buffer);
}

bool DiagnosticLocation::isFileLocation() const
{
    return !m_filename.empty();
}

bool DiagnosticLocation::isMemoryLocation() const
{
    return m_buffer != nullptr;
}

auto DiagnosticLocation::filename() const -> const std::string&
{
    Expects(isFileLocation());
    return m_filename;
}

auto DiagnosticLocation::buffer() const -> const uint8_t*
{
    Expects(isMemoryLocation());
    return m_buffer;
}

auto DiagnosticLocation::offset() const->size_t
{
    Expects(isValid());
    return m_offset;
}

Diagnostic::Diagnostic(
    DiagnosticLevel level,
    const std::string& message,
    const DiagnosticLocation& location)
    : m_level{ level }, m_message{ message }, m_location{ location } {}

bool Diagnostic::hasTag() const
{
    return m_tag.has_value();
}

void Diagnostic::setTag(const std::string& tag)
{
    m_tag = tag;
}

auto Diagnostic::tag() const -> const std::string&
{
    Expects(hasTag());
    return *m_tag;
}

auto Diagnostic::level() const -> DiagnosticLevel
{
    return m_level;
}

void Diagnostic::setLevel(DiagnosticLevel level)
{
    m_level = level;
}

bool Diagnostic::isFileSpecific() const
{
    return m_location.isValid();
}

void Diagnostic::setLocation(const DiagnosticLocation& location)
{
    m_location = location;
}

auto Diagnostic::location() const -> const DiagnosticLocation&
{
    return m_location;
}

void Diagnostic::setMessage(const std::string& message)
{
    m_message = message;
}

auto Diagnostic::message() const -> const std::string&
{
    return m_message;
}

}