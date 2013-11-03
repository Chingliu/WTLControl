#pragma once

#ifdef _DEBUG

#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif

class CMemLeakCheck
{
public:
    CMemLeakCheck()
    {
        _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    }
    
    virtual ~CMemLeakCheck()
    {
        _CrtDumpMemoryLeaks();
    }
};

static CMemLeakCheck memLeakCheck;
