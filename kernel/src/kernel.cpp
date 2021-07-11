#include "kernelUtil.h"
#include "IO.h"

extern "C" void _start(BootInfo* bootInfo){

    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer->Clear(0x000a1a);
    GlobalRenderer->Print("Kernel Initialized Successfully"); GlobalRenderer->Next();
    
    extern uint64_t _bssStart;
	extern uint64_t _bssEnd;

	uint64_t bssSize = (uint64_t)&_bssEnd - (uint64_t)&_bssStart;
    void* bssaddr = &_bssStart;

    for(long i = 0; i < bssSize; i++){
        GlobalRenderer->Print(to_hstring(
                (uint8_t)*((uint8_t*)(bssaddr + i))
            ));
        GlobalRenderer->Print(", ");
    }

    while(true){};
}