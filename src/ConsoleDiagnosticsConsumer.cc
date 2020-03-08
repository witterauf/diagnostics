#include "ConsoleDiagnosticsConsumer.h"
#include <iomanip>
#include <iostream>

namespace diagnostics {

auto bars(const std::string& str) -> std::string
{
    return std::string(str.length(), '=');
}

ConsoleDiagnosticsConsumer::ConsoleDiagnosticsConsumer(std::ostream* out)
    : m_out{out}
{
}

void ConsoleDiagnosticsConsumer::startPhase(const std::string& name)
{
    *m_out << bars(name) << "\n";
    *m_out << name << "\n";
    *m_out << bars(name) << "\n\n";
}

void ConsoleDiagnosticsConsumer::consume(const Diagnostic& diagnostic)
{
    m_diagnostic = &diagnostic;
    printDiagnostic();
    printSnippet();
    printDetails();
}

void ConsoleDiagnosticsConsumer::endPhase() {}

void ConsoleDiagnosticsConsumer::printDetails()
{
    if (m_diagnostic->hasDetails())
    {
        *m_out << m_diagnostic->details() << "\n";
    }
}

void ConsoleDiagnosticsConsumer::printMessage()
{
    switch (diagnostic().level())
    {
    case DiagnosticLevel::Note: *m_out << "note"; break;
    case DiagnosticLevel::Warning: *m_out << "warning"; break;
    case DiagnosticLevel::Error: *m_out << "error"; break;
    case DiagnosticLevel::Fatal: *m_out << "fatal error"; break;
    default: break;
    }

    if (m_showTag && diagnostic().hasTag())
    {
        *m_out << " [" << diagnostic().tag() << "]";
    }

    *m_out << ": " << diagnostic().message() << "\n";
}

void ConsoleDiagnosticsConsumer::printDiagnostic()
{
    printLocation();
    printMessage();
}

void ConsoleDiagnosticsConsumer::printLocation()
{
    if (diagnostic().hasLocation())
    {
        auto const& location = diagnostic().location();
        if (location.isInFile())
        {
            *m_out << location.path().string();
        }
        if (location.hasComment())
        {
            *m_out << "[" << location.comment() << "]";
        }
        if (location.isInFile() || location.hasComment())
        {
            *m_out << ":";
        }
        *m_out << location.line() << ":" << location.column() << ": ";
    }
}

void ConsoleDiagnosticsConsumer::printSnippet()
{
    if (!diagnostic().hasSnippet())
    {
        return;
    }
    calculateSnippetLayout();

    auto const& snippet = diagnostic().snippet();
    for (auto number = 0U; number < snippet.lineCount(); ++number)
    {
        auto const line = snippet.line(number);
        printIndentation();
        printLineNumber(line.number);
        *m_out << line.text << "\n";
        printMarking(number);
    }
}

static void repeat(std::ostream& out, char c, size_t count)
{
    for (auto i = 0U; i < count; ++i) { out << c; }
}

void ConsoleDiagnosticsConsumer::printIndentation()
{
    if (m_includeLineNumber)
    {
        repeat(*m_out, ' ', m_lineNumberIndentation);
    }
    else
    {
        repeat(*m_out, ' ', m_snippetIndentation);
    }
}

void ConsoleDiagnosticsConsumer::printLineNumber(size_t number)
{
    if (m_includeLineNumber)
    {
        *m_out << std::setw(m_lineNumberWidth) << number << ":";
        repeat(*m_out, ' ', m_snippetIndentation);
    }
}

void ConsoleDiagnosticsConsumer::printMarking(size_t number)
{
    auto const line = diagnostic().snippet().line(number);

    if (line.mark && line.cursor)
    {
        auto const leftOfCursor = *line.cursor - line.mark->start;
        auto const rightOfCursor = line.mark->end - *line.cursor - 1;
        repeat(*m_out, ' ', snippetCodeLeft() + line.mark->start - 1);
        repeat(*m_out, '~', leftOfCursor);
        *m_out << "^";
        repeat(*m_out, '~', rightOfCursor);
        *m_out << "\n";
    }
    else if (line.cursor)
    {
        repeat(*m_out, ' ', snippetCodeLeft() + *line.cursor - 1);
        *m_out << "^\n";
    }
    else if (line.mark)
    {
    }
}

void ConsoleDiagnosticsConsumer::calculateSnippetLayout()
{
    if (m_includeLineNumber)
    {
        auto const& snippet = diagnostic().snippet();
        auto const firstLine = snippet.sourceRange().start.line;
        auto const lastLine = snippet.sourceRange().end.line;
        m_lineNumberWidth =
            std::max(std::to_string(firstLine).length(), std::to_string(lastLine).length());
    }
    else
    {
        m_lineNumberWidth = 0;
    }
}

auto ConsoleDiagnosticsConsumer::snippetCodeLeft() const -> size_t
{
    return m_snippetIndentation + m_lineNumberWidth + m_lineNumberIndentation + 1;
}

auto ConsoleDiagnosticsConsumer::diagnostic() const -> const Diagnostic&
{
    return *m_diagnostic;
}

void ConsoleDiagnosticsConsumer::showTag()
{
    m_showTag = true;
}

void ConsoleDiagnosticsConsumer::hideTag()
{
    m_showTag = false;
}

} // namespace diagnostics