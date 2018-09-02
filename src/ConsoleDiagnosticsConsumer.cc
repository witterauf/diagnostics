#include "ConsoleDiagnosticsConsumer.h"
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
}

void ConsoleDiagnosticsConsumer::endPhase()
{
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

    auto const& snippet = diagnostic().snippet();
    for (auto number = 0U; number < snippet.lineCount(); ++number)
    {
        auto const line = snippet.line(number);
        std::cout << "  " << line.number << "  " << line.text << "\n";
    }
}

}