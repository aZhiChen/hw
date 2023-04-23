#include "asm_utils.h"
#include "interrupt.h"
#include"stdio.h"

STDIO stdio;
// 中断管理器
InterruptManager interruptManager;

extern "C" void setup_kernel()
{
    // 中断处理部件
    interruptManager.initialize();

    // 尝试触发除0错误
    //int a = 1 / 0;

    stdio.initialize();
    interruptManager.enableTimeInterrupt();
    interruptManager.setTimeInterrupt((void *)asm_time_interrupt_handler );
    asm_enable_interrupt();
    // 死循环
    asm_halt();
}
