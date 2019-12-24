#pragma once

#include "Diagnostic.h"
#include <string>
#include <type_traits>

namespace diagnostics {

class DiagnosticsReporter;

class DiagnosticsBuilder
{
public:
    explicit DiagnosticsBuilder(DiagnosticsReporter* reporter, const std::string& message)
        : m_message{message}
        , m_reporter{reporter}
    {
    }
    explicit DiagnosticsBuilder(DiagnosticsReporter* reporter, const DiagnosticLocation& location,
                                const std::string& message);

    ~DiagnosticsBuilder();
    DiagnosticsBuilder(const DiagnosticsBuilder& other) = delete;
    auto operator=(const DiagnosticsBuilder& rhs) -> DiagnosticsBuilder& = delete;

    auto level(DiagnosticLevel level) -> DiagnosticsBuilder&;
    auto at(const DiagnosticLocation& location) -> DiagnosticsBuilder&;
    auto tag(const std::string& tag) -> DiagnosticsBuilder&;
    auto snippet(std::shared_ptr<DiagnosticSnippet>&& snippet) -> DiagnosticsBuilder&;
    auto details(const std::string& details) -> DiagnosticsBuilder&;
    auto release() -> DiagnosticsBuilder;

    template <class T> auto substitute(const T& value) -> DiagnosticsBuilder&
    {
        if constexpr (std::is_arithmetic<T>::value)
        {
            substituteString(std::to_string(value));
        }
        else
        {
            substituteString(value);
        }
        return *this;
    }

private:
    DiagnosticsBuilder() = default;
    DiagnosticsBuilder(DiagnosticsBuilder&& other);

    void applySubstitutions();
    void substituteString(const std::string& value);

    std::string m_message;
    Diagnostic m_diagnostic;
    DiagnosticsReporter* m_reporter;
    std::vector<std::string> m_substitutions;
};

} // namespace diagnostics
