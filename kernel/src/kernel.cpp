#include "kernelUtil.h"
#include "IO.h"

extern "C" void _start(BootInfo* bootInfo){

    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer->Print("Kernel Initialized Successfully"); GlobalRenderer->Next();
    GlobalRenderer->Print(to_string(GlobalRenderer->backbufSize));
    while(true){};
}