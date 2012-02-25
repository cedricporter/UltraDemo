
#include "Parser.h" 
#include "ETMachine.h"
#include "SymbolTable.h"
#include "Imm2asm.h"
#include <string>

namespace ETCompiler
{
    class ETController
    {
    private:
        IParser     *m_pParser;
        ETMachine   m_machine;
    public:
        ETController();

        int Initialiaze_Machine(const char* code);

        bool Step();

        unsigned long *Getdreg();

        unsigned long *Getreg();

        int GetCurrentLine();

    };


}