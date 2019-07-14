#pragma once

#include "Diagnostic.h"

namespace diagnostics {

class DiagnosticsReporter;

class DiagnosticsBuilder
{
public:
    explicit DiagnosticsBuilder(DiagnosticsReporter* reporter, const std::string& message)
        : m_message{ message }, m_reporter{ reporter } {}
    explicit DiagnosticsBuilder(DiagnosticsReporter* reporter, const DiagnosticLocation& location,
                                const std::string& message);

    ~DiagnosticsBuilder();

    auto level(DiagnosticLevel level) -> DiagnosticsBuilder&;
    auto at(const DiagnosticLocation& location) -> DiagnosticsBuilder&;
    auto tag(const std::string& tag) -> DiagnosticsBuilder&;
    auto snippet(std::shared_ptr<DiagnosticSnippet>&& snippet) -> DiagnosticsBuilder&;
    auto substitute(const std::string& value) -> DiagnosticsBuilder&;
    auto substitute(int value) -> DiagnosticsBuilder&;
    auto substitute(unsigned int value) -> DiagnosticsBuilder&;
    auto substitute(unsigned long long value) -> DiagnosticsBuilder&;

private:
    void applySubstitutions();

    std::string m_message;
    Diagnostic m_diagnostic;
    DiagnosticsReporter* m_reporter;
    std::vector<std::string> m_substitutions;
};

}