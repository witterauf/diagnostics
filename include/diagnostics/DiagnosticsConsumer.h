#pragma once

#include "Diagnostic.h"

namespace diagnostics {

class DiagnosticsConsumer
{
public:
    virtual void startPhase(const std::string& name) = 0;
    virtual void consume(const Diagnostic& diagnostic) = 0;
    virtual void endPhase() = 0;
};

}