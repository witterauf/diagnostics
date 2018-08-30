#pragma once

#include "Diagnostic.h"
#include <set>
#include <vector>

namespace diagnostics {

class DiagnosticsConsumer;

class DiagnosticsReporter
{
public:
    class Builder
    {
        friend class DiagnosticsReporter;

    public:
        ~Builder();
        auto level(DiagnosticLevel level) -> Builder&;
        auto at(const DiagnosticLocation& location) -> Builder&;
        auto tag(const std::string& tag) -> Builder&;
        auto substitute(const std::string& value) -> Builder&;
        auto substitute(int value) -> Builder&;
        auto substitute(unsigned int value) -> Builder&;
        auto substitute(size_t value) -> Builder&;

    private:
        explicit Builder(DiagnosticsReporter* reporter, const std::string& message)
            : m_message{ message }, m_reporter{ reporter } {}

        void applySubstitutions();

        std::string m_message;
        Diagnostic m_diagnostic;
        DiagnosticsReporter* m_reporter;
        std::vector<std::string> m_substitutions;
    };

    void startPhase(const std::string& name);
    auto report(const std::string& message) -> Builder;
    void report(const Diagnostic& diagnostic);
    void endPhase();

    void registerConsumer(DiagnosticsConsumer* consumer);
    void unregisterConsumer(DiagnosticsConsumer* consumer);

private:
    std::set<DiagnosticsConsumer*> m_consumers;
};

}
