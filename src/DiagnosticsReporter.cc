#include "DiagnosticsReporter.h"
#include "DiagnosticsConsumer.h"

namespace diagnostics {

DiagnosticsReporter::Builder::~Builder()
{
    applySubstitutions();
    m_diagnostic.setMessage(m_message);
    m_reporter->report(m_diagnostic);
}

auto DiagnosticsReporter::Builder::level(DiagnosticLevel level) -> Builder&
{
    m_diagnostic.setLevel(level);
    return *this;
}

auto DiagnosticsReporter::Builder::tag(const std::string& tag) -> Builder&
{
    m_diagnostic.setTag(tag);
    return *this;
}

auto DiagnosticsReporter::Builder::at(const DiagnosticLocation& location) -> Builder&
{
    m_diagnostic.setLocation(location);
    return *this;
}

void DiagnosticsReporter::Builder::applySubstitutions()
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

        message.replace (startPos, pos - startPos, m_substitutions.at(index));
        pos = message.find('%', 0);
    }

    m_message = message;
}

auto DiagnosticsReporter::Builder::substitute(const std::string& value) -> Builder&
{
    m_substitutions.push_back (value);
    return *this;
}

auto DiagnosticsReporter::Builder::substitute(int value) -> Builder&
{
    return substitute (std::to_string (value));
}

auto DiagnosticsReporter::Builder::substitute(unsigned int value) -> Builder&
{
    return substitute(std::to_string(value));
}

auto DiagnosticsReporter::Builder::substitute(size_t value) -> Builder&
{
    return substitute(std::to_string(value));
}

void DiagnosticsReporter::startPhase(const std::string& name)
{
    for (auto* consumer : m_consumers)
    {
        consumer->startPhase(name);
    }
}

void DiagnosticsReporter::endPhase()
{
    for (auto* consumer : m_consumers)
    {
        consumer->endPhase();
    }
}

auto DiagnosticsReporter::report(const std::string& message) -> Builder
{
    return Builder{ this, message };
}

void DiagnosticsReporter::report(const Diagnostic& diagnostic)
{
    for (auto* consumer : m_consumers)
    {
        consumer->consume(diagnostic);
    }
}

void DiagnosticsReporter::registerConsumer(DiagnosticsConsumer* consumer)
{
    m_consumers.insert(consumer);
}

void DiagnosticsReporter::unregisterConsumer(DiagnosticsConsumer* consumer)
{
    m_consumers.erase(consumer);
}

}
