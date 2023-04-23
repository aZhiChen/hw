#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "os_type.h"

class InterruptManager
{
    private:
        // initial address of IDT
        uint32 *IDT;
	uint32 IRQ0_8259A_MASTER; // 主片中断起始向量号
    	uint32 IRQ0_8259A_SLAVE;  // 从片中断起始向量号
	void initialize8259A();
    public:
        InterruptManager();
        //initialize
        void initialize();
        //set index--describe_number, address--program_start_address, DPL--level
        void setInterruptDescriptor( uint32 index, uint32 adderss, byte DPL );
	// 开启时钟中断
    void enableTimeInterrupt();
    // 禁止时钟中断
    void disableTimeInterrupt();
    // 设置时钟中断处理函数
    void setTimeInterrupt(void *handler);
};
#endif
