
void dot_product(dword8x8_t *param_1,bytes8x32_t *param_2,bytes8x32_t *param_3,dword8x8_t *c,
                bytes8x32_t *a,bytes8x32_t *b)

{
  int *piVar1;
  long lVar2;
  byte bVar3;
  ulong uVar4;
  long lVar5;
  undefined auVar6 [16];
  undefined auVar7 [16];
  undefined auVar8 [16];
  undefined auVar9 [16];
  undefined auVar10 [16];
  int iVar12;
  undefined in_XMM6 [16];
  undefined auVar11 [16];
  undefined auVar13 [16];
  undefined auVar14 [16];
  undefined auVar15 [16];
  undefined auVar16 [16];
  undefined auVar17 [16];
  undefined auVar18 [16];
  
  bVar3 = 0;
  lVar2 = 0;
  do {
    auVar9._8_8_ = 0;
    auVar9._0_8_ = *(ulong *)((long)&param_2->field_0 + lVar2 + 0x18);
    uVar4 = 0;
    auVar8._8_8_ = 0;
    auVar8._0_8_ = *(ulong *)((long)&param_2->field_0 + lVar2);
    auVar7._8_8_ = 0;
    auVar7._0_8_ = *(ulong *)((long)&param_2->field_0 + lVar2 + 0x10);
    lVar5 = 0;
    auVar10._8_8_ = 0;
    auVar10._0_8_ = *(ulong *)((long)&param_2->field_0 + lVar2 + 8);
    auVar9 = pmovsxbw(auVar9,auVar9);
    auVar8 = pmovsxbw(auVar8,auVar8);
    auVar7 = pmovsxbw(auVar7,auVar7);
    auVar10 = pmovsxbw(auVar10,auVar10);
    do {
      auVar6._8_8_ = 0;
      auVar6._0_8_ = *(ulong *)((long)&param_3->field_0 + lVar5);
      auVar6 = pmovsxbw(auVar6,auVar6);
      auVar11 = pmovsxbw(in_XMM6,*(undefined8 *)((long)&param_3->field_0 + lVar5 + 0x10));
      auVar6 = pmaddwd(auVar6,auVar8);
      auVar11 = pmaddwd(auVar11,auVar7);
      auVar14._8_8_ = 0;
      auVar14._0_8_ = *(ulong *)((long)&param_3->field_0 + lVar5 + 8);
      auVar13._8_8_ = 0;
      auVar13._0_8_ = *(ulong *)((long)&param_3->field_0 + lVar5 + 0x18);
      auVar15 = pmovsxbw(auVar14,auVar14);
      auVar14 = pmaddwd(auVar15,auVar10);
      auVar13 = pmovsxbw(auVar13,auVar13);
      auVar15._8_8_ = 0;
      auVar15._0_8_ = *(ulong *)((long)&param_3->field_0 + lVar5 + 0x20);
      auVar18._8_8_ = 0;
      auVar18._0_8_ = *(ulong *)((long)&param_3->field_0 + lVar5 + 0x28);
      auVar13 = pmaddwd(auVar13,auVar9);
      auVar16._8_8_ = 0;
      auVar16._0_8_ = *(ulong *)((long)&param_3->field_0 + lVar5 + 0x30);
      auVar17._8_8_ = 0;
      auVar17._0_8_ = *(ulong *)((long)&param_3->field_0 + lVar5 + 0x38);
      auVar15 = pmovsxbw(auVar15,auVar15);
      auVar18 = pmovsxbw(auVar18,auVar18);
      lVar5 = lVar5 + 0x40;
      auVar15 = pmaddwd(auVar15,auVar8);
      auVar18 = pmaddwd(auVar18,auVar10);
      auVar16 = pmovsxbw(auVar16,auVar16);
      auVar17 = pmovsxbw(auVar17,auVar17);
      auVar16 = pmaddwd(auVar16,auVar7);
      auVar17 = pmaddwd(auVar17,auVar9);
      iVar12 = auVar15._8_4_ + auVar16._8_4_ + auVar18._8_4_ + auVar17._8_4_;
      in_XMM6._12_4_ = auVar15._12_4_ + auVar16._12_4_ + auVar18._12_4_ + auVar17._12_4_;
      in_XMM6._4_4_ = auVar15._4_4_ + auVar16._4_4_ + auVar18._4_4_ + auVar17._4_4_ + in_XMM6._12_4_
      ;
      in_XMM6._0_4_ =
           auVar15._0_4_ + auVar16._0_4_ + auVar18._0_4_ + auVar17._0_4_ + iVar12 + in_XMM6._4_4_;
      in_XMM6._8_4_ = iVar12 + in_XMM6._12_4_;
      piVar1 = (int *)((long)&param_1->field_0 + uVar4 * 8 + lVar2);
      *piVar1 = *piVar1 + auVar6._0_4_ + auVar11._0_4_ + auVar14._0_4_ + auVar13._0_4_ +
                          auVar6._8_4_ + auVar11._8_4_ + auVar14._8_4_ + auVar13._8_4_ +
                          auVar6._4_4_ + auVar11._4_4_ + auVar14._4_4_ + auVar13._4_4_ +
                          auVar6._12_4_ + auVar11._12_4_ + auVar14._12_4_ + auVar13._12_4_;
      piVar1 = (int *)((long)&param_1->field_0 + uVar4 * 8 + lVar2 + 4);
      *piVar1 = *piVar1 + in_XMM6._0_4_;
      uVar4 = uVar4 + 1;
    } while (uVar4 < 4);
    bVar3 = bVar3 + 1;
    lVar2 = lVar2 + 0x20;
  } while (bVar3 < 8);
  return;
}

