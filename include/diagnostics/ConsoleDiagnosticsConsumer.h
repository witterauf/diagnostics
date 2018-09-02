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
    void printIndentation();
    void printLineNumber(size_t number);
    void printMarking(size_t number);
    
    void calculateSnippetLayout();
    auto snippetCodeLeft() const -> size_t;

    const Diagnostic* m_diagnostic = nullptr;

    bool m_includeLineNumber = true;
    size_t m_snippetIndentation = 2;
    size_t m_lineNumberWidth;
    size_t m_lineNumberIndentation = 2;
};

}