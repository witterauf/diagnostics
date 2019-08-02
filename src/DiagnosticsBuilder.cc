#include "DiagnosticsBuilder.h"
#include "DiagnosticsReporter.h"

namespace diagnostics {

DiagnosticsBuilder::DiagnosticsBuilder(DiagnosticsReporter* reporter,
                                       const DiagnosticLocation& location,
                                       const std::string& message)
    : m_message{ message }, m_reporter{ reporter }
{
    m_diagnostic.setLocation(location);
}

DiagnosticsBuilder::~DiagnosticsBuilder()
{
    applySubstitutions();
    m_diagnostic.setMessage(m_message);
    m_reporter->report(m_diagnostic);
}

auto DiagnosticsBuilder::level(DiagnosticLevel level) -> DiagnosticsBuilder&
{
    m_diagnostic.setLevel(level);
    return *this;
}

auto DiagnosticsBuilder::tag(const std::string& tag) -> DiagnosticsBuilder&
{
    m_diagnostic.setTag(tag);
    return *this;
}

auto DiagnosticsBuilder::at(const DiagnosticLocation& location) -> DiagnosticsBuilder&
{
    m_diagnostic.setLocation(location);
    return *this;
}

auto DiagnosticsBuilder::snippet(std::shared_ptr<DiagnosticSnippet>&& snippet) -> DiagnosticsBuilder&
{
    m_diagnostic.setSnippet(std::move(snippet));
    return *this;
}

void DiagnosticsBuilder::applySubstitutions()
{
    std::string message = m_message;
    auto pos = message.find('%', 0);

    while (pos != std::string::npos)
    {
        const auto startPos = pos;

        size_t index = 0;
        bool atLeastOneDigit = false;
        for (++pos; pos < message.length(); ++pos)
        {
            if (message[pos] >= '0' && message[pos] <= '9')
            {
                atLeastOneDigit = true;
                index *= 10;
                index += static_cast<size_t>(message[pos] - '0');
            }
            else
            {
                break;
            }
        }
        if (!atLeastOneDigit)
        {
            throw std::runtime_error{ "invalid format string" };
        }
        if (index > m_substitutions.size())
        {
            throw std::runtime_error{ "invalid substitution index" };
        }

        message.replace(startPos, pos - startPos, m_substitutions.at(index));
        pos = message.find('%', 0);
    }

    m_message = message;
}

void DiagnosticsBuilder::substituteString(const std::string& value)
{
    m_substitutions.push_back(value);
}

}
