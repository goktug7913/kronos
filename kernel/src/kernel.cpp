#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo){

    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer->Print("Kernel Initialized Successfully");

    int* test = (int*)0x9012030120130;

    *test = 2;

    while(true);
}