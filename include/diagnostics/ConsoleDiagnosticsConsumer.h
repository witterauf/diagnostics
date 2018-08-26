#pragma once

#include "DiagnosticsConsumer.h"

namespace diagnostics {

class ConsoleDiagnosticsConsumer : public DiagnosticsConsumer
{
public:
    void startPhase(const std::string& name) override;
    void consume(const Diagnostic& diagnostic) override;
    void endPhase() override;
};

}