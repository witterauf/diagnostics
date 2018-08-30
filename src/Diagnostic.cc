#include "Diagnostic.h"
#include "Contracts.h"

namespace diagnostics {

Diagnostic::Diagnostic(DiagnosticLevel level, const std::string& message)
    : m_level{ level }, m_message{ message } {}

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

bool Diagnostic::hasLocation() const
{
    return m_location.has_value();
}

void Diagnostic::setLocation(const DiagnosticLocation& location)
{
    m_location = location;
}

auto Diagnostic::location() const -> const DiagnosticLocation&
{
    Expects(hasLocation());
    return *m_location;
}

void Diagnostic::setMessage(const std::string& message)
{
    m_message = message;
}

auto Diagnostic::message() const -> const std::string&
{
    return m_message;
}

bool Diagnostic::hasSnippet() const
{
    return m_snippet != nullptr;
}

void Diagnostic::setSnippet(std::shared_ptr<DiagnosticSnippet>&& snippet)
{
    m_snippet = std::move(snippet);
}

auto Diagnostic::snippet() const -> const DiagnosticSnippet&
{
    return *m_snippet;
}

auto Diagnostic::shareSnippet() const -> std::shared_ptr<DiagnosticSnippet>
{
    return m_snippet;
}

}