# IntProc disassembly
#


<main>:
0000  add const_zero, const_one, use_intro_mode
0004  arb 3080
0006  mov 1182, @15   # add 0, 1182, @15
0010  mov bss, @24   # add 0, bss, @24
0014  mov @0, tmp   # add @0, 0, tmp
0018  jz  tmp, 36
0021  mov flipflip_pc36, @0   # mul flipflip_pc36, 1, @0
0025  add tmp, -1, tmp
0029  add @24, 1, @24
0033  jmp 18   # jnz 1, 18
0036  seq flipflip_pc36, 0, flipflip_pc36
0040  add @15, 1, @15
0044  seq @15, bss, tmp
0048  jz  tmp, 14
0051  mov 58, rel + @0   # add 58, 0, rel + @0
0055  jmp 786   # jnz 1, 786
0058  jz  use_intro_mode, 62
0061  hlt
0062  mov 333, rel + @1   # mul 333, 1, rel + @1
0066  mov 73, rel + @0   # add 0, 73, rel + @0
0070  jmp <execute>   # jnz 1, <execute>
0073  mov 0, @572   # mul 0, 1, @572
0077  mov 0, @573   # add 0, 0, @573
0081  in  @574
0083  add 1, @573, @573
0087  slt @574, 65, tmp
0091  jnz tmp, 151
0094  slt 67, @574, tmp
0098  jnz tmp, 151
0101  add @574, -64, @574
0105  mul @574, -1, @574
0109  add @572, 1, @572
0113  slt @572, 11, tmp
0117  jz  tmp, 165
0120  add 1182, @572, @127
0124  mov @574, @0   # add 0, @574, @0
0128  in  @574
0130  add 1, @573, @573
0134  seq @574, 10, tmp
0138  jnz tmp, 189
0141  seq @574, 44, tmp
0145  jz  tmp, 158
0148  jmp 81   # jz  0, 81
0151  mov 340, rel + @1   # mul 1, 340, rel + @1
0155  jmp 177   # jnz 1, 177
0158  mov 477, rel + @1   # mul 477, 1, rel + @1
0162  jmp 177   # jnz 1, 177
0165  mov 514, rel + @1   # mul 1, 514, rel + @1
0169  mov 176, rel + @0   # mul 1, 176, rel + @0
0173  jmp <execute>   # jz  0, <execute>
0176  hlt
0177  mov 184, rel + @0   # add 184, 0, rel + @0
0181  jmp <execute>   # jnz 1, <execute>
0184  out @574
0186  out 10
0188  hlt
0189  slt @573, 22, tmp
0193  jz  tmp, 165
0196  mov @572, @1182   # mul @572, 1, @1182
0200  mov 375, rel + @1   # add 0, 375, rel + @1
0204  mov 211, rel + @0   # mul 211, 1, rel + @0
0208  jmp <execute>   # jnz 1, <execute>
0211  add 1182, 11, rel + @1
0215  mov 222, rel + @0   # mul 1, 222, rel + @0
0219  jmp 979   # jz  0, 979
0222  mov 388, rel + @1   # add 0, 388, rel + @1
0226  mov 233, rel + @0   # mul 233, 1, rel + @0
0230  jmp <execute>   # jnz 1, <execute>
0233  add 1182, 22, rel + @1
0237  mov 244, rel + @0   # add 244, 0, rel + @0
0241  jmp 979   # jnz 1, 979
0244  mov 401, rel + @1   # add 0, 401, rel + @1
0248  mov 255, rel + @0   # add 255, 0, rel + @0
0252  jmp <execute>   # jz  0, <execute>
0255  add 1182, 33, rel + @1
0259  mov 266, rel + @0   # add 266, 0, rel + @0
0263  jmp 979   # jz  0, 979
0266  mov 414, rel + @1   # add 0, 414, rel + @1
0270  mov 277, rel + @0   # mul 1, 277, rel + @0
0274  jmp <execute>   # jnz 1, <execute>
0277  in  continuous_video_enable
0279  seq continuous_video_enable, 89, tmp
0283  seq continuous_video_enable, 121, continuous_video_enable
0287  add continuous_video_enable, tmp, continuous_video_enable
0291  in  @574
0293  seq @574, 10, tmp
0297  jz  tmp, 291
0300  out 10
0302  mov 1182, rel + @1   # add 1182, 0, rel + @1
0306  mov 313, rel + @0   # add 313, 0, rel + @0
0310  jmp 622   # jnz 1, 622
0313  jnz continuous_video_enable, 327
0316  mov 1, continuous_video_enable   # add 1, 0, continuous_video_enable
0320  mov 327, rel + @0   # add 327, 0, rel + @0
0324  jmp 786   # jz  0, 786
0327  out @438
0329  hlt
0330    const_zero: 0
0331    const_one: 1
0332    use_intro_mode: 1
0333    .data 6
0334    .data 77
0335    .data 97
0336    .data 105
0337    .data 110
0338    .data 58
0339    .data 10
0340    .data 33
0341    .data 10
0342    .data 69
0343    .data 120
0344    .data 112
0345    .data 101
0346    .data 99
0347    .data 116
0348    .data 101
0349    .data 100
0350    .data 32
0351    .data 102
0352    .data 117
0353    .data 110
0354    .data 99
0355    .data 116
0356    .data 105
0357    .data 111
0358    .data 110
0359    .data 32
0360    .data 110
0361    .data 97
0362    .data 109
0363    .data 101
0364    .data 32
0365    .data 98
0366    .data 117
0367    .data 116
0368    .data 32
0369    .data 103
0370    .data 111
0371    .data 116
0372    .data 58
0373    .data 32
0374    .data 0
0375    .data 12
0376    .data 70
0377    .data 117
0378    .data 110
0379    .data 99
0380    .data 116
0381    .data 105
0382    .data 111
0383    .data 110
0384    .data 32
0385    .data 65
0386    .data 58
0387    .data 10
0388    .data 12
0389    .data 70
0390    .data 117
0391    .data 110
0392    .data 99
0393    .data 116
0394    .data 105
0395    .data 111
0396    .data 110
0397    .data 32
0398    .data 66
0399    .data 58
0400    .data 10
0401    .data 12
0402    .data 70
0403    .data 117
0404    .data 110
0405    .data 99
0406    .data 116
0407    .data 105
0408    .data 111
0409    .data 110
0410    .data 32
0411    .data 67
0412    .data 58
0413    .data 10
0414    .data 23
0415    .data 67
0416    .data 111
0417    .data 110
0418    .data 116
0419    .data 105
0420    .data 110
0421    .data 117
0422    .data 111
0423    .data 117
0424    .data 115
0425    .data 32
0426    .data 118
0427    .data 105
0428    .data 100
0429    .data 101
0430    .data 111
0431    .data 32
0432    .data 102
0433    .data 101
0434    .data 101
0435    .data 100
0436    .data 63
0437    .data 10
0438    .data 0
0439    .data 37
0440    .data 10
0441    .data 69
0442    .data 120
0443    .data 112
0444    .data 101
0445    .data 99
0446    .data 116
0447    .data 101
0448    .data 100
0449    .data 32
0450    .data 82
0451    .data 44
0452    .data 32
0453    .data 76
0454    .data 44
0455    .data 32
0456    .data 111
0457    .data 114
0458    .data 32
0459    .data 100
0460    .data 105
0461    .data 115
0462    .data 116
0463    .data 97
0464    .data 110
0465    .data 99
0466    .data 101
0467    .data 32
0468    .data 98
0469    .data 117
0470    .data 116
0471    .data 32
0472    .data 103
0473    .data 111
0474    .data 116
0475    .data 58
0476    .data 32
0477    .data 36
0478    .data 10
0479    .data 69
0480    .data 120
0481    .data 112
0482    .data 101
0483    .data 99
0484    .data 116
0485    .data 101
0486    .data 100
0487    .data 32
0488    .data 99
0489    .data 111
0490    .data 109
0491    .data 109
0492    .data 97
0493    .data 32
0494    .data 111
0495    .data 114
0496    .data 32
0497    .data 110
0498    .data 101
0499    .data 119
0500    .data 108
0501    .data 105
0502    .data 110
0503    .data 101
0504    .data 32
0505    .data 98
0506    .data 117
0507    .data 116
0508    .data 32
0509    .data 103
0510    .data 111
0511    .data 116
0512    .data 58
0513    .data 32
0514    .data 43
0515    .data 10
0516    .data 68
0517    .data 101
0518    .data 102
0519    .data 105
0520    .data 110
0521    .data 105
0522    .data 116
0523    .data 105
0524    .data 111
0525    .data 110
0526    .data 115
0527    .data 32
0528    .data 109
0529    .data 97
0530    .data 121
0531    .data 32
0532    .data 98
0533    .data 101
0534    .data 32
0535    .data 97
0536    .data 116
0537    .data 32
0538    .data 109
0539    .data 111
0540    .data 115
0541    .data 116
0542    .data 32
0543    .data 50
0544    .data 48
0545    .data 32
0546    .data 99
0547    .data 104
0548    .data 97
0549    .data 114
0550    .data 97
0551    .data 99
0552    .data 116
0553    .data 101
0554    .data 114
0555    .data 115
0556    .data 33
0557    .data 10
0558    .data 94
0559    .data 62
0560    .data 118
0561    .data 60
0562    .data 0
0563    .data 1
0564    .data 0
0565    .data -1
0566    .data -1
0567    .data 0
0568    .data 1
0569    .data 0
0570    tmp: 0
0571    flipflip_pc36: 0
0572    .data 0
0573    .data 0
0574    .data 0
0575    continuous_video_enable: 1
0576    .data 20
0577    .data 26
0578    .data 0

<execute>:
0579  arb 4
0581  mov rel + @-3, @587   # mul rel + @-3, 1, @587
0585  mov @0, rel + @-1   # mul 1, @0, rel + @-1
0589  add 1, rel + @-3, rel + @-3
0593  mov 0, rel + @-2   # add 0, 0, rel + @-2
0597  seq rel + @-2, rel + @-1, tmp
0601  jnz tmp, 617
0604  add rel + @-3, rel + @-2, @609
0608  out @0
0610  add rel + @-2, 1, rel + @-2
0614  jmp 597   # jz  0, 597
0617  arb -4
0619  jmp rel + <main>   # jz  0, rel + <main>   # NOTE: unable to guess jump destination
0622  arb 5
0624  mov rel + @-4, @630   # mul rel + @-4, 1, @630
0628  mov @0, rel + @-2   # add 0, @0, rel + @-2
0632  add 1, rel + @-4, rel + @-4
0636  mov 0, rel + @-3   # mul 0, 1, rel + @-3
0640  seq rel + @-3, rel + @-2, tmp
0644  jnz tmp, 781
0647  add rel + @-4, rel + @-3, @653
0651  mov @0, rel + @-1   # mul 1, @0, rel + @-1
0655  seq rel + @-1, -4, tmp
0659  jnz tmp, 709
0662  seq rel + @-1, -5, tmp
0666  jnz tmp, 734
0669  slt rel + @-1, 0, tmp
0673  jnz tmp, 759
0676  jz  rel + @-1, 774
0679  add @578, 562, @684
0683  add @0, @576, @576
0687  add @578, 566, @692
0691  add @0, @577, @577
0695  mov 702, rel + @0   # add 0, 702, rel + @0
0699  jmp 786   # jnz 1, 786
0702  add rel + @-1, -1, rel + @-1
0706  jmp 676   # jz  0, 676
0709  add @578, 1, @578
0713  seq @578, 4, tmp
0717  jz  tmp, 724
0720  add @578, -4, @578
0724  mov 731, rel + @0   # mul 1, 731, rel + @0
0728  jmp 786   # jz  0, 786
0731  jmp 774   # jnz 1, 774
0734  add @578, -1, @578
0738  seq @578, -1, tmp
0742  jz  tmp, 749
0745  add @578, 4, @578
0749  mov 756, rel + @0   # add 756, 0, rel + @0
0753  jmp 786   # jnz 1, 786
0756  jmp 774   # jz  0, 774
0759  mul rel + @-1, -11, rel + @1
0763  add 1182, rel + @1, rel + @1
0767  mov 774, rel + @0   # add 774, 0, rel + @0
0771  jmp 622   # jnz 1, 622
0774  add rel + @-3, 1, rel + @-3
0778  jmp 640   # jz  0, 640
0781  arb -5
0783  jmp rel + <main>   # jnz 1, rel + <main>   # NOTE: unable to guess jump destination
0786  arb 7
0788  jnz continuous_video_enable, 802
0791  mov @576, rel + @-6   # add 0, @576, rel + @-6
0795  mov @577, rel + @-5   # mul @577, 1, rel + @-5
0799  jmp 814   # jz  0, 814
0802  mov 0, rel + @-1   # mul 0, 1, rel + @-1
0806  mov 0, rel + @-5   # add 0, 0, rel + @-5
0810  mov 0, rel + @-6   # add 0, 0, rel + @-6
0814  seq rel + @-6, @576, rel + @-2
0818  seq rel + @-5, @577, tmp
0822  mul tmp, rel + @-2, rel + @-2
0826  mul rel + @-5, 43, rel + @-3
0830  add rel + @-6, rel + @-3, rel + @-3
0834  add bss, rel + @-3, rel + @-3
0838  mov rel + @-3, @843   # mul 1, rel + @-3, @843
0842  jnz @0, 863
0845  mul rel + @-2, 42, rel + @-4
0849  add 46, rel + @-4, rel + @-4
0853  jz  rel + @-2, 924
0856  mov 1, rel + @-1   # add 0, 1, rel + @-1
0860  jmp 924   # jz  0, 924
0863  jnz rel + @-2, 873
0866  mov 35, rel + @-4   # add 35, 0, rel + @-4
0870  jmp 924   # jz  0, 924
0873  mov rel + @-3, @878   # add rel + @-3, 0, @878
0877  seq @0, 1, tmp
0881  jz  tmp, 916
0884  add @374, 1, @374
0888  mov rel + @-3, @895   # add rel + @-3, 0, @895
0892  mov 2, @0   # add 2, 0, @0
0896  mov rel + @-3, @902   # add rel + @-3, 0, @902
0900  mov @438, @438   # add @438, 0, @438
0904  mul rel + @-6, rel + @-5, tmp
0908  add tmp, @374, tmp
0912  add tmp, @438, @438
0916  add @578, 558, @922
0920  mov @0, rel + @-4   # mul 1, @0, rel + @-4
0924  jz  continuous_video_enable, 959
0927  out rel + @-4
0929  add 1, rel + @-6, rel + @-6
0933  seq rel + @-6, 43, tmp
0937  jz  tmp, 814
0940  out 10
0942  add 1, rel + @-5, rel + @-5
0946  seq rel + @-5, 39, tmp
0950  jz  tmp, 810
0953  out 10
0955  jz  rel + @-1, 974
0958  hlt
0959  jz  rel + @-1, 974
0962  mov 1, continuous_video_enable   # add 0, 1, continuous_video_enable
0966  mov 973, rel + @0   # add 0, 973, rel + @0
0970  jmp 786   # jnz 1, 786
0973  hlt
0974  arb -7
0976  jmp rel + <main>   # jnz 1, rel + <main>   # NOTE: unable to guess jump destination
0979  arb 6
0981  mov 0, rel + @-4   # add 0, 0, rel + @-4
0985  mov 0, rel + @-3   # add 0, 0, rel + @-3
0989  in  rel + @-2
0991  add 1, rel + @-3, rel + @-3
0995  seq rel + @-2, 82, rel + @-1
0999  jnz rel + @-1, 1030
1002  seq rel + @-2, 76, rel + @-1
1006  jnz rel + @-1, 1037
1009  slt rel + @-2, 48, rel + @-1
1013  jnz rel + @-1, 1124
1016  slt 57, rel + @-2, rel + @-1
1020  jnz rel + @-1, 1124
1023  add rel + @-2, -48, rel + @-2
1027  jmp 1041   # jz  0, 1041
1030  mov -4, rel + @-2   # mul 1, -4, rel + @-2
1034  jmp 1041   # jz  0, 1041
1037  mov -5, rel + @-2   # mul 1, -5, rel + @-2
1041  add rel + @-4, 1, rel + @-4
1045  slt rel + @-4, 11, rel + @-1
1049  jz  rel + @-1, 1138
1052  add rel + @-5, rel + @-4, @1059
1056  mov rel + @-2, @0   # add rel + @-2, 0, @0
1060  in  rel + @-2
1062  add 1, rel + @-3, rel + @-3
1066  slt rel + @-2, 48, rel + @-1
1070  jnz rel + @-1, 1107
1073  slt 57, rel + @-2, rel + @-1
1077  jnz rel + @-1, 1107
1080  add rel + @-2, -48, rel + @-2
1084  add rel + @-5, rel + @-4, @1090
1088  mul 10, @0, rel + @-1
1092  add rel + @-2, rel + @-1, rel + @-2
1096  add rel + @-5, rel + @-4, @1103
1100  mov rel + @-2, @0   # add 0, rel + @-2, @0
1104  jmp 1060   # jz  0, 1060
1107  seq rel + @-2, 10, rel + @-1
1111  jnz rel + @-1, 1162
1114  seq rel + @-2, 44, rel + @-1
1118  jz  rel + @-1, 1131
1121  jmp 989   # jnz 1, 989
1124  mov 439, rel + @1   # add 0, 439, rel + @1
1128  jmp 1150   # jz  0, 1150
1131  mov 477, rel + @1   # mul 1, 477, rel + @1
1135  jmp 1150   # jz  0, 1150
1138  mov 514, rel + @1   # add 514, 0, rel + @1
1142  mov 1149, rel + @0   # add 1149, 0, rel + @0
1146  jmp <execute>   # jz  0, <execute>
1149  hlt
1150  mov 1157, rel + @0   # add 1157, 0, rel + @0
1154  jmp <execute>   # jnz 1, <execute>
1157  out rel + @-2
1159  out 10
1161  hlt
1162  slt rel + @-3, 22, rel + @-1
1166  jz  rel + @-1, 1138
1169  mov rel + @-5, @1176   # add rel + @-5, 0, @1176
1173  mov rel + @-4, @0   # add 0, rel + @-4, @0
1177  arb -6
1179  jmp rel + <main>   # jnz 1, rel + <main>   # NOTE: unable to guess jump destination
1182    .data 28
1183    .data 5
1184    .data 38
1185    .data 1
1186    .data 3
1187    .data 1
1188    .data 38
1189    .data 1
1190    .data 3
1191    .data 1
1192    .data 38
1193    .data 1
1194    .data 3
1195    .data 1
1196    .data 38
1197    .data 1
1198    .data 3
1199    .data 1
1200    .data 38
1201    .data 1
1202    .data 3
1203    .data 1
1204    .data 34
1205    .data 9
1206    .data 34
1207    .data 1
1208    .data 3
1209    .data 1
1210    .data 38
1211    .data 1
1212    .data 3
1213    .data 1
1214    .data 38
1215    .data 1
1216    .data 3
1217    .data 1
1218    .data 34
1219    .data 9
1220    .data 34
1221    .data 1
1222    .data 3
1223    .data 1
1224    .data 38
1225    .data 1
1226    .data 3
1227    .data 1
1228    .data 38
1229    .data 1
1230    .data 3
1231    .data 1
1232    .data 34
1233    .data 5
1234    .data 3
1235    .data 5
1236    .data 30
1237    .data 1
1238    .data 11
1239    .data 1
1240    .data 30
1241    .data 1
1242    .data 11
1243    .data 1
1244    .data 30
1245    .data 1
1246    .data 11
1247    .data 1
1248    .data 22
1249    .data 9
1250    .data 11
1251    .data 5
1252    .data 18
1253    .data 1
1254    .data 23
1255    .data 1
1256    .data 18
1257    .data 1
1258    .data 23
1259    .data 1
1260    .data 18
1261    .data 1
1262    .data 23
1263    .data 1
1264    .data 10
1265    .data 7
1266    .data 1
1267    .data 1
1268    .data 19
1269    .data 9
1270    .data 6
1271    .data 1
1272    .data 5
1273    .data 1
1274    .data 1
1275    .data 1
1276    .data 19
1277    .data 1
1278    .data 3
1279    .data 1
1280    .data 3
1281    .data 1
1282    .data 6
1283    .data 1
1284    .data 5
1285    .data 1
1286    .data 1
1287    .data 5
1288    .data 15
1289    .data 1
1290    .data 3
1291    .data 1
1292    .data 3
1293    .data 1
1294    .data 6
1295    .data 1
1296    .data 5
1297    .data 1
1298    .data 5
1299    .data 1
1300    .data 15
1301    .data 1
1302    .data 3
1303    .data 1
1304    .data 3
1305    .data 1
1306    .data 6
1307    .data 11
1308    .data 1
1309    .data 1
1310    .data 1
1311    .data 7
1312    .data 3
1313    .data 5
1314    .data 3
1315    .data 11
1316    .data 6
1317    .data 1
1318    .data 3
1319    .data 1
1320    .data 1
1321    .data 1
1322    .data 1
1323    .data 1
1324    .data 9
1325    .data 1
1326    .data 11
1327    .data 1
1328    .data 5
1329    .data 1
1330    .data 6
1331    .data 1
1332    .data 1
1333    .data 9
1334    .data 7
1335    .data 1
1336    .data 11
1337    .data 1
1338    .data 5
1339    .data 1
1340    .data 6
1341    .data 1
1342    .data 1
1343    .data 1
1344    .data 1
1345    .data 1
1346    .data 1
1347    .data 1
1348    .data 1
1349    .data 1
1350    .data 9
1351    .data 1
1352    .data 11
1353    .data 1
1354    .data 5
1355    .data 1
1356    .data 6
1357    .data 9
1358    .data 1
1359    .data 9
1360    .data 11
1361    .data 7
1362    .data 8
1363    .data 1
1364    .data 1
1365    .data 1
1366    .data 1
1367    .data 1
1368    .data 3
1369    .data 1
1370    .data 30
1371    .data 9
1372    .data 3
1373    .data 1
1374    .data 30
1375    .data 1
1376    .data 3
1377    .data 1
1378    .data 1
1379    .data 1
1380    .data 5
1381    .data 1
1382    .data 30
1383    .data 1
1384    .data 3
1385    .data 1
1386    .data 1
1387    .data 7
1388    .data 30
1389    .data 1
1390    .data 3
1391    .data 1
1392    .data 38
1393    .data 1
1394    .data 3
1395    .data 1
1396    .data 38
1397    .data 1
1398    .data 3
1399    .data 1
1400    .data 38
1401    .data 5
1402    .data 34
