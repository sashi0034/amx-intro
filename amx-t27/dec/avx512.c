
/* WARNING: Unknown calling convention */

void convolution(output_mat_t *output, input_mat_t *input, filter7x7_t *filter)

{
    filter7x7_t *pfVar1;
    ulong uVar2;
    uint uVar3;
    ulong uVar4;
    ulong uVar5;
    byte bVar6;
    ulong uVar7;
    filter7x7_t *pfVar8;
    byte bVar9;
    undefined in_ZMM0[64];
    undefined auVar10[64];
    undefined auVar11[64];
    undefined in_ZMM16[64];
    undefined in_ZMM17[64];
    undefined in_ZMM18[64];

    uVar5 = 0;
    uVar2 = 0;
    auVar10 = vpxord_avx512f(in_ZMM16, in_ZMM0);
    do {
        uVar3 = 0;
        uVar7 = uVar2;
        do {
            vmovaps_avx512f(auVar10);
            bVar9 = 0;
            pfVar8 = filter;
            do {
                bVar6 = 0;
                uVar4 = uVar2;
                pfVar1 = pfVar8;
                do {
                    do {
                        auVar11 = vpmovsxbd_avx512f(*(undefined(*)[16])
                                                         pfVar1[uVar4]
                                                             .field_0.rows[0]
                                                             .cols[0]
                                                             .ch);
                        uVar4 = uVar4 + 1;
                        vpbroadcastd_avx512f();
                        auVar11 = vpmulld_avx512f(in_ZMM18, auVar11);
                        auVar11 = vpaddd_avx512f(in_ZMM17, auVar11);
                    } while (uVar4 < 9);
                    bVar6 = bVar6 + 1;
                    pfVar1 = (filter7x7_t *)((long)&pfVar1->field_0 + 0x10);
                    uVar4 = uVar2;
                } while (bVar6 < 7);
                bVar9 = bVar9 + 1;
                pfVar8 = (filter7x7_t *)((pfVar8->field_0).rows + 1);
            } while (bVar9 < 7);
            uVar3 = uVar3 + 1;
            auVar11 = vmovdqu32_avx512f(auVar11);
            *(undefined(*)[64])((long)&output->field_0 + uVar7 + uVar5 * 0x3000) = auVar11;
            uVar7 = uVar7 + 0x40;
        } while (uVar3 < 0xba);
        uVar5 = uVar5 + 1;
    } while (uVar5 < 0xba);
    return;
}
