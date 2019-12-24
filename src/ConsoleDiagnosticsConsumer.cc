#include "ConsoleDiagnosticsConsumer.h"
#include <iomanip>
#include <iostream>

namespace diagnostics {

auto bars(const std::string& str) -> std::string
{
    return std::string(str.length(), '=');
}

void ConsoleDiagnosticsConsumer::startPhase(const std::string& name)
{
    std::cout << bars(name) << "\n";
    std::cout << name << "\n";
    std::cout << bars(name) << "\n\n";
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
        std::cout << m_diagnostic->details() << "\n";
    }
}

void ConsoleDiagnosticsConsumer::printMessage()
{
    switch (diagnostic().level())
    {
    case DiagnosticLevel::Note: std::cout << "note"; break;
    case DiagnosticLevel::Warning: std::cout << "warning"; break;
    case DiagnosticLevel::Error: std::cout << "error"; break;
    case DiagnosticLevel::Fatal: std::cout << "fatal error"; break;
    default: break;
    }

    if (diagnostic().hasTag())
    {
        std::cout << " [" << diagnostic().tag() << "]";
    }

    std::cout << ": " << diagnostic().message() << "\n";
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
            std::cout << location.path().string();
        }
        if (location.hasComment())
        {
            std::cout << "[" << location.comment() << "]";
        }
        if (location.isInFile() || location.hasComment())
        {
            std::cout << ":";
        }
        std::cout << location.line() << ":" << location.column() << ": ";
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
        std::cout << line.text << "\n";
        printMarking(number);
    }
}

static void repeat(char c, size_t count)
{
    for (auto i = 0U; i < count; ++i) { std::cout << c; }
}

void ConsoleDiagnosticsConsumer::printIndentation()
{
    if (m_includeLineNumber)
    {
        repeat(' ', m_lineNumberIndentation);
    }
    else
    {
        repeat(' ', m_snippetIndentation);
    }
}

void ConsoleDiagnosticsConsumer::printLineNumber(size_t number)
{
    if (m_includeLineNumber)
    {
        std::cout << std::setw(m_lineNumberWidth) << number << ":";
        repeat(' ', m_snippetIndentation);
    }
}

void ConsoleDiagnosticsConsumer::printMarking(size_t number)
{
    auto const line = diagnostic().snippet().line(number);

    if (line.mark && line.cursor)
    {
        auto const leftOfCursor = *line.cursor - line.mark->start;
        auto const rightOfCursor = line.mark->end - *line.cursor - 1;
        repeat(' ', snippetCodeLeft() + line.mark->start - 1);
        repeat('~', leftOfCursor);
        std::cout << "^";
        repeat('~', rightOfCursor);
        std::cout << "\n";
    }
    else if (line.cursor)
    {
        repeat(' ', snippetCodeLeft() + *line.cursor - 1);
        std::cout << "^\n";
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

} // namespace diagnostics