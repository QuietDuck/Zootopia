// VISUAL LEAK DETECTOR
//#include <vld.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "bc_core.h"
#include "zpd.h"
using namespace zootopia;


int main() {

    ZPD::changeDir();

    // INPUT LEAK NUMBER.
    //_CrtSetBreakAlloc(365);

    BcCore* core = BcCore::getInstance();
    core->initialize(BcCore::BackendType::kOpenGL);
    core->run();
    core->terminate();

    //_CrtDumpMemoryLeaks();
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    return 0;
}