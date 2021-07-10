#include "kernelUtil.h"
#include "IO.h"

extern "C" void _start(BootInfo* bootInfo){

    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer->Clear(0x000a1a);
    GlobalRenderer->Print("Kernel Initialized Successfully"); GlobalRenderer->Next();
    
    while(true){};
}