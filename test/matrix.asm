# mark_description "Intel(R) C Intel(R) 64 Compiler Classic for applications running on Intel(R) 64, Version 2021.9.0 Build 2023";
# mark_description "0302_000000";
# mark_description "-diag-disable=10441 -xCORE-AVX512 -O3 -qopt-zmm-usage=high -S -o matrix.asm";
	.file "matrix.c"
	.text
..TXTST0:
.L_2__routine_start_main_0:
# -- Begin  main
	.text
# mark_begin;
       .align    16,0x90
	.globl main
# --- main()
main:
..B1.1:                         # Preds ..B1.0
                                # Execution count [1.00e+00]
	.cfi_startproc
..___tag_value_main.1:
..L2:
                                                          #28.12
        pushq     %rbp                                          #28.12
	.cfi_def_cfa_offset 16
        movq      %rsp, %rbp                                    #28.12
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        andq      $-128, %rsp                                   #28.12
        pushq     %r12                                          #28.12
        pushq     %r13                                          #28.12
        pushq     %r14                                          #28.12
        pushq     %r15                                          #28.12
        pushq     %rbx                                          #28.12
        subq      $1368, %rsp                                   #28.12
        movl      $3, %edi                                      #28.12
        movq      $0x64199d9ffe, %rsi                           #28.12
        call      __intel_new_feature_proc_init                 #28.12
	.cfi_escape 0x10, 0x03, 0x0e, 0x38, 0x1c, 0x0d, 0x80, 0xff, 0xff, 0xff, 0x1a, 0x0d, 0xd8, 0xff, 0xff, 0xff, 0x22
	.cfi_escape 0x10, 0x0c, 0x0e, 0x38, 0x1c, 0x0d, 0x80, 0xff, 0xff, 0xff, 0x1a, 0x0d, 0xf8, 0xff, 0xff, 0xff, 0x22
	.cfi_escape 0x10, 0x0d, 0x0e, 0x38, 0x1c, 0x0d, 0x80, 0xff, 0xff, 0xff, 0x1a, 0x0d, 0xf0, 0xff, 0xff, 0xff, 0x22
	.cfi_escape 0x10, 0x0e, 0x0e, 0x38, 0x1c, 0x0d, 0x80, 0xff, 0xff, 0xff, 0x1a, 0x0d, 0xe8, 0xff, 0xff, 0xff, 0x22
	.cfi_escape 0x10, 0x0f, 0x0e, 0x38, 0x1c, 0x0d, 0x80, 0xff, 0xff, 0xff, 0x1a, 0x0d, 0xe0, 0xff, 0xff, 0xff, 0x22
                                # LOE
..B1.26:                        # Preds ..B1.1
                                # Execution count [1.00e+00]
        vstmxcsr  (%rsp)                                        #28.12
        movl      $8, %esi                                      #33.5
        movl      $.L_2__STRING.2, %edi                         #33.5
        orl       $32832, (%rsp)                                #28.12
        movl      %esi, %edx                                    #33.5
        xorl      %eax, %eax                                    #33.5
        vldmxcsr  (%rsp)                                        #28.12
..___tag_value_main.11:
#       printf(const char *__restrict__, ...)
        call      printf                                        #33.5
..___tag_value_main.12:
                                # LOE
..B1.2:                         # Preds ..B1.26
                                # Execution count [1.00e+00]
        xorb      %r12b, %r12b                                  #34.5
        lea       256(%rsp), %rbx                               #35.22
                                # LOE rbx r12b
..B1.3:                         # Preds ..B1.4 ..B1.2
                                # Execution count [6.40e+01]
        movl      $.L_2__STRING.3, %edi                         #35.9
        movq      %rbx, %rsi                                    #35.9
        xorl      %eax, %eax                                    #35.9
#       scanf(const char *__restrict__, ...)
        call      __isoc99_scanf                                #35.9
                                # LOE rbx r12b
..B1.4:                         # Preds ..B1.3
                                # Execution count [6.40e+01]
        incb      %r12b                                         #34.5
        addq      $4, %rbx                                      #34.5
        cmpb      $64, %r12b                                    #34.5
        jb        ..B1.3        # Prob 98%                      #34.5
                                # LOE rbx r12b
..B1.5:                         # Preds ..B1.4
                                # Execution count [1.00e+00]
        movl      $8, %esi                                      #38.5
        movl      $.L_2__STRING.4, %edi                         #38.5
        movl      %esi, %edx                                    #38.5
        xorl      %eax, %eax                                    #38.5
..___tag_value_main.13:
#       printf(const char *__restrict__, ...)
        call      printf                                        #38.5
..___tag_value_main.14:
                                # LOE
..B1.6:                         # Preds ..B1.5
                                # Execution count [1.00e+00]
        xorb      %r12b, %r12b                                  #39.5
        lea       (%rsp), %rbx                                  #40.22
                                # LOE rbx r12b
..B1.7:                         # Preds ..B1.8 ..B1.6
                                # Execution count [6.40e+01]
        movl      $.L_2__STRING.3, %edi                         #40.9
        movq      %rbx, %rsi                                    #40.9
        xorl      %eax, %eax                                    #40.9
#       scanf(const char *__restrict__, ...)
        call      __isoc99_scanf                                #40.9
                                # LOE rbx r12b
..B1.8:                         # Preds ..B1.7
                                # Execution count [6.40e+01]
        incb      %r12b                                         #39.5
        addq      $4, %rbx                                      #39.5
        cmpb      $64, %r12b                                    #39.5
        jb        ..B1.7        # Prob 98%                      #39.5
                                # LOE rbx r12b
..B1.9:                         # Preds ..B1.8
                                # Execution count [1.00e+00]
        movl      164(%rsp), %r15d                              #44.31
        xorb      %r13b, %r13b                                  #44.5
        movl      %r15d, 872(%rsp)                              #44.31[spill]
        movl      196(%rsp), %r15d                              #44.31
        movl      %r15d, 880(%rsp)                              #44.31[spill]
        movl      228(%rsp), %r15d                              #44.31
        movl      %r15d, 888(%rsp)                              #44.31[spill]
        movl      8(%rsp), %r15d                                #44.31
        movl      %r15d, 896(%rsp)                              #44.31[spill]
        movl      40(%rsp), %r15d                               #44.31
        movl      %r15d, 904(%rsp)                              #44.31[spill]
        movl      72(%rsp), %r15d                               #44.31
        movl      %r15d, 912(%rsp)                              #44.31[spill]
        movl      104(%rsp), %r15d                              #44.31
        movl      %r15d, 920(%rsp)                              #44.31[spill]
        movl      136(%rsp), %r15d                              #44.31
        movl      %r15d, 928(%rsp)                              #44.31[spill]
        movl      168(%rsp), %r15d                              #44.31
        movl      %r15d, 936(%rsp)                              #44.31[spill]
        movl      200(%rsp), %r15d                              #44.31
        movl      %r15d, 944(%rsp)                              #44.31[spill]
        movl      232(%rsp), %r15d                              #44.31
        movl      %r15d, 952(%rsp)                              #44.31[spill]
        movl      12(%rsp), %r15d                               #44.31
        movl      %r15d, 512(%rsp)                              #44.31[spill]
        movl      44(%rsp), %r15d                               #44.31
        movl      %r15d, 960(%rsp)                              #44.31[spill]
        movl      76(%rsp), %r15d                               #44.31
        movl      %r15d, 968(%rsp)                              #44.31[spill]
        movl      108(%rsp), %r15d                              #44.31
        movl      %r15d, 976(%rsp)                              #44.31[spill]
        movl      140(%rsp), %r15d                              #44.31
        movl      %r15d, 984(%rsp)                              #44.31[spill]
        movl      172(%rsp), %r15d                              #44.31
        movl      %r15d, 992(%rsp)                              #44.31[spill]
        movl      204(%rsp), %r15d                              #44.31
        movl      %r15d, 1000(%rsp)                             #44.31[spill]
        movl      236(%rsp), %r15d                              #44.31
        movl      %r15d, 1008(%rsp)                             #44.31[spill]
        movl      16(%rsp), %r15d                               #44.31
        movl      %r15d, 1016(%rsp)                             #44.31[spill]
        movl      48(%rsp), %r15d                               #44.31
        movl      %r15d, 1024(%rsp)                             #44.31[spill]
        movl      80(%rsp), %r15d                               #44.31
        movl      %r15d, 1032(%rsp)                             #44.31[spill]
        movl      112(%rsp), %r15d                              #44.31
        movl      %r15d, 1040(%rsp)                             #44.31[spill]
        movl      144(%rsp), %r15d                              #44.31
        movl      %r15d, 1048(%rsp)                             #44.31[spill]
        movl      176(%rsp), %r15d                              #44.31
        movl      %r15d, 1056(%rsp)                             #44.31[spill]
        movl      208(%rsp), %r15d                              #44.31
        movl      %r15d, 1064(%rsp)                             #44.31[spill]
        movl      240(%rsp), %r15d                              #44.31
        movl      %r15d, 1072(%rsp)                             #44.31[spill]
        movl      20(%rsp), %r15d                               #44.31
        movl      %r15d, 1080(%rsp)                             #44.31[spill]
        movl      52(%rsp), %r15d                               #44.31
        movl      %r15d, 1088(%rsp)                             #44.31[spill]
        movl      84(%rsp), %r15d                               #44.31
        movl      %r15d, 1096(%rsp)                             #44.31[spill]
        movl      116(%rsp), %r15d                              #44.31
        movl      %r15d, 1104(%rsp)                             #44.31[spill]
        movl      148(%rsp), %r15d                              #44.31
        movl      %r15d, 1112(%rsp)                             #44.31[spill]
        movl      180(%rsp), %r15d                              #44.31
        movl      %r15d, 1120(%rsp)                             #44.31[spill]
        movl      212(%rsp), %r15d                              #44.31
        movl      %r15d, 1128(%rsp)                             #44.31[spill]
        movl      244(%rsp), %r15d                              #44.31
        movl      %r15d, 1136(%rsp)                             #44.31[spill]
        movl      24(%rsp), %r15d                               #44.31
        movl      %r15d, 1144(%rsp)                             #44.31[spill]
        movl      56(%rsp), %r15d                               #44.31
        movl      %r15d, 1152(%rsp)                             #44.31[spill]
        movl      88(%rsp), %r15d                               #44.31
        movl      %r15d, 1160(%rsp)                             #44.31[spill]
        movl      120(%rsp), %r15d                              #44.31
        movl      %r15d, 1168(%rsp)                             #44.31[spill]
        movl      152(%rsp), %r15d                              #44.31
        movl      %r15d, 1176(%rsp)                             #44.31[spill]
        movl      184(%rsp), %r15d                              #44.31
        movl      %r15d, 1184(%rsp)                             #44.31[spill]
        movl      216(%rsp), %r15d                              #44.31
        movl      %r15d, 1192(%rsp)                             #44.31[spill]
        movl      248(%rsp), %r15d                              #44.31
        movl      %r15d, 1200(%rsp)                             #44.31[spill]
        movl      28(%rsp), %r15d                               #44.31
        movl      %r15d, 1208(%rsp)                             #44.31[spill]
        movl      60(%rsp), %r15d                               #44.31
        movl      %r15d, 1216(%rsp)                             #44.31[spill]
        movl      92(%rsp), %r15d                               #44.31
        movl      %r15d, 1224(%rsp)                             #44.31[spill]
        movl      124(%rsp), %r15d                              #44.31
        movl      %r15d, 1232(%rsp)                             #44.31[spill]
        movl      156(%rsp), %r15d                              #44.31
        movl      %r15d, 1240(%rsp)                             #44.31[spill]
        movl      188(%rsp), %r15d                              #44.31
        movl      %r15d, 1248(%rsp)                             #44.31[spill]
        movl      224(%rsp), %eax                               #44.31
        movl      220(%rsp), %r15d                              #44.31
        movl      32(%rsp), %r11d                               #44.31
        movl      %eax, 864(%rsp)                               #44.31[spill]
        movl      %r15d, 1256(%rsp)                             #44.31[spill]
        movl      (%rsp), %r14d                                 #44.31
        movl      64(%rsp), %r10d                               #44.31
        movl      96(%rsp), %r9d                                #44.31
        movl      160(%rsp), %r8d                               #44.31
        movl      192(%rsp), %edi                               #44.31
        movl      132(%rsp), %esi                               #44.31
        movl      68(%rsp), %edx                                #44.31
        movl      4(%rsp), %ebx                                 #44.31
        movl      36(%rsp), %ecx                                #44.31
        movl      100(%rsp), %eax                               #44.31
        movl      252(%rsp), %r15d                              #44.31
        movl      %r11d, 840(%rsp)                              #44.31[spill]
        movl      128(%rsp), %r12d                              #44.31
        movl      %r15d, 776(%rsp)                              #44.31[spill]
        movl      %esi, 816(%rsp)                               #44.31[spill]
        movl      %eax, 824(%rsp)                               #44.31[spill]
        movl      %edx, 768(%rsp)                               #44.31[spill]
        movl      %ecx, 784(%rsp)                               #44.31[spill]
        movl      %ebx, 792(%rsp)                               #44.31[spill]
        movl      %edi, 800(%rsp)                               #44.31[spill]
        movl      %r8d, 808(%rsp)                               #44.31[spill]
        movl      %r9d, 848(%rsp)                               #44.31[spill]
        movl      %r10d, 856(%rsp)                              #44.31[spill]
        movl      %r14d, 832(%rsp)                              #44.31[spill]
        movl      512(%rsp), %r11d                              #44.31[spill]
                                # LOE r11d r12d r13b
..B1.10:                        # Preds ..B1.10 ..B1.9
                                # Execution count [8.00e+00]
        movb      %r13b, %al                                    #44.5
        incb      %r13b                                         #44.5
        shlb      $3, %al                                       #44.5
        movzbl    %al, %r10d                                    #44.5
        movl      832(%rsp), %r14d                              #44.5[spill]
        movl      840(%rsp), %r15d                              #44.5[spill]
        movl      260(%rsp,%r10,4), %edx                        #44.22
        movl      256(%rsp,%r10,4), %eax                        #44.22
        imull     %eax, %r14d                                   #44.5
        imull     %edx, %r15d                                   #44.5
        movl      264(%rsp,%r10,4), %ecx                        #44.22
        addl      %r15d, %r14d                                  #44.5
        movl      856(%rsp), %r15d                              #44.5[spill]
        imull     %ecx, %r15d                                   #44.5
        movl      268(%rsp,%r10,4), %ebx                        #44.22
        addl      %r15d, %r14d                                  #44.5
        movl      848(%rsp), %r15d                              #44.5[spill]
        imull     %ebx, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      %r12d, %r15d                                  #44.5
        movl      272(%rsp,%r10,4), %esi                        #44.22
        imull     %esi, %r15d                                   #44.5
        movl      276(%rsp,%r10,4), %edi                        #44.22
        addl      %r15d, %r14d                                  #44.5
        movl      808(%rsp), %r15d                              #44.5[spill]
        imull     %edi, %r15d                                   #44.5
        movl      280(%rsp,%r10,4), %r9d                        #44.22
        addl      %r15d, %r14d                                  #44.5
        movl      800(%rsp), %r15d                              #44.5[spill]
        imull     %r9d, %r15d                                   #44.5
        movl      284(%rsp,%r10,4), %r8d                        #44.22
        addl      %r15d, %r14d                                  #44.5
        movl      864(%rsp), %r15d                              #44.5[spill]
        imull     %r8d, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      %r14d, 512(%rsp,%r10,4)                       #44.40
        movl      792(%rsp), %r15d                              #44.5[spill]
        movl      784(%rsp), %r14d                              #44.5[spill]
        imull     %eax, %r15d                                   #44.5
        imull     %edx, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      768(%rsp), %r14d                              #44.5[spill]
        imull     %ecx, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      824(%rsp), %r14d                              #44.5[spill]
        imull     %ebx, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      816(%rsp), %r14d                              #44.5[spill]
        imull     %esi, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      872(%rsp), %r14d                              #44.5[spill]
        imull     %edi, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      880(%rsp), %r14d                              #44.5[spill]
        imull     %r9d, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      888(%rsp), %r14d                              #44.5[spill]
        imull     %r8d, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      %r15d, 516(%rsp,%r10,4)                       #44.40
        movl      896(%rsp), %r14d                              #44.5[spill]
        movl      904(%rsp), %r15d                              #44.5[spill]
        imull     %eax, %r14d                                   #44.5
        imull     %edx, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      912(%rsp), %r15d                              #44.5[spill]
        imull     %ecx, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      920(%rsp), %r15d                              #44.5[spill]
        imull     %ebx, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      928(%rsp), %r15d                              #44.5[spill]
        imull     %esi, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      936(%rsp), %r15d                              #44.5[spill]
        imull     %edi, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      944(%rsp), %r15d                              #44.5[spill]
        imull     %r9d, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      952(%rsp), %r15d                              #44.5[spill]
        imull     %r8d, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      %r11d, %r15d                                  #44.5
        movl      %r14d, 520(%rsp,%r10,4)                       #44.40
        movl      960(%rsp), %r14d                              #44.5[spill]
        imull     %eax, %r15d                                   #44.5
        imull     %edx, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      968(%rsp), %r14d                              #44.5[spill]
        imull     %ecx, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      976(%rsp), %r14d                              #44.5[spill]
        imull     %ebx, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      984(%rsp), %r14d                              #44.5[spill]
        imull     %esi, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      992(%rsp), %r14d                              #44.5[spill]
        imull     %edi, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      1000(%rsp), %r14d                             #44.5[spill]
        imull     %r9d, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      1008(%rsp), %r14d                             #44.5[spill]
        imull     %r8d, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      %r15d, 524(%rsp,%r10,4)                       #44.40
        movl      1016(%rsp), %r14d                             #44.5[spill]
        movl      1024(%rsp), %r15d                             #44.5[spill]
        imull     %eax, %r14d                                   #44.5
        imull     %edx, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      1032(%rsp), %r15d                             #44.5[spill]
        imull     %ecx, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      1040(%rsp), %r15d                             #44.5[spill]
        imull     %ebx, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      1048(%rsp), %r15d                             #44.5[spill]
        imull     %esi, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      1056(%rsp), %r15d                             #44.5[spill]
        imull     %edi, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      1064(%rsp), %r15d                             #44.5[spill]
        imull     %r9d, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      1072(%rsp), %r15d                             #44.5[spill]
        imull     %r8d, %r15d                                   #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      %r14d, 528(%rsp,%r10,4)                       #44.40
        movl      1080(%rsp), %r15d                             #44.5[spill]
        movl      1088(%rsp), %r14d                             #44.5[spill]
        imull     %eax, %r15d                                   #44.5
        imull     %edx, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      1096(%rsp), %r14d                             #44.5[spill]
        imull     %ecx, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      1104(%rsp), %r14d                             #44.5[spill]
        imull     %ebx, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      1112(%rsp), %r14d                             #44.5[spill]
        imull     %esi, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      1120(%rsp), %r14d                             #44.5[spill]
        imull     %edi, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      1128(%rsp), %r14d                             #44.5[spill]
        imull     %r9d, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      1136(%rsp), %r14d                             #44.5[spill]
        imull     %r8d, %r14d                                   #44.5
        addl      %r14d, %r15d                                  #44.5
        movl      %r15d, 532(%rsp,%r10,4)                       #44.40
        movl      1144(%rsp), %r14d                             #44.5[spill]
        movl      1152(%rsp), %r15d                             #44.5[spill]
        imull     %eax, %r14d                                   #44.5
        imull     1208(%rsp), %eax                              #44.5[spill]
        imull     %edx, %r15d                                   #44.5
        imull     1216(%rsp), %edx                              #44.5[spill]
        addl      %edx, %eax                                    #44.5
        addl      %r15d, %r14d                                  #44.5
        movl      1160(%rsp), %edx                              #44.5[spill]
        imull     %ecx, %edx                                    #44.5
        imull     1224(%rsp), %ecx                              #44.5[spill]
        addl      %ecx, %eax                                    #44.5
        addl      %edx, %r14d                                   #44.5
        .byte     144                                           #44.5
        movl      1168(%rsp), %ecx                              #44.5[spill]
        imull     %ebx, %ecx                                    #44.5
        imull     1232(%rsp), %ebx                              #44.5[spill]
        addl      %ebx, %eax                                    #44.5
        addl      %ecx, %r14d                                   #44.5
        movl      1176(%rsp), %ebx                              #44.5[spill]
        imull     %esi, %ebx                                    #44.5
        imull     1240(%rsp), %esi                              #44.5[spill]
        addl      %esi, %eax                                    #44.5
        addl      %ebx, %r14d                                   #44.5
        movl      1184(%rsp), %esi                              #44.5[spill]
        imull     %edi, %esi                                    #44.5
        imull     1248(%rsp), %edi                              #44.5[spill]
        addl      %edi, %eax                                    #44.5
        addl      %esi, %r14d                                   #44.5
        movl      1192(%rsp), %edi                              #44.5[spill]
        imull     %r9d, %edi                                    #44.5
        imull     1256(%rsp), %r9d                              #44.5[spill]
        addl      %r9d, %eax                                    #44.5
        addl      %edi, %r14d                                   #44.5
        movl      1200(%rsp), %r9d                              #44.5[spill]
        imull     %r8d, %r9d                                    #44.5
        imull     776(%rsp), %r8d                               #44.5[spill]
        addl      %r9d, %r14d                                   #44.5
        addl      %r8d, %eax                                    #44.5
        movl      %r14d, 536(%rsp,%r10,4)                       #44.40
        movl      %eax, 540(%rsp,%r10,4)                        #44.40
        cmpb      $8, %r13b                                     #44.5
        jb        ..B1.10       # Prob 87%                      #44.5
                                # LOE r11d r12d r13b
..B1.11:                        # Preds ..B1.10
                                # Execution count [1.00e+00]
        movl      $il0_peep_printf_format_0, %edi               #45.5
        call      puts                                          #45.5
                                # LOE
..B1.12:                        # Preds ..B1.11
                                # Execution count [1.00e+00]
        xorb      %bl, %bl                                      #46.5
                                # LOE bl
..B1.13:                        # Preds ..B1.22 ..B1.12
                                # Execution count [8.00e+00]
        movb      %bl, %dl                                      #46.5
        movl      $.L_2__STRING.0, %edi                         #46.5
        shlb      $3, %dl                                       #46.5
        xorl      %eax, %eax                                    #46.5
        movzbl    %dl, %r12d                                    #46.5
        movl      512(%rsp,%r12,4), %esi                        #46.5
..___tag_value_main.141:
#       printf(const char *__restrict__, ...)
        call      printf                                        #46.5
..___tag_value_main.142:
                                # LOE r12 bl
..B1.14:                        # Preds ..B1.13
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #46.5
        xorl      %eax, %eax                                    #46.5
        movl      516(%rsp,%r12,4), %esi                        #46.5
..___tag_value_main.143:
#       printf(const char *__restrict__, ...)
        call      printf                                        #46.5
..___tag_value_main.144:
                                # LOE r12 bl
..B1.15:                        # Preds ..B1.14
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #46.5
        xorl      %eax, %eax                                    #46.5
        movl      520(%rsp,%r12,4), %esi                        #46.5
..___tag_value_main.145:
#       printf(const char *__restrict__, ...)
        call      printf                                        #46.5
..___tag_value_main.146:
                                # LOE r12 bl
..B1.16:                        # Preds ..B1.15
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #46.5
        xorl      %eax, %eax                                    #46.5
        movl      524(%rsp,%r12,4), %esi                        #46.5
..___tag_value_main.147:
#       printf(const char *__restrict__, ...)
        call      printf                                        #46.5
..___tag_value_main.148:
                                # LOE r12 bl
..B1.17:                        # Preds ..B1.16
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #46.5
        xorl      %eax, %eax                                    #46.5
        movl      528(%rsp,%r12,4), %esi                        #46.5
..___tag_value_main.149:
#       printf(const char *__restrict__, ...)
        call      printf                                        #46.5
..___tag_value_main.150:
                                # LOE r12 bl
..B1.18:                        # Preds ..B1.17
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #46.5
        xorl      %eax, %eax                                    #46.5
        movl      532(%rsp,%r12,4), %esi                        #46.5
..___tag_value_main.151:
#       printf(const char *__restrict__, ...)
        call      printf                                        #46.5
..___tag_value_main.152:
                                # LOE r12 bl
..B1.19:                        # Preds ..B1.18
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #46.5
        xorl      %eax, %eax                                    #46.5
        movl      536(%rsp,%r12,4), %esi                        #46.5
..___tag_value_main.153:
#       printf(const char *__restrict__, ...)
        call      printf                                        #46.5
..___tag_value_main.154:
                                # LOE r12 bl
..B1.20:                        # Preds ..B1.19
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #46.5
        xorl      %eax, %eax                                    #46.5
        movl      540(%rsp,%r12,4), %esi                        #46.5
..___tag_value_main.155:
#       printf(const char *__restrict__, ...)
        call      printf                                        #46.5
..___tag_value_main.156:
                                # LOE bl
..B1.21:                        # Preds ..B1.20
                                # Execution count [8.00e+00]
        movl      $10, %edi                                     #46.5
        call      putchar                                       #46.5
                                # LOE bl
..B1.22:                        # Preds ..B1.21
                                # Execution count [8.00e+00]
        incb      %bl                                           #46.5
        cmpb      $8, %bl                                       #46.5
        jb        ..B1.13       # Prob 87%                      #46.5
                                # LOE bl
..B1.23:                        # Preds ..B1.22
                                # Execution count [1.00e+00]
        xorl      %eax, %eax                                    #48.12
        addq      $1368, %rsp                                   #48.12
	.cfi_restore 3
        popq      %rbx                                          #48.12
	.cfi_restore 15
        popq      %r15                                          #48.12
	.cfi_restore 14
        popq      %r14                                          #48.12
	.cfi_restore 13
        popq      %r13                                          #48.12
	.cfi_restore 12
        popq      %r12                                          #48.12
        movq      %rbp, %rsp                                    #48.12
        popq      %rbp                                          #48.12
	.cfi_def_cfa 7, 8
	.cfi_restore 6
        ret                                                     #48.12
        .align    16,0x90
                                # LOE
	.cfi_endproc
# mark_end;
	.type	main,@function
	.size	main,.-main
..LNmain.0:
	.section .rodata.str1.4, "aMS",@progbits,1
	.align 4
	.align 4
il0_peep_printf_format_0:
	.long	1953264973
	.long	2037149801
	.word	58
	.data
# -- End  main
	.text
.L_2__routine_start_multiplyMatrices_1:
# -- Begin  multiplyMatrices
	.text
# mark_begin;
       .align    16,0x90
	.globl multiplyMatrices
# --- multiplyMatrices(int *__restrict__, int *__restrict__, int *__restrict__)
multiplyMatrices:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
..B2.1:                         # Preds ..B2.0
                                # Execution count [1.00e+00]
	.cfi_startproc
..___tag_value_multiplyMatrices.165:
..L166:
                                                        #7.115
        pushq     %rbp                                          #7.115
	.cfi_def_cfa_offset 16
        movq      %rsp, %rbp                                    #7.115
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        pushq     %r12                                          #7.115
        pushq     %r13                                          #7.115
        pushq     %r14                                          #7.115
        pushq     %r15                                          #7.115
        pushq     %rbx                                          #7.115
        subq      $536, %rsp                                    #7.115
	.cfi_offset 3, -56
	.cfi_offset 12, -24
	.cfi_offset 13, -32
	.cfi_offset 14, -40
	.cfi_offset 15, -48
        xorl      %r13d, %r13d                                  #8.5
        movl      (%rsi), %eax                                  #12.59
        movl      32(%rsi), %ecx                                #12.59
        movl      64(%rsi), %r8d                                #12.59
        movl      96(%rsi), %r9d                                #12.59
        movl      128(%rsi), %r10d                              #12.59
        movl      160(%rsi), %r11d                              #12.59
        movq      %rdx, -576(%rbp)                              #7.115[spill]
        movq      %rdi, -568(%rbp)                              #7.115[spill]
        movl      %eax, -456(%rbp)                              #12.59[spill]
        movl      %ecx, -448(%rbp)                              #12.59[spill]
        movl      %r8d, -440(%rbp)                              #12.59[spill]
        movl      %r9d, -432(%rbp)                              #12.59[spill]
        movl      %r10d, -424(%rbp)                             #12.59[spill]
        movl      %r11d, -416(%rbp)                             #12.59[spill]
        movl      192(%rsi), %eax                               #12.59
        movl      224(%rsi), %edx                               #12.59
        movl      164(%rsi), %r10d                              #12.59
        movl      100(%rsi), %r8d                               #12.59
        movl      36(%rsi), %ebx                                #12.59
        movl      4(%rsi), %ecx                                 #12.59
        movl      68(%rsi), %edi                                #12.59
        movl      132(%rsi), %r9d                               #12.59
        movl      196(%rsi), %r11d                              #12.59
        movl      %eax, -408(%rbp)                              #12.59[spill]
        movl      %edx, -400(%rbp)                              #12.59[spill]
        movl      %ecx, -392(%rbp)                              #12.59[spill]
        movl      %ebx, -384(%rbp)                              #12.59[spill]
        movl      %edi, -376(%rbp)                              #12.59[spill]
        movl      %r8d, -368(%rbp)                              #12.59[spill]
        movl      %r9d, -360(%rbp)                              #12.59[spill]
        movl      %r10d, -352(%rbp)                             #12.59[spill]
        movl      %r11d, -344(%rbp)                             #12.59[spill]
        movl      228(%rsi), %r12d                              #12.59
        movl      168(%rsi), %ebx                               #12.59
        movl      104(%rsi), %edx                               #12.59
        movl      40(%rsi), %r15d                               #12.59
        movl      8(%rsi), %r14d                                #12.59
        movl      72(%rsi), %eax                                #12.59
        movl      136(%rsi), %ecx                               #12.59
        movl      200(%rsi), %edi                               #12.59
        movl      232(%rsi), %r8d                               #12.59
        movl      12(%rsi), %r9d                                #12.59
        movl      44(%rsi), %r10d                               #12.59
        movl      76(%rsi), %r11d                               #12.59
        movl      %r12d, -336(%rbp)                             #12.59[spill]
        movl      %r14d, -328(%rbp)                             #12.59[spill]
        movl      %r15d, -320(%rbp)                             #12.59[spill]
        movl      %eax, -312(%rbp)                              #12.59[spill]
        movl      %edx, -304(%rbp)                              #12.59[spill]
        movl      %ecx, -296(%rbp)                              #12.59[spill]
        movl      %ebx, -288(%rbp)                              #12.59[spill]
        movl      %edi, -280(%rbp)                              #12.59[spill]
        movl      %r8d, -272(%rbp)                              #12.59[spill]
        movl      %r9d, -264(%rbp)                              #12.59[spill]
        movl      %r10d, -256(%rbp)                             #12.59[spill]
        movl      %r11d, -248(%rbp)                             #12.59[spill]
        movl      108(%rsi), %r12d                              #12.59
        movl      140(%rsi), %r14d                              #12.59
        movl      172(%rsi), %r15d                              #12.59
        movl      204(%rsi), %eax                               #12.59
        movl      236(%rsi), %edx                               #12.59
        movl      176(%rsi), %r10d                              #12.59
        movl      112(%rsi), %r8d                               #12.59
        movl      48(%rsi), %ebx                                #12.59
        movl      16(%rsi), %ecx                                #12.59
        movl      80(%rsi), %edi                                #12.59
        movl      144(%rsi), %r9d                               #12.59
        movl      208(%rsi), %r11d                              #12.59
        movl      %r12d, -240(%rbp)                             #12.59[spill]
        movl      %r14d, -232(%rbp)                             #12.59[spill]
        movl      %r15d, -224(%rbp)                             #12.59[spill]
        movl      %eax, -216(%rbp)                              #12.59[spill]
        movl      %edx, -208(%rbp)                              #12.59[spill]
        movl      %ecx, -200(%rbp)                              #12.59[spill]
        movl      %ebx, -192(%rbp)                              #12.59[spill]
        movl      %edi, -184(%rbp)                              #12.59[spill]
        movl      %r8d, -176(%rbp)                              #12.59[spill]
        movl      %r9d, -168(%rbp)                              #12.59[spill]
        movl      %r10d, -160(%rbp)                             #12.59[spill]
        movl      %r11d, -152(%rbp)                             #12.59[spill]
        movl      240(%rsi), %r12d                              #12.59
        movl      180(%rsi), %ebx                               #12.59
        movl      116(%rsi), %edx                               #12.59
        movl      52(%rsi), %r15d                               #12.59
        movl      20(%rsi), %r14d                               #12.59
        movl      84(%rsi), %eax                                #12.59
        movl      148(%rsi), %ecx                               #12.59
        movl      212(%rsi), %edi                               #12.59
        movl      244(%rsi), %r8d                               #12.59
        movl      24(%rsi), %r9d                                #12.59
        movl      56(%rsi), %r10d                               #12.59
        movl      88(%rsi), %r11d                               #12.59
        movl      %r12d, -144(%rbp)                             #12.59[spill]
        movl      %r14d, -136(%rbp)                             #12.59[spill]
        movl      %r15d, -128(%rbp)                             #12.59[spill]
        movl      %eax, -120(%rbp)                              #12.59[spill]
        movl      %edx, -112(%rbp)                              #12.59[spill]
        movl      %ecx, -104(%rbp)                              #12.59[spill]
        movl      %ebx, -96(%rbp)                               #12.59[spill]
        movl      %edi, -88(%rbp)                               #12.59[spill]
        movl      %r8d, -80(%rbp)                               #12.59[spill]
        movl      %r9d, -72(%rbp)                               #12.59[spill]
        movl      %r10d, -64(%rbp)                              #12.59[spill]
        movl      %r11d, -56(%rbp)                              #12.59[spill]
        movl      120(%rsi), %r14d                              #12.59
        movl      152(%rsi), %r12d                              #12.59
        movl      184(%rsi), %r11d                              #12.59
        movl      216(%rsi), %r10d                              #12.59
        movl      248(%rsi), %r9d                               #12.59
        movl      188(%rsi), %eax                               #12.59
        movl      124(%rsi), %ecx                               #12.59
        movl      60(%rsi), %edi                                #12.59
        movl      28(%rsi), %r8d                                #12.59
        movl      92(%rsi), %ebx                                #12.59
        movl      156(%rsi), %edx                               #12.59
        movl      220(%rsi), %r15d                              #12.59
        movl      252(%rsi), %esi                               #12.59
        movl      %r11d, -544(%rbp)                             #12.59[spill]
        movl      %r12d, -552(%rbp)                             #12.59[spill]
        movl      %esi, -464(%rbp)                              #12.59[spill]
        movl      %r15d, -472(%rbp)                             #12.59[spill]
        movl      %eax, -480(%rbp)                              #12.59[spill]
        movl      %edx, -488(%rbp)                              #12.59[spill]
        movl      %ecx, -496(%rbp)                              #12.59[spill]
        movl      %ebx, -504(%rbp)                              #12.59[spill]
        movl      %edi, -512(%rbp)                              #12.59[spill]
        movl      %r8d, -520(%rbp)                              #12.59[spill]
        movl      %r9d, -528(%rbp)                              #12.59[spill]
        movl      %r10d, -536(%rbp)                             #12.59[spill]
        movl      %r14d, -560(%rbp)                             #12.59[spill]
        movq      -568(%rbp), %r11                              #12.59[spill]
        movq      -576(%rbp), %r12                              #12.59[spill]
                                # LOE r11 r12 r13
..B2.2:                         # Preds ..B2.2 ..B2.1
                                # Execution count [8.00e+00]
        movq      %r13, %r10                                    #12.54
        incq      %r13                                          #8.5
        shlq      $5, %r10                                      #12.54
        movl      -456(%rbp), %r14d                             #12.59[spill]
        movl      -448(%rbp), %r15d                             #12.59[spill]
        movl      (%r11,%r10), %eax                             #12.41
        movl      4(%r11,%r10), %edx                            #12.41
        imull     %eax, %r14d                                   #12.59
        imull     %edx, %r15d                                   #12.59
        movl      8(%r11,%r10), %ecx                            #12.41
        addl      %r15d, %r14d                                  #12.17
        movl      -440(%rbp), %r15d                             #12.59[spill]
        imull     %ecx, %r15d                                   #12.59
        movl      12(%r11,%r10), %ebx                           #12.41
        addl      %r15d, %r14d                                  #12.17
        movl      -432(%rbp), %r15d                             #12.59[spill]
        imull     %ebx, %r15d                                   #12.59
        movl      16(%r11,%r10), %esi                           #12.41
        addl      %r15d, %r14d                                  #12.17
        movl      -424(%rbp), %r15d                             #12.59[spill]
        imull     %esi, %r15d                                   #12.59
        movl      20(%r11,%r10), %edi                           #12.41
        addl      %r15d, %r14d                                  #12.17
        movl      -416(%rbp), %r15d                             #12.59[spill]
        imull     %edi, %r15d                                   #12.59
        movl      24(%r11,%r10), %r9d                           #12.41
        addl      %r15d, %r14d                                  #12.17
        movl      -408(%rbp), %r15d                             #12.59[spill]
        imull     %r9d, %r15d                                   #12.59
        movl      28(%r11,%r10), %r8d                           #12.41
        addl      %r15d, %r14d                                  #12.17
        movl      -400(%rbp), %r15d                             #12.59[spill]
        imull     %r8d, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      %r14d, (%r12,%r10)                            #12.17
        movl      -392(%rbp), %r15d                             #12.59[spill]
        movl      -384(%rbp), %r14d                             #12.59[spill]
        imull     %eax, %r15d                                   #12.59
        imull     %edx, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -376(%rbp), %r14d                             #12.59[spill]
        imull     %ecx, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -368(%rbp), %r14d                             #12.59[spill]
        imull     %ebx, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -360(%rbp), %r14d                             #12.59[spill]
        imull     %esi, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -352(%rbp), %r14d                             #12.59[spill]
        imull     %edi, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -344(%rbp), %r14d                             #12.59[spill]
        imull     %r9d, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -336(%rbp), %r14d                             #12.59[spill]
        imull     %r8d, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      %r15d, 4(%r12,%r10)                           #12.17
        movl      -328(%rbp), %r14d                             #12.59[spill]
        movl      -320(%rbp), %r15d                             #12.59[spill]
        imull     %eax, %r14d                                   #12.59
        imull     %edx, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -312(%rbp), %r15d                             #12.59[spill]
        imull     %ecx, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -304(%rbp), %r15d                             #12.59[spill]
        imull     %ebx, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -296(%rbp), %r15d                             #12.59[spill]
        imull     %esi, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -288(%rbp), %r15d                             #12.59[spill]
        imull     %edi, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -280(%rbp), %r15d                             #12.59[spill]
        imull     %r9d, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -272(%rbp), %r15d                             #12.59[spill]
        imull     %r8d, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      %r14d, 8(%r12,%r10)                           #12.17
        movl      -264(%rbp), %r15d                             #12.59[spill]
        movl      -256(%rbp), %r14d                             #12.59[spill]
        imull     %eax, %r15d                                   #12.59
        imull     %edx, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -248(%rbp), %r14d                             #12.59[spill]
        imull     %ecx, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -240(%rbp), %r14d                             #12.59[spill]
        imull     %ebx, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -232(%rbp), %r14d                             #12.59[spill]
        imull     %esi, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -224(%rbp), %r14d                             #12.59[spill]
        imull     %edi, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -216(%rbp), %r14d                             #12.59[spill]
        imull     %r9d, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -208(%rbp), %r14d                             #12.59[spill]
        imull     %r8d, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        .byte     102                                           #12.17
        .byte     144                                           #12.17
        movl      %r15d, 12(%r12,%r10)                          #12.17
        movl      -200(%rbp), %r14d                             #12.59[spill]
        movl      -192(%rbp), %r15d                             #12.59[spill]
        imull     %eax, %r14d                                   #12.59
        imull     %edx, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -184(%rbp), %r15d                             #12.59[spill]
        imull     %ecx, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -176(%rbp), %r15d                             #12.59[spill]
        imull     %ebx, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -168(%rbp), %r15d                             #12.59[spill]
        imull     %esi, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -160(%rbp), %r15d                             #12.59[spill]
        imull     %edi, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -152(%rbp), %r15d                             #12.59[spill]
        imull     %r9d, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      -144(%rbp), %r15d                             #12.59[spill]
        imull     %r8d, %r15d                                   #12.59
        addl      %r15d, %r14d                                  #12.17
        movl      %r14d, 16(%r12,%r10)                          #12.17
        movl      -136(%rbp), %r15d                             #12.59[spill]
        movl      -128(%rbp), %r14d                             #12.59[spill]
        imull     %eax, %r15d                                   #12.59
        imull     %edx, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -120(%rbp), %r14d                             #12.59[spill]
        imull     %ecx, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -112(%rbp), %r14d                             #12.59[spill]
        imull     %ebx, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -104(%rbp), %r14d                             #12.59[spill]
        imull     %esi, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -96(%rbp), %r14d                              #12.59[spill]
        imull     %edi, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -88(%rbp), %r14d                              #12.59[spill]
        imull     %r9d, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      -80(%rbp), %r14d                              #12.59[spill]
        imull     %r8d, %r14d                                   #12.59
        addl      %r14d, %r15d                                  #12.17
        movl      %r15d, 20(%r12,%r10)                          #12.17
        movl      -72(%rbp), %r14d                              #12.59[spill]
        movl      -64(%rbp), %r15d                              #12.59[spill]
        imull     %eax, %r14d                                   #12.59
        imull     -520(%rbp), %eax                              #12.59[spill]
        imull     %edx, %r15d                                   #12.59
        imull     -512(%rbp), %edx                              #12.59[spill]
        addl      %edx, %eax                                    #12.17
        addl      %r15d, %r14d                                  #12.17
        movl      -56(%rbp), %edx                               #12.59[spill]
        imull     %ecx, %edx                                    #12.59
        imull     -504(%rbp), %ecx                              #12.59[spill]
        addl      %ecx, %eax                                    #12.17
        addl      %edx, %r14d                                   #12.17
        movl      -560(%rbp), %ecx                              #12.59[spill]
        imull     %ebx, %ecx                                    #12.59
        imull     -496(%rbp), %ebx                              #12.59[spill]
        addl      %ebx, %eax                                    #12.17
        addl      %ecx, %r14d                                   #12.17
        movl      -552(%rbp), %ebx                              #12.59[spill]
        imull     %esi, %ebx                                    #12.59
        .byte     15                                            #12.59
        .byte     31                                            #12.59
        .byte     0                                             #12.59
        imull     -488(%rbp), %esi                              #12.59[spill]
        addl      %esi, %eax                                    #12.17
        addl      %ebx, %r14d                                   #12.17
        movl      -544(%rbp), %esi                              #12.59[spill]
        imull     %edi, %esi                                    #12.59
        imull     -480(%rbp), %edi                              #12.59[spill]
        addl      %edi, %eax                                    #12.17
        addl      %esi, %r14d                                   #12.17
        movl      -536(%rbp), %edi                              #12.59[spill]
        imull     %r9d, %edi                                    #12.59
        imull     -472(%rbp), %r9d                              #12.59[spill]
        addl      %r9d, %eax                                    #12.17
        addl      %edi, %r14d                                   #12.17
        movl      -528(%rbp), %r9d                              #12.59[spill]
        imull     %r8d, %r9d                                    #12.59
        imull     -464(%rbp), %r8d                              #12.59[spill]
        addl      %r9d, %r14d                                   #12.17
        addl      %r8d, %eax                                    #12.17
        movl      %r14d, 24(%r12,%r10)                          #12.17
        movl      %eax, 28(%r12,%r10)                           #12.17
        cmpq      $8, %r13                                      #8.5
        jb        ..B2.2        # Prob 87%                      #8.5
                                # LOE r11 r12 r13
..B2.3:                         # Preds ..B2.2
                                # Execution count [1.00e+00]
        lea       -40(%rbp), %rsp                               #16.1
	.cfi_restore 3
        popq      %rbx                                          #16.1
	.cfi_restore 15
        popq      %r15                                          #16.1
	.cfi_restore 14
        popq      %r14                                          #16.1
	.cfi_restore 13
        popq      %r13                                          #16.1
	.cfi_restore 12
        popq      %r12                                          #16.1
        popq      %rbp                                          #16.1
	.cfi_restore 6
        ret                                                     #16.1
        .align    16,0x90
                                # LOE
	.cfi_endproc
# mark_end;
	.type	multiplyMatrices,@function
	.size	multiplyMatrices,.-multiplyMatrices
..LNmultiplyMatrices.1:
	.data
# -- End  multiplyMatrices
	.text
.L_2__routine_start_printMatrix_2:
# -- Begin  printMatrix
	.text
# mark_begin;
       .align    16,0x90
	.globl printMatrix
# --- printMatrix(int *__restrict__)
printMatrix:
# parameter 1: %rdi
..B3.1:                         # Preds ..B3.0
                                # Execution count [1.00e+00]
	.cfi_startproc
..___tag_value_printMatrix.314:
..L315:
                                                        #19.52
        pushq     %r12                                          #19.52
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
        pushq     %r13                                          #19.52
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
        pushq     %r14                                          #19.52
	.cfi_def_cfa_offset 32
	.cfi_offset 14, -32
        xorb      %dl, %dl                                      #20.5
        xorl      %eax, %eax                                    #20.5
        movb      %dl, %r13b                                    #20.5
        movq      %rax, %r12                                    #20.5
        movq      %rdi, %r14                                    #20.5
                                # LOE rbx rbp r12 r14 r15 r13b
..B3.2:                         # Preds ..B3.11 ..B3.1
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #22.13
        xorl      %eax, %eax                                    #22.13
        movl      (%r12,%r14), %esi                             #22.13
..___tag_value_printMatrix.322:
#       printf(const char *__restrict__, ...)
        call      printf                                        #22.13
..___tag_value_printMatrix.323:
                                # LOE rbx rbp r12 r14 r15 r13b
..B3.3:                         # Preds ..B3.2
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #22.13
        xorl      %eax, %eax                                    #22.13
        movl      4(%r12,%r14), %esi                            #22.13
..___tag_value_printMatrix.324:
#       printf(const char *__restrict__, ...)
        call      printf                                        #22.13
..___tag_value_printMatrix.325:
                                # LOE rbx rbp r12 r14 r15 r13b
..B3.4:                         # Preds ..B3.3
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #22.13
        xorl      %eax, %eax                                    #22.13
        movl      8(%r12,%r14), %esi                            #22.13
..___tag_value_printMatrix.326:
#       printf(const char *__restrict__, ...)
        call      printf                                        #22.13
..___tag_value_printMatrix.327:
                                # LOE rbx rbp r12 r14 r15 r13b
..B3.5:                         # Preds ..B3.4
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #22.13
        xorl      %eax, %eax                                    #22.13
        movl      12(%r12,%r14), %esi                           #22.13
..___tag_value_printMatrix.328:
#       printf(const char *__restrict__, ...)
        call      printf                                        #22.13
..___tag_value_printMatrix.329:
                                # LOE rbx rbp r12 r14 r15 r13b
..B3.6:                         # Preds ..B3.5
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #22.13
        xorl      %eax, %eax                                    #22.13
        movl      16(%r12,%r14), %esi                           #22.13
..___tag_value_printMatrix.330:
#       printf(const char *__restrict__, ...)
        call      printf                                        #22.13
..___tag_value_printMatrix.331:
                                # LOE rbx rbp r12 r14 r15 r13b
..B3.7:                         # Preds ..B3.6
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #22.13
        xorl      %eax, %eax                                    #22.13
        movl      20(%r12,%r14), %esi                           #22.13
..___tag_value_printMatrix.332:
#       printf(const char *__restrict__, ...)
        call      printf                                        #22.13
..___tag_value_printMatrix.333:
                                # LOE rbx rbp r12 r14 r15 r13b
..B3.8:                         # Preds ..B3.7
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #22.13
        xorl      %eax, %eax                                    #22.13
        movl      24(%r12,%r14), %esi                           #22.13
..___tag_value_printMatrix.334:
#       printf(const char *__restrict__, ...)
        call      printf                                        #22.13
..___tag_value_printMatrix.335:
                                # LOE rbx rbp r12 r14 r15 r13b
..B3.9:                         # Preds ..B3.8
                                # Execution count [8.00e+00]
        movl      $.L_2__STRING.0, %edi                         #22.13
        xorl      %eax, %eax                                    #22.13
        movl      28(%r12,%r14), %esi                           #22.13
..___tag_value_printMatrix.336:
#       printf(const char *__restrict__, ...)
        call      printf                                        #22.13
..___tag_value_printMatrix.337:
                                # LOE rbx rbp r12 r14 r15 r13b
..B3.10:                        # Preds ..B3.9
                                # Execution count [8.00e+00]
        movl      $10, %edi                                     #24.9
        call      putchar                                       #24.9
                                # LOE rbx rbp r12 r14 r15 r13b
..B3.11:                        # Preds ..B3.10
                                # Execution count [8.00e+00]
        incb      %r13b                                         #20.5
        addq      $32, %r12                                     #20.5
        cmpb      $8, %r13b                                     #20.5
        jb        ..B3.2        # Prob 87%                      #20.5
                                # LOE rbx rbp r12 r14 r15 r13b
..B3.12:                        # Preds ..B3.11
                                # Execution count [1.00e+00]
	.cfi_restore 14
        popq      %r14                                          #26.1
	.cfi_def_cfa_offset 24
	.cfi_restore 13
        popq      %r13                                          #26.1
	.cfi_def_cfa_offset 16
	.cfi_restore 12
        popq      %r12                                          #26.1
	.cfi_def_cfa_offset 8
        ret                                                     #26.1
        .align    16,0x90
                                # LOE
	.cfi_endproc
# mark_end;
	.type	printMatrix,@function
	.size	printMatrix,.-printMatrix
..LNprintMatrix.2:
	.data
# -- End  printMatrix
	.section .rodata.str1.4, "aMS",@progbits,1
	.space 2, 0x00 	# pad
	.align 4
.L_2__STRING.2:
	.long	1970302537
	.long	1632444532
	.long	2020176500
	.long	540684576
	.long	543434024
	.long	1680154744
	.long	670249
	.type	.L_2__STRING.2,@object
	.size	.L_2__STRING.2,28
	.align 4
.L_2__STRING.3:
	.word	25637
	.byte	0
	.type	.L_2__STRING.3,@object
	.size	.L_2__STRING.3,3
	.space 1, 0x00 	# pad
	.align 4
.L_2__STRING.4:
	.long	1970302537
	.long	1632444532
	.long	2020176500
	.long	540684832
	.long	543434024
	.long	1680154744
	.long	670249
	.type	.L_2__STRING.4,@object
	.size	.L_2__STRING.4,28
	.align 4
.L_2__STRING.0:
	.long	2122789
	.type	.L_2__STRING.0,@object
	.size	.L_2__STRING.0,4
	.data
	.section .note.GNU-stack, ""
# End
