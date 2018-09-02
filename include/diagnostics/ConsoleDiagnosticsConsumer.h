#pragma once

#include "DiagnosticsConsumer.h"

namespace diagnostics {

class ConsoleDiagnosticsConsumer : public DiagnosticsConsumer
{
public:
    void startPhase(const std::string& name) override;
    void consume(const Diagnostic& diagnostic) override;
    void endPhase() override;

private:
    auto diagnostic() const -> const Diagnostic&;

    void printDiagnostic();
    void printLocation();
    void printMessage();
    void printSnippet();

    const Diagnostic* m_diagnostic = nullptr;
};

}