#include "DiagnosticsReporter.h"
#include "DiagnosticsConsumer.h"

namespace diagnostics {

void DiagnosticsReporter::startPhase(const std::string& name)
{
    for (auto* consumer : m_consumers)
    {
        consumer->startPhase(name);
    }
}

void DiagnosticsReporter::consume(const Diagnostic& diagnostic)
{
    report(diagnostic);
}

void DiagnosticsReporter::endPhase()
{
    for (auto* consumer : m_consumers)
    {
        consumer->endPhase();
    }
}

auto DiagnosticsReporter::report(const std::string& message) -> DiagnosticsBuilder
{
    return DiagnosticsBuilder{ this, message };
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
