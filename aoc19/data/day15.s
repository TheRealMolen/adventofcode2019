# IntProc disassembly
#


<main>:
0000  in  command
0002  seq command, 1, t
0006  jnz t, <go_north>
0009  seq command, 2, t
0013  jnz t, <go_south>
0016  seq command, 3, t
0020  jnz t, <go_west>
0023  seq command, 4, t
0027  jnz t, <go_east>
0030  hlt

<go_north>:
0031  mov curr_x, new_x   # mul curr_x, 1, new_x
0035  mov curr_x_odd, new_x_odd   # mul 1, curr_x_odd, new_x_odd
0039  add curr_y, -1, new_y
0043  seq curr_y_odd, 0, new_y_odd
0047  mul -1, new_y_odd, t
0051  add curr_halfy, t, new_halfy
0055  jmp <do_move>   # jnz 1, <do_move>

<go_south>:
0058  mov curr_x, new_x   # add 0, curr_x, new_x
0062  mov curr_x_odd, new_x_odd   # mul 1, curr_x_odd, new_x_odd
0066  add curr_y, 1, new_y
0070  seq curr_y_odd, 0, new_y_odd
0074  add curr_halfy, curr_y_odd, new_halfy
0078  jmp <do_move>   # jz  0, <do_move>

<go_west>:
0081  add curr_x, -1, new_x
0085  seq curr_x_odd, 0, new_x_odd
0089  mov curr_y, new_y   # mul curr_y, 1, new_y
0093  mov curr_y_odd, new_y_odd   # add curr_y_odd, 0, new_y_odd
0097  mov curr_halfy, new_halfy   # add 0, curr_halfy, new_halfy
0101  jmp <do_move>   # jz  0, <do_move>

<go_east>:
0104  add curr_x, 1, new_x
0108  seq curr_x_odd, 0, new_x_odd
0112  mov curr_y, new_y   # add 0, curr_y, new_y
0116  mov curr_y_odd, new_y_odd   # mul 1, curr_y_odd, new_y_odd
0120  mov curr_halfy, new_halfy   # mul curr_halfy, 1, new_halfy

<do_move>:
0124  jz  new_x, <out_of_bounds>
0127  jz  new_y, <out_of_bounds>
0130  seq new_x, 40, t
0134  jnz t, <out_of_bounds>
0137  seq new_y, 40, t
0141  jnz t, <out_of_bounds>
0144  seq new_x, 35, t
0148  jz  t, <check_tile>
0151  seq new_y, 9, t
0155  jz  t, <check_tile>
0158  mov 2, result   # add 0, 2, result
0162  jmp <end_move>   # jnz 1, <end_move>

<check_tile>:
0165  mul new_x_odd, new_y_odd, t
0169  jz  t, 179
0172  mov 1, result   # mul 1, 1, result
0176  jmp <end_move>   # jnz 1, <end_move>
0179  add new_x_odd, new_y_odd, t
0183  jz  t, <out_of_bounds>
0186  add new_halfy, new_y_odd, t
0190  add t, -1, t
0194  mul t, 39, t
0198  add t, new_x, t
0202  add -1, t, t
0206  add 252, t, @211
0210  slt @0, 26, result
0214  jmp <end_move>   # jnz 1, <end_move>

<out_of_bounds>:
0217  mov 0, result   # add 0, 0, result
0221  jmp <end_move>   # jz  0, <end_move>

<end_move>:
0224  jz  result, <output_result>
0227  mov new_x, curr_x   # mul 1, new_x, curr_x
0231  mov new_y, curr_y   # add 0, new_y, curr_y
0235  mov new_x_odd, curr_x_odd   # mul 1, new_x_odd, curr_x_odd
0239  mov new_y_odd, curr_y_odd   # mul new_y_odd, 1, curr_y_odd
0243  mov new_halfy, curr_halfy   # add new_halfy, 0, curr_halfy

<output_result>:
0247  out result
0249  jmp <main>   # jz  0, <main>
0252    .data 22
0253    .data 11
0254    .data 19
0255    .data 72
0256    .data 14
0257    .data 9
0258    .data 6
0259    .data 73
0260    .data 82
0261    .data 17
0262    .data 41
0263    .data 18
0264    .data 83
0265    .data 18
0266    .data 49
0267    .data 19
0268    .data 12
0269    .data 14
0270    .data 39
0271    .data 17
0272    .data 20
0273    .data 69
0274    .data 20
0275    .data 12
0276    .data 48
0277    .data 8
0278    .data 8
0279    .data 59
0280    .data 36
0281    .data 7
0282    .data 33
0283    .data 1
0284    .data 15
0285    .data 13
0286    .data 10
0287    .data 46
0288    .data 96
0289    .data 15
0290    .data 2
0291    .data 22
0292    .data 80
0293    .data 99
0294    .data 12
0295    .data 68
0296    .data 99
0297    .data 79
0298    .data 22
0299    .data 84
0300    .data 16
0301    .data 45
0302    .data 25
0303    .data 51
0304    .data 4
0305    .data 20
0306    .data 95
0307    .data 4
0308    .data 51
0309    .data 43
0310    .data 13
0311    .data 89
0312    .data 2
0313    .data 91
0314    .data 48
0315    .data 2
0316    .data 46
0317    .data 55
0318    .data 24
0319    .data 84
0320    .data 8
0321    .data 88
0322    .data 10
0323    .data 98
0324    .data 46
0325    .data 57
0326    .data 15
0327    .data 27
0328    .data 7
0329    .data 1
0330    .data 19
0331    .data 20
0332    .data 63
0333    .data 24
0334    .data 50
0335    .data 13
0336    .data 63
0337    .data 13
0338    .data 59
0339    .data 19
0340    .data 13
0341    .data 53
0342    .data 75
0343    .data 8
0344    .data 20
0345    .data 8
0346    .data 44
0347    .data 44
0348    .data 21
0349    .data 5
0350    .data 11
0351    .data 76
0352    .data 9
0353    .data 21
0354    .data 2
0355    .data 11
0356    .data 27
0357    .data 61
0358    .data 6
0359    .data 12
0360    .data 72
0361    .data 22
0362    .data 40
0363    .data 11
0364    .data 9
0365    .data 50
0366    .data 18
0367    .data 2
0368    .data 38
0369    .data 21
0370    .data 78
0371    .data 18
0372    .data 13
0373    .data 99
0374    .data 9
0375    .data 74
0376    .data 5
0377    .data 22
0378    .data 30
0379    .data 35
0380    .data 5
0381    .data 16
0382    .data 34
0383    .data 91
0384    .data 55
0385    .data 4
0386    .data 19
0387    .data 28
0388    .data 42
0389    .data 21
0390    .data 62
0391    .data 12
0392    .data 74
0393    .data 94
0394    .data 16
0395    .data 40
0396    .data 2
0397    .data 95
0398    .data 54
0399    .data 21
0400    .data 2
0401    .data 23
0402    .data 56
0403    .data 34
0404    .data 9
0405    .data 49
0406    .data 47
0407    .data 14
0408    .data 39
0409    .data 9
0410    .data 65
0411    .data 35
0412    .data 53
0413    .data 23
0414    .data 25
0415    .data 68
0416    .data 15
0417    .data 95
0418    .data 25
0419    .data 70
0420    .data 27
0421    .data 3
0422    .data 33
0423    .data 2
0424    .data 31
0425    .data 17
0426    .data 40
0427    .data 60
0428    .data 24
0429    .data 94
0430    .data 34
0431    .data 6
0432    .data 99
0433    .data 9
0434    .data 92
0435    .data 1
0436    .data 92
0437    .data 7
0438    .data 49
0439    .data 32
0440    .data 8
0441    .data 46
0442    .data 47
0443    .data 13
0444    .data 37
0445    .data 15
0446    .data 11
0447    .data 2
0448    .data 15
0449    .data 24
0450    .data 8
0451    .data 73
0452    .data 8
0453    .data 21
0454    .data 64
0455    .data 19
0456    .data 74
0457    .data 24
0458    .data 5
0459    .data 60
0460    .data 9
0461    .data 21
0462    .data 47
0463    .data 12
0464    .data 12
0465    .data 72
0466    .data 18
0467    .data 39
0468    .data 90
0469    .data 16
0470    .data 6
0471    .data 85
0472    .data 13
0473    .data 71
0474    .data 19
0475    .data 14
0476    .data 24
0477    .data 2
0478    .data 65
0479    .data 11
0480    .data 51
0481    .data 9
0482    .data 19
0483    .data 23
0484    .data 34
0485    .data 12
0486    .data 9
0487    .data 88
0488    .data 77
0489    .data 17
0490    .data 6
0491    .data 72
0492    .data 19
0493    .data 79
0494    .data 39
0495    .data 19
0496    .data 21
0497    .data 95
0498    .data 87
0499    .data 24
0500    .data 91
0501    .data 53
0502    .data 7
0503    .data 29
0504    .data 20
0505    .data 25
0506    .data 11
0507    .data 39
0508    .data 38
0509    .data 24
0510    .data 72
0511    .data 6
0512    .data 1
0513    .data 97
0514    .data 15
0515    .data 87
0516    .data 11
0517    .data 77
0518    .data 64
0519    .data 17
0520    .data 57
0521    .data 95
0522    .data 9
0523    .data 85
0524    .data 19
0525    .data 77
0526    .data 8
0527    .data 18
0528    .data 97
0529    .data 8
0530    .data 39
0531    .data 49
0532    .data 4
0533    .data 16
0534    .data 81
0535    .data 12
0536    .data 36
0537    .data 7
0538    .data 7
0539    .data 81
0540    .data 22
0541    .data 52
0542    .data 56
0543    .data 22
0544    .data 47
0545    .data 42
0546    .data 4
0547    .data 46
0548    .data 75
0549    .data 21
0550    .data 19
0551    .data 85
0552    .data 37
0553    .data 22
0554    .data 90
0555    .data 20
0556    .data 10
0557    .data 56
0558    .data 24
0559    .data 85
0560    .data 55
0561    .data 4
0562    .data 91
0563    .data 7
0564    .data 22
0565    .data 86
0566    .data 1
0567    .data 89
0568    .data 13
0569    .data 68
0570    .data 35
0571    .data 14
0572    .data 27
0573    .data 35
0574    .data 9
0575    .data 44
0576    .data 79
0577    .data 12
0578    .data 42
0579    .data 20
0580    .data 16
0581    .data 28
0582    .data 89
0583    .data 11
0584    .data 57
0585    .data 10
0586    .data 60
0587    .data 15
0588    .data 13
0589    .data 95
0590    .data 3
0591    .data 48
0592    .data 24
0593    .data 90
0594    .data 86
0595    .data 51
0596    .data 18
0597    .data 8
0598    .data 71
0599    .data 11
0600    .data 80
0601    .data 91
0602    .data 5
0603    .data 4
0604    .data 93
0605    .data 9
0606    .data 80
0607    .data 94
0608    .data 9
0609    .data 31
0610    .data 7
0611    .data 6
0612    .data 90
0613    .data 6
0614    .data 57
0615    .data 18
0616    .data 19
0617    .data 41
0618    .data 69
0619    .data 57
0620    .data 8
0621    .data 3
0622    .data 42
0623    .data 21
0624    .data 16
0625    .data 5
0626    .data 79
0627    .data 9
0628    .data 13
0629    .data 56
0630    .data 99
0631    .data 98
0632    .data 19
0633    .data 22
0634    .data 85
0635    .data 14
0636    .data 35
0637    .data 12
0638    .data 21
0639    .data 69
0640    .data 16
0641    .data 23
0642    .data 3
0643    .data 5
0644    .data 78
0645    .data 68
0646    .data 2
0647    .data 24
0648    .data 12
0649    .data 35
0650    .data 36
0651    .data 24
0652    .data 93
0653    .data 72
0654    .data 12
0655    .data 16
0656    .data 7
0657    .data 7
0658    .data 19
0659    .data 56
0660    .data 8
0661    .data 69
0662    .data 45
0663    .data 94
0664    .data 18
0665    .data 49
0666    .data 44
0667    .data 61
0668    .data 21
0669    .data 25
0670    .data 19
0671    .data 96
0672    .data 7
0673    .data 13
0674    .data 27
0675    .data 50
0676    .data 76
0677    .data 14
0678    .data 5
0679    .data 60
0680    .data 4
0681    .data 11
0682    .data 90
0683    .data 60
0684    .data 9
0685    .data 31
0686    .data 85
0687    .data 17
0688    .data 11
0689    .data 18
0690    .data 74
0691    .data 37
0692    .data 20
0693    .data 53
0694    .data 53
0695    .data 1
0696    .data 42
0697    .data 93
0698    .data 66
0699    .data 24
0700    .data 10
0701    .data 10
0702    .data 73
0703    .data 36
0704    .data 19
0705    .data 84
0706    .data 14
0707    .data 87
0708    .data 71
0709    .data 18
0710    .data 64
0711    .data 58
0712    .data 3
0713    .data 9
0714    .data 70
0715    .data 14
0716    .data 10
0717    .data 62
0718    .data 81
0719    .data 25
0720    .data 19
0721    .data 52
0722    .data 5
0723    .data 3
0724    .data 78
0725    .data 10
0726    .data 66
0727    .data 84
0728    .data 84
0729    .data 14
0730    .data 66
0731    .data 9
0732    .data 19
0733    .data 81
0734    .data 8
0735    .data 56
0736    .data 11
0737    .data 7
0738    .data 39
0739    .data 84
0740    .data 31
0741    .data 98
0742    .data 22
0743    .data 25
0744    .data 56
0745    .data 4
0746    .data 12
0747    .data 43
0748    .data 78
0749    .data 20
0750    .data 19
0751    .data 43
0752    .data 88
0753    .data 23
0754    .data 10
0755    .data 62
0756    .data 90
0757    .data 22
0758    .data 38
0759    .data 29
0760    .data 5
0761    .data 29
0762    .data 32
0763    .data 20
0764    .data 14
0765    .data 1
0766    .data 3
0767    .data 44
0768    .data 13
0769    .data 92
0770    .data 79
0771    .data 11
0772    .data 59
0773    .data 22
0774    .data 77
0775    .data 38
0776    .data 3
0777    .data 83
0778    .data 18
0779    .data 22
0780    .data 37
0781    .data 24
0782    .data 32
0783    .data 8
0784    .data 19
0785    .data 47
0786    .data 20
0787    .data 23
0788    .data 32
0789    .data 14
0790    .data 72
0791    .data 80
0792    .data 24
0793    .data 37
0794    .data 33
0795    .data 20
0796    .data 8
0797    .data 12
0798    .data 17
0799    .data 31
0800    .data 20
0801    .data 13
0802    .data 51
0803    .data 68
0804    .data 65
0805    .data 19
0806    .data 31
0807    .data 1
0808    .data 1
0809    .data 47
0810    .data 88
0811    .data 15
0812    .data 31
0813    .data 25
0814    .data 94
0815    .data 4
0816    .data 11
0817    .data 95
0818    .data 87
0819    .data 16
0820    .data 77
0821    .data 86
0822    .data 92
0823    .data 3
0824    .data 2
0825    .data 48
0826    .data 39
0827    .data 52
0828    .data 62
0829    .data 22
0830    .data 63
0831    .data 1
0832    .data 70
0833    .data 18
0834    .data 61
0835    .data 78
0836    .data 14
0837    .data 12
0838    .data 50
0839    .data 75
0840    .data 10
0841    .data 30
0842    .data 2
0843    .data 10
0844    .data 96
0845    .data 13
0846    .data 58
0847    .data 87
0848    .data 9
0849    .data 90
0850    .data 3
0851    .data 83
0852    .data 5
0853    .data 13
0854    .data 28
0855    .data 3
0856    .data 67
0857    .data 66
0858    .data 21
0859    .data 46
0860    .data 10
0861    .data 1
0862    .data 70
0863    .data 64
0864    .data 8
0865    .data 10
0866    .data 50
0867    .data 13
0868    .data 22
0869    .data 93
0870    .data 3
0871    .data 58
0872    .data 13
0873    .data 58
0874    .data 2
0875    .data 69
0876    .data 1
0877    .data 44
0878    .data 2
0879    .data 18
0880    .data 22
0881    .data 61
0882    .data 61
0883    .data 25
0884    .data 36
0885    .data 20
0886    .data 7
0887    .data 31
0888    .data 6
0889    .data 2
0890    .data 7
0891    .data 29
0892    .data 2
0893    .data 27
0894    .data 22
0895    .data 93
0896    .data 16
0897    .data 25
0898    .data 8
0899    .data 79
0900    .data 93
0901    .data 22
0902    .data 2
0903    .data 29
0904    .data 27
0905    .data 12
0906    .data 56
0907    .data 48
0908    .data 34
0909    .data 6
0910    .data 40
0911    .data 14
0912    .data 13
0913    .data 8
0914    .data 14
0915    .data 2
0916    .data 8
0917    .data 64
0918    .data 32
0919    .data 19
0920    .data 18
0921    .data 99
0922    .data 22
0923    .data 83
0924    .data 83
0925    .data 79
0926    .data 16
0927    .data 84
0928    .data 58
0929    .data 22
0930    .data 88
0931    .data 19
0932    .data 31
0933    .data 18
0934    .data 35
0935    .data 18
0936    .data 31
0937    .data 85
0938    .data 20
0939    .data 30
0940    .data 16
0941    .data 75
0942    .data 16
0943    .data 46
0944    .data 16
0945    .data 65
0946    .data 16
0947    .data 3
0948    .data 44
0949    .data 6
0950    .data 2
0951    .data 65
0952    .data 97
0953    .data 24
0954    .data 40
0955    .data 20
0956    .data 25
0957    .data 31
0958    .data 88
0959    .data 14
0960    .data 66
0961    .data 20
0962    .data 13
0963    .data 11
0964    .data 76
0965    .data 18
0966    .data 43
0967    .data 67
0968    .data 13
0969    .data 92
0970    .data 47
0971    .data 9
0972    .data 81
0973    .data 78
0974    .data 20
0975    .data 51
0976    .data 12
0977    .data 7
0978    .data 43
0979    .data 17
0980    .data 24
0981    .data 99
0982    .data 14
0983    .data 4
0984    .data 89
0985    .data 13
0986    .data 84
0987    .data 48
0988    .data 13
0989    .data 60
0990    .data 13
0991    .data 51
0992    .data 23
0993    .data 66
0994    .data 7
0995    .data 61
0996    .data 19
0997    .data 91
0998    .data 17
0999    .data 72
1000    .data 64
1001    .data 48
1002    .data 10
1003    .data 74
1004    .data 13
1005    .data 85
1006    .data 8
1007    .data 76
1008    .data 11
1009    .data 72
1010    .data 3
1011    .data 32
1012    .data 22
1013    .data 37
1014    .data 80
1015    .data 44
1016    .data 18
1017    .data 86
1018    .data 50
1019    .data 71
1020    .data 5
1021    .data 36
1022    .data 21
1023    .data 76
1024    .data 23
1025    .data 64
1026    .data 23
1027    .data 61
1028    .data 40
1029    .data 62
1030    .data 24
1031    .data 61
1032    t: 0
1033    command: 0
1034    curr_x: 21
1035    curr_y: 21
1036    curr_x_odd: 1
1037    curr_halfy: 10
1038    curr_y_odd: 1
1039    new_x: 0
1040    new_y: 0
1041    new_x_odd: 0
1042    new_halfy: 0
1043    new_y_odd: 0
1044    result: 0
