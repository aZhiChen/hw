%include "boot.inc"
org 0x7e00
[bits 16]
mov ax, 0xb800
mov gs, ax
mov ah, 0x03 ;青色
xor ebx, ebx

;空描述符
mov dword [GDT_START_ADDRESS+0x00],0x00
mov dword [GDT_START_ADDRESS+0x04],0x00  

;创建描述符，这是一个数据段，对应0~4GB的线性地址空间
mov dword [GDT_START_ADDRESS+0x08],0x0000ffff    ; 基地址为0，段界限为0xFFFFF
mov dword [GDT_START_ADDRESS+0x0c],0x00cf9200    ; 粒度为4KB，存储器段描述符 

;建立保护模式下的堆栈段描述符      
mov dword [GDT_START_ADDRESS+0x10],0x00000000    ; 基地址为0x00000000，界限0x0 
mov dword [GDT_START_ADDRESS+0x14],0x00409600    ; 粒度为1个字节

;建立保护模式下的显存描述符   
mov dword [GDT_START_ADDRESS+0x18],0x80007fff    ; 基地址为0x000B8000，界限0x07FFF 
mov dword [GDT_START_ADDRESS+0x1c],0x0040920b    ; 粒度为字节

;创建保护模式下平坦模式代码段描述符
mov dword [GDT_START_ADDRESS+0x20],0x0000ffff    ; 基地址为0，段界限为0xFFFFF
mov dword [GDT_START_ADDRESS+0x24],0x00cf9800    ; 粒度为4kb，代码段描述符 

;初始化描述符表寄存器GDTR
mov word [pgdt], 39      ;描述符表的界限   
lgdt [pgdt]
      
in al,0x92                         ;南桥芯片内的端口 
or al,0000_0010B
out 0x92,al                        ;打开A20

cli                                ;中断机制尚未工作
mov eax,cr0
or eax,1
mov cr0,eax                        ;设置PE位
      
;以下进入保护模式
jmp dword CODE_SELECTOR:assi4

;16位的描述符选择子：32位偏移
;清流水线并串行化处理器
[bits 32]        
 assi4:  

mov eax, DATA_SELECTOR                     ;加载数据段(0..4GB)选择子
mov ds, eax
mov es, eax
mov eax, STACK_SELECTOR
mov ss, eax
mov eax, VIDEO_SELECTOR
mov gs, eax

mov ah,3fh ;青色
mov esi,BootMessage
mov ebx,0

loop1:
	dec word[count]				; 递减计数变量
	jnz loop1					; >0：跳转;
	mov word[count],delay
	dec word[dcount]			; 递减计数变量
    jnz loop1
	mov word[count],delay
	mov word[dcount],ddelay
	; 以上是用一个二重循环实现时延50000*580个单位时间
	
	jmp			Entrance		;进行一个周期的工作
	jmp			$				;halt

Entrance:	
    jmp			BoundaryCheckx

Set:
    mov esi,BootMessage
    xor ecx,ecx
    mov cl,byte[x]
    imul ecx,2
    add ebx,ecx
    sub ebx,4
    xor ecx,ecx 
    mov cl,byte[y]
    imul ecx,80
    add ebx,ecx
    ;xor ecx,ecx
    ;mov ecx,[x]


DispStr:
  ;  mov esi,BootMessage
  ;  mov	 ax, BootMessage		;打印字符串
    mov al,[esi]
    mov word[gs:ebx],ax
    add ebx,2
    inc esi
    mov ecx,esi
    sub ecx,BootMessage
    cmp ecx,3
    jl DispStr 
    ;jmp Updatexy

Updatexy:
	mov 		al, byte[x]
	add			al, byte[vx]
	mov			byte[x], al
	mov 		al, byte[y]
	add			al, byte[vy]
	mov			byte[y], al

changeCol:
   mov     dh,ah
   cmp     dh,1eh
    jz      col1
    mov     dh,ah
    cmp     dh,5ah
    jz      col2
    mov     dh,ah
    cmp     dh,6bh
    jz      col3   
    mov     dh,ah
    cmp     dh,25h
    jz      col4 
    mov     dh,ah
    cmp     dh,42h
    jz      col5  
    mov     dh,ah
    cmp     dh,3fh
    jz      col6
    mov     dh,ah
    cmp     dh,7ch
    jz      col7

	;jmp			loop1				;无限循环
	
	
BoundaryCheckx:
	mov 		al, byte[x]
	add			al, byte[vx]	;预测下一刻的x
	cmp			al, byte[upper]	;如果x小于上边界
	jl			Changevx		;更新vx
	cmp			al, byte[lower]	;如果x大于下边界
	jg			Changevx		;更新vx
	
BoundaryChecky:
	mov 		al, byte[y]
	add			al, byte[vy]
	cmp			al, byte[left]	;如果y小于左边界
	jl			Changevy		;更新vy
	add			al, byte[Strlen];预测下一刻的yr=y+字符串长
	cmp			al, byte[right]	;如果yr大于下边界
	jg			Changevy		;更新vy
	jmp			Set		;如果不需要更新vx vy就继续打印流程			


Changevx:
	neg			byte[vx]
	jmp			BoundaryChecky
	
Changevy:
	neg			byte[vy]
	jmp			Set
	
    col1:
        mov ah,3fh
        jmp			loop1

    col2:
        mov ah,7ch
        jmp		loop1	

    col3:
        mov ah,1eh
        jmp			loop1		
    
    col4:
        mov ah,5ah
        jmp			loop1

    col5:
        mov ah,6bh
        jmp			loop1	
    
    col6:
        mov ah,25h
        jmp			loop1	
    
    col7:
        mov    ah,42h
        jmp			loop1	



BootMessage		db		"zhi"
Strlen              db      3
delay 				equ 	50
ddelay 				equ 	500
count 				dw 		delay
dcount 				dw 		ddelay

x    				db 		0
y    				db 		0
vx    				db 		1
vy    				db 		1
left					db		0
upper				db		0
right				db		79
lower				db		24

boundary			db		10


pgdt dw 0
     dd GDT_START_ADDRESS

times 512*5- ($ - $$) db 0
db 0x55, 0xaa