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
    if (diagnostic.isFileSpecific())
    {
        auto const& location = diagnostic.location();
        if (location.isFileLocation())
        {
            std::cout << location.filename() << ": ";
        }
    }

    switch (diagnostic.level())
    {
    case DiagnosticLevel::Note: std::cout << "Note"; break;
    case DiagnosticLevel::Warning: std::cout << "Warning"; break;
    case DiagnosticLevel::Error: std::cout << "Error"; break;
    case DiagnosticLevel::Fatal: std::cout << "Fatal error"; break;
    default: break;
    }

    if (diagnostic.hasTag())
    {
        std::cout << " [" << diagnostic.tag() << "]";
    }

    std::cout << ": " << diagnostic.message() << "\n";
}

void ConsoleDiagnosticsConsumer::endPhase()
{
}

}