
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Unknown calling convention */

void convolution(output_mat_t *output, input_mat_t *input, filter7x7_t *filter)

{
    undefined auVar1[16];
    undefined auVar2[32];
    undefined auVar3[32];
    uint uVar4;
    anon_struct_9_1_3fcf6523 *paVar5;
    filter7x7_t *pfVar6;
    ulong uVar7;
    byte bVar8;
    ulong uVar9;
    ulong uVar10;
    filter7x7_t *pfVar11;
    filter7x7_t *pfVar12;
    byte bVar13;
    anon_union_331776_3_aba9db02_for_input_mat_t_0 *paVar14;
    anon_struct_9_1_3fcf6523 *paVar15;
    uint uVar16;
    anon_union_2359296_3_e9e7177a_for_output_mat_t_0 *paVar17;
    undefined auVar18[64];
    short sVar19;
    undefined auVar20[16];

    uVar4 = 0;
    do {
        uVar16 = 0;
        paVar15 = (anon_struct_9_1_3fcf6523 *)input;
        paVar17 = &output->field_0;
        do {
            uVar7 = 0;
            pfVar11 = filter;
            do {
                auVar18 = ZEXT3264((undefined[32])0x0);
                bVar8 = 0;
                paVar5 = paVar15;
                pfVar12 = pfVar11;
                do {
                    bVar13 = 0;
                    pfVar6 = pfVar12;
                    paVar14 = (anon_union_331776_3_aba9db02_for_input_mat_t_0 *)paVar5;
                    do {
                        uVar9 = 0;
                        do {
                            sVar19 = (short)((anon_union_331776_3_aba9db02_for_input_mat_t_0 *)
                                                 paVar14->rows[0]
                                                     .cols)
                                         ->bytes[uVar9];
                            auVar20._2_2_ = sVar19;
                            auVar20._0_2_ = sVar19;
                            auVar20._4_2_ = sVar19;
                            auVar20._6_2_ = sVar19;
                            auVar20._8_2_ = sVar19;
                            auVar20._10_2_ = sVar19;
                            auVar20._12_2_ = sVar19;
                            auVar20._14_2_ = sVar19;
                            uVar10 = uVar9 + 1;
                            auVar1._8_8_ = 0;
                            auVar1._0_8_ = *(ulong *)(pfVar6 + uVar9);
                            auVar1 = vpmovsxbw_avx(auVar1);
                            auVar2 = vpmovzxwd_avx2(auVar20);
                            auVar3 = vpmovzxwd_avx2(auVar1);
                            auVar2 = vpmaddwd_avx2(auVar2, auVar3);
                            auVar2 = vpaddd_avx2(auVar18._0_32_, auVar2);
                            auVar18 = ZEXT3264(auVar2);
                            uVar9 = uVar10;
                        } while (uVar10 < 9);
                        bVar13 = bVar13 + 1;
                        paVar14 = (anon_union_331776_3_aba9db02_for_input_mat_t_0 *)((long)paVar14->bytes + 9);
                        pfVar6 = (filter7x7_t *)((long)&pfVar6->field_0 + 0x10);
                    } while (bVar13 < 7);
                    bVar8 = bVar8 + 1;
                    paVar5 = (anon_struct_9_1_3fcf6523 *)(paVar5->ch + 0x6c0);
                    pfVar12 = (filter7x7_t *)((long)&pfVar12->field_0 + 0x70);
                } while (bVar8 < 7);
                *(undefined(*)[32])((long)paVar17->rows[0].cols + uVar7 * 4) = auVar2;
                uVar7 = uVar7 + 8;
                pfVar11 = (filter7x7_t *)((long)&pfVar11->field_0 + 8);
            } while (uVar7 < 0x10);
            uVar16 = uVar16 + 1;
            paVar15 = (anon_struct_9_1_3fcf6523 *)(paVar15->ch + 9);
            paVar17 = (anon_union_2359296_3_e9e7177a_for_output_mat_t_0 *)((long)paVar17->dwords + 0x40);
        } while (uVar16 < 0xba);
        uVar4 = uVar4 + 1;
        output = (output_mat_t *)(((anon_struct_64_1_3fcf6523 *)output)->ch + 0xc00);
        input = (input_mat_t *)(((anon_struct_9_1_3fcf6523 *)input)->ch + 0x6c0);
    } while (uVar4 < 0xba);
    return;
}
