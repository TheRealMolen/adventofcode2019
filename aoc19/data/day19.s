# IntProc disassembly
#


<main>:
0000  arb 424
0002  in  rel + @1
0004  mov 11, rel + @0   # add 11, 0, rel + @0
0008  jmp <handle_coord_oob>   # jnz 1, <handle_coord_oob>
0011  mov 18, rel + @0   # add 0, 18, rel + @0
0015  jmp 259   # jz  0, 259
0018  mov rel + @1, @221   # mul rel + @1, 1, @221
0022  in  rel + @1
0024  mov 31, rel + @0   # add 0, 31, rel + @0
0028  jmp <handle_coord_oob>   # jnz 1, <handle_coord_oob>
0031  mov 38, rel + @0   # mul 1, 38, rel + @0
0035  jmp 259   # jz  0, 259
0038  mov @23, rel + @2   # add 0, @23, rel + @2
0042  mov rel + @1, rel + @3   # mul 1, rel + @1, rel + @3
0046  mov 1, rel + @1   # add 1, 0, rel + @1
0050  mov 57, rel + @0   # add 0, 57, rel + @0
0054  jmp 303   # jz  0, 303
0057  mov rel + @1, @222   # mul rel + @1, 1, @222
0061  mov @221, rel + @3   # mul @221, 1, rel + @3
0065  mov @221, rel + @2   # add @221, 0, rel + @2
0069  mov 259, rel + @1   # mul 1, 259, rel + @1
0073  mov 80, rel + @0   # add 80, 0, rel + @0
0077  jmp 225   # jnz 1, 225
0080  mov 117, rel + @2   # mul 1, 117, rel + @2
0084  mov 91, rel + @0   # mul 1, 91, rel + @0
0088  jmp 303   # jnz 1, 303
0091  mov rel + @1, @223   # mul rel + @1, 1, @223
0095  mov @222, rel + @4   # mul 1, @222, rel + @4
0099  mov 259, rel + @3   # add 0, 259, rel + @3
0103  mov 225, rel + @2   # add 0, 225, rel + @2
0107  mov 225, rel + @1   # add 225, 0, rel + @1
0111  mov 118, rel + @0   # add 118, 0, rel + @0
0115  jmp 225   # jnz 1, 225
0118  mov @222, rel + @3   # add @222, 0, rel + @3
0122  mov 20, rel + @2   # add 20, 0, rel + @2
0126  mov 133, rel + @0   # mul 1, 133, rel + @0
0130  jmp 303   # jnz 1, 303
0133  mul rel + @1, -1, rel + @1
0137  add @223, rel + @1, rel + @1
0141  mov 148, rel + @0   # add 0, 148, rel + @0
0145  jmp 259   # jz  0, 259
0148  mov rel + @1, @223   # add 0, rel + @1, @223
0152  mov @221, rel + @4   # mul 1, @221, rel + @4
0156  mov @222, rel + @3   # add @222, 0, rel + @3
0160  mov 16, rel + @2   # add 0, 16, rel + @2
0164  add @132, -2, @224
0168  mul @224, 2, @224
0172  add @224, 3, @224
0176  mul @132, -1, @132
0180  add @224, @132, @224
0184  add @224, 1, rel + @1
0188  mov 195, rel + @0   # mul 195, 1, rel + @0
0192  jmp @108   # jnz 1, @108
0195  slt rel + @1, @223, rel + @2
0199  mov @23, rel + @1   # mul @23, 1, rel + @1
0203  mov -1, rel + @3   # mul -1, 1, rel + @3
0207  mov 214, rel + @0   # add 0, 214, rel + @0
0211  jmp 303   # jnz 1, 303
0214  add 1, rel + @1, rel + @1
0218  out rel + @1
0220  hlt
0221    .data 0
0222    .data 0
0223    .data 0
0224    .data 0
0225  arb 5
0227  mov rel + @-4, @249   # add rel + @-4, 0, @249
0231  mov rel + @-3, rel + @1   # mul 1, rel + @-3, rel + @1
0235  mov rel + @-2, rel + @2   # add 0, rel + @-2, rel + @2
0239  mov rel + @-1, rel + @3   # mul rel + @-1, 1, rel + @3
0243  mov 250, rel + @0   # mul 1, 250, rel + @0
0247  jmp 225   # jz  0, 225
0250  mov rel + @1, rel + @-4   # mul 1, rel + @1, rel + @-4
0254  arb -5
0256  jmp rel + <main>   # jnz 1, rel + <main>   # NOTE: unable to guess jump destination
0259  arb 3
0261  slt 0, rel + @-2, rel + @-1
0265  mul rel + @-1, 2, rel + @-1
0269  add rel + @-1, -1, rel + @-1
0273  mul rel + @-1, rel + @-2, rel + @-2
0277  arb -3
0279  jmp rel + <main>   # jz  0, rel + <main>   # NOTE: unable to guess jump destination

<handle_coord_oob>:
0282  arb 3
0284  slt rel + @-2, 0, rel + @-1
0288  jz  rel + @-1, 294
0291  out 0
0293  hlt
0294  mov rel + @-2, rel + @-2   # mul rel + @-2, 1, rel + @-2
0298  arb -3
0300  jmp rel + <main>   # jnz 1, rel + <main>   # NOTE: unable to guess jump destination
0303  arb 5
0305  slt rel + @-3, rel + @-4, rel + @-1
0309  jz  rel + @-1, 346
0312  add rel + @-4, rel + @-3, rel + @-4
0316  mul rel + @-3, -1, rel + @-1
0320  add rel + @-4, rel + @-1, rel + @2
0324  mul rel + @2, -1, rel + @-1
0328  add rel + @-4, rel + @-1, rel + @1
0332  mov rel + @-2, rel + @3   # add rel + @-2, 0, rel + @3
0336  mov 343, rel + @0   # add 343, 0, rel + @0
0340  jmp 303   # jnz 1, 303
0343  jmp 415   # jnz 1, 415
0346  slt rel + @-2, rel + @-3, rel + @-1
0350  jz  rel + @-1, 387
0353  add rel + @-3, rel + @-2, rel + @-3
0357  mul rel + @-2, -1, rel + @-1
0361  add rel + @-3, rel + @-1, rel + @3
0365  mul rel + @3, -1, rel + @-1
0369  add rel + @-3, rel + @-1, rel + @2
0373  mov rel + @-4, rel + @1   # add rel + @-4, 0, rel + @1
0377  mov 384, rel + @0   # add 0, 384, rel + @0
0381  jmp 303   # jnz 1, 303
0384  jmp 415   # jnz 1, 415
0387  mul rel + @-4, -1, rel + @-4
0391  add rel + @-4, rel + @-3, rel + @-4
0395  mul rel + @-3, rel + @-2, rel + @-2
0399  mul rel + @-2, rel + @-4, rel + @-4
0403  mul rel + @-3, rel + @-2, rel + @-3
0407  mul rel + @-4, -1, rel + @-2
0411  add rel + @-3, rel + @-2, rel + @1
0415  mov rel + @1, rel + @-4   # add 0, rel + @1, rel + @-4
0419  arb -5
0421  jmp rel + <main>   # jnz 1, rel + <main>   # NOTE: unable to guess jump destination
