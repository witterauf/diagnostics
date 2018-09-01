#pragma once

#include "DiagnosticsBuilder.h"
#include "DiagnosticsConsumer.h"
#include <memory>
#include <set>
#include <vector>

namespace diagnostics {

class DiagnosticsReporter : public DiagnosticsConsumer
{
public:
    virtual ~DiagnosticsReporter() = default;

    virtual auto report(const std::string& message) -> DiagnosticsBuilder;
    virtual void report(const Diagnostic& diagnostic);

    void startPhase(const std::string& name) override;
    void consume(const Diagnostic& diagnostic) override;
    void endPhase() override;

    void registerConsumer(DiagnosticsConsumer* consumer);
    void unregisterConsumer(DiagnosticsConsumer* consumer);

    template<class Reporter, class... Args>
    auto makeProxyUnique(Args&&... args) -> std::unique_ptr<Reporter>
    {
        auto proxy = std::make_unique<Reporter>(std::forward<Args>(args)...);
        proxy->registerConsumer(this);
        return std::move(proxy);
    }

    template<class Reporter, class... Args>
    auto makeProxy(Args&&... args) -> Reporter
    {
        Reporter proxy{ std::forward<Args>(args)... };
        proxy.registerConsumer(this);
        return std::move(proxy);
    }

private:
    std::set<DiagnosticsConsumer*> m_consumers;
};

}
