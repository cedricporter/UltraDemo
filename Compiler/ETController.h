
#include "Parser.h" 
#include "ETMachine.h"
#include "SymbolTable.h"
#include "Imm2asm.h"
#include <string>
#include "def.h"

namespace ETCompiler
{
    class ETController
    {
    private:
        IParser     *m_pParser;
        ETMachine   m_machine;
        IDebugger   *m_memoryWatcher;
        WatchPropertiesListType::iterator currentIter;

    public:
        WatchPropertiesListType m_memoryList;


    public:
        ETController();

        int Initialiaze_Machine(const char* code, char* error_message);

        bool Step();

        unsigned long *Getdreg();

        unsigned long *Getreg();

        int GetCurrentLine();

        // 返回内存变量列表
        WatchPropertiesListType& GetMemoryList();

        // 返回第一个元素
        PropertiesInsertItem* GetFirstItem();

        // 返回下一个
        PropertiesInsertItem* GetNextItem();
    };


}