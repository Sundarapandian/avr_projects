armcc -c -g -O2 -cpu ARM9TM -D__TARGET_CPU_ARM9TM -Id:\arm250\include -I. -I..\..\inc -I..\..\ports\arm ..\..\src\pico\picoos.c
armcc -c -g -O2 -cpu ARM9TM -D__TARGET_CPU_ARM9TM -Id:\arm250\include -I. -I..\..\inc -I..\..\ports\arm ..\..\src\pico\fbit_gen.c
armcc -c -g -O2 -cpu ARM9TM -D__TARGET_CPU_ARM9TM -Id:\arm250\include -I. -I..\..\inc -I..\..\ports\arm arch_arm_c.c
armcc -c -g -O2 -cpu ARM9TM -D__TARGET_CPU_ARM9TM -Id:\arm250\include -I. -I..\..\inc -I..\..\ports\arm -Iboot boot\init.c
armcc -c -g -O2 -cpu ARM9TM -D__TARGET_CPU_ARM9TM -Id:\arm250\include -I. -I..\..\inc -I..\..\ports\arm -Iboot boot\mem.c
armcc -c -g -O2 -cpu ARM9TM -D__TARGET_CPU_ARM9TM -Id:\arm250\include -I. -I..\..\inc -I..\..\ports\arm -Iboot boot\interrupt.c

armasm -g -cpu ARM9TM arch_arm_asm.s 
armasm -g -cpu ARM9TM boot\boot.s 

armcc -c -g -O2 -cpu ARM9TM -D__TARGET_CPU_ARM9TM -Id:\arm250\include -I. -I..\..\inc -I..\..\ports\arm test\demo.c

armlink -info total -ro-base 0x100 -libpath D:\ARM250\lib\embedded -first Boot.o(Boot) -Output demo.axf boot.o arch_arm_asm.o arch_arm_c.o picoos.o fbit_gen.o init.o mem.o interrupt.o demo.o
