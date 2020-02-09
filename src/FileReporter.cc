#include "FileReporter.h"
#include "Contracts.h"
#include <fstream>

namespace diagnostics {

FileReporter::FileReporter(const std::filesystem::path& path)
    : m_path{path}
{
}

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
    DiagnosticLocation location{m_path, position};
    if (m_comment.has_value())
    {
        location.setComment(*m_comment);
    }
    return location;
}

auto FileReporter::loadSource(const std::filesystem::path& path,
                              LineColumnDecoder::Encoding encoding)
    -> std::pair<std::unique_ptr<uint8_t[]>, size_t>
{
    std::ifstream input{path, std::ios::binary};
    if (input.good())
    {
        auto const size = std::filesystem::file_size(path) + 1;
        auto buffer = std::make_unique<uint8_t[]>(size);
        input.read((char*)buffer.get(), size - 1);
        buffer[size - 1] = 0;
        setSource(buffer.get(), size, encoding);
        setFileName(path);
        return std::make_pair(std::move(buffer), size);
    }
    else
    {
        return std::make_pair(nullptr, 0);
    }
}

} // namespace diagnostics