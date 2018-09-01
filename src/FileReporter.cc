#include "FileReporter.h"
#include "Contracts.h"

namespace diagnostics {

FileReporter::FileReporter(const std::filesystem::path& path)
    : m_path{ path } {}

void FileReporter::setFileName(const std::filesystem::path& path)
{
    m_path = path;
}

void FileReporter::setComment(const std::string& comment)
{
    m_comment = comment;
}

void FileReporter::unsetComment()
{
    m_comment = {};
}

auto FileReporter::makeLocation(size_t offset) -> DiagnosticLocation
{
    Expects(!m_path.empty());
    auto const position = decoder().decode(offset);
    DiagnosticLocation location{ m_path, position };
    if (m_comment.has_value())
    {
        location.setComment(*m_comment);
    }
    return location;
}

}