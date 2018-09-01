#include "DiagnosticLocation.h"
#include "Contracts.h"

namespace diagnostics {

DiagnosticLocation::DiagnosticLocation(const std::filesystem::path& path, const LineAndColumn& position)
    : m_path{ path }, m_position{ position } {}

DiagnosticLocation::DiagnosticLocation(const LineAndColumn& position)
    : m_position{ position } {}

auto DiagnosticLocation::position() const -> const LineAndColumn&
{
    return m_position;
}

auto DiagnosticLocation::line() const -> size_t
{
    return m_position.line;
}

auto DiagnosticLocation::column() const -> size_t
{
    return m_position.column;
}

bool DiagnosticLocation::isInFile() const
{
    return m_path.has_value();
}

bool DiagnosticLocation::isInMemory() const
{
    return !isInFile();
}

auto DiagnosticLocation::path() const -> const std::filesystem::path&
{
    Expects(isInFile());
    return *m_path;
}

bool DiagnosticLocation::hasComment() const
{
    return m_comment.has_value();
}

void DiagnosticLocation::setComment(const std::string& comment)
{
    m_comment = comment;
}

auto DiagnosticLocation::comment() const -> const std::string&
{
    Expects(hasComment());
    return *m_comment;
}

}