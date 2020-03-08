#pragma once

#include "DiagnosticsConsumer.h"
#include <iostream>

namespace diagnostics {

class ConsoleDiagnosticsConsumer : public DiagnosticsConsumer
{
public:
    ConsoleDiagnosticsConsumer() = default;
    explicit ConsoleDiagnosticsConsumer(std::ostream* out);
    void startPhase(const std::string& name) override;
    void consume(const Diagnostic& diagnostic) override;
    void endPhase() override;

    void showTag();
    void hideTag();

private:
    auto diagnostic() const -> const Diagnostic&;

    void printDiagnostic();
    void printLocation();
    void printMessage();
    void printSnippet();
    void printIndentation();
    void printLineNumber(size_t number);
    void printMarking(size_t number);
    void printDetails();

    void calculateSnippetLayout();
    auto snippetCodeLeft() const -> size_t;

    const Diagnostic* m_diagnostic{nullptr};
    std::ostream* m_out{&std::cout};

    bool m_includeLineNumber = true;
    size_t m_snippetIndentation = 2;
    size_t m_lineNumberWidth;
    size_t m_lineNumberIndentation = 2;
    bool m_showTag{false};
};

} // namespace diagnostics