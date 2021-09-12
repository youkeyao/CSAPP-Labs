# CSAPP-Labs

## Current Status
- [x] Data Lab
- [x] Bomb Lab
- [x] Attack Lab
- [x] Architecture Lab
- [x] Cache Lab
- [x] Shell Lab
- [x] Malloc Lab

## Data Lab
| Points | Rating | Errors | Points | Ops |    Puzzle    |
| ------ | ------ | ------ | ------ | --- |    ------    |
|    1   |    1   |    0   |    2   |  8  |    bitXor    |
|    1   |    1   |    0   |    2   |  1  |     tmin     |
|    1   |    1   |    0   |    2   |  8  |    isTmax    |
|    2   |    2   |    0   |    2   |  7  |  allOddBits  |
|    2   |    2   |    0   |    2   |  2  |    negate    |
|    3   |    3   |    0   |    2   |  8  | isAsciiDigit |
|    3   |    3   |    0   |    2   |  7  |  conditional |
|    3   |    3   |    0   |    2   |  16 | isLessOrEqual|
|    4   |    4   |    0   |    2   |  8  |  logicalNeg  |
|    4   |    4   |    0   |    2   |  56 |  howManyBits |
|    4   |    4   |    0   |    2   |  8  |  floatScale2 |
|    4   |    4   |    0   |    2   |  15 |floatFloat2Int|
|    4   |    4   |    0   |    2   |  8  |  floatPower2 |

Score = 62/62 [36/36 Corr + 26/26 Perf] (152 total operators)

**Tip :** change the TIMEOUT_LIMIT in btest.c from 10 to 20 so that floatPower2 can pass.

## Bomb Lab
|   func    | answer |
| --------- | ------ |
| <phase_1> | Border relations with Canada have never been better. |
| <phase_2> | 1 2 4 8 16 32 |
| <phase_3> | 0 207 |
|           | 1 311 |
|           | 2 707 |
|           | 3 256 |
|           | 4 389 |
|           | 5 206 |
|           | 6 682 |
|           | 7 327 |
| <phase_4> | 7 0 |
|           | 3 0 |
|           | 1 0 |
|           | 0 0 |
| <phase_5> | ionefg |
|           | ionefw |
|           | ionevg |
|           | ionevw |
|           | ionufg |
|           | ionufw |
|           | ionuvg |
|           | ionuvw |
|           | yonefg |
|           | yonefw |
|           | yonevg |
|           | yonevw |
|           | yonufg |
|           | yonufw |
|           | yonuvg |
|           | yonuvw |
| <phase_6> | 4 3 2 1 6 5 |
| <secret_phase> | 22 |

To get into `<secret_phase>`, add "DrEvil" at the end in `<phase_4>`.

## Attack Lab
| target | answer |
| ------ | ------ |
| ctarget_level1 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 c0 17 40 00 00 00 00 00 |
| ctarget_level2 | 48 c7 c7 fa 97 b9 59 68 ec 17 40 00 c3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 dc 61 55 00 00 00 00 |
| ctarget_level3 | 48 c7 c7 a8 dc 61 55 68 fa 18 40 00 c3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 dc 61 55 00 00 00 00 35 39 62 39 39 37 66 61 00 |
| rtarget_level2 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ab 19 40 00 00 00 00 00 fa 97 b9 59 00 00 00 00 a2 19 40 00 00 00 00 00 ec 17 40 00 00 00 00 00 |
| rtarget_level3 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 06 1a 40 00 00 00 00 00 a2 19 40 00 00 00 00 00 ab 19 40 00 00 00 00 00 48 00 00 00 00 00 00 00 dd 19 40 00 00 00 00 00 70 1a 40 00 00 00 00 00 13 1a 40 00 00 00 00 00 d6 19 40 00 00 00 00 00 a2 19 40 00 00 00 00 00 fa 18 40 00 00 00 00 00 35 39 62 39 39 37 66 61 00 |

## Architecture Lab
| Part | Answer Path |
| ---- | ----------- |
| A | [archlab/sim/misc/answer](archlab/sim/misc/answer) |
| B | [archlab/sim/seq/seq-full.hcl](archlab/sim/seq/seq-full.hcl) |
| C | [archlab/sim/pipe/pipe-full.hcl](archlab/sim/pipe/pipe-full.hcl) |
|   | [archlab/sim/pipe/ncopy.ys](archlab/sim/pipe/ncopy.ys) |

Part C Score : 60.0/60.0

Average CPE : 7.50

**Tip :** 
- Modify some files to adapt to tcl8.6.
- In Part C I remove the `xorq %rax, %rax` because I really can't find anything else to optimize.

## Cache Lab
| Summary | Points | Max pts | Misses |
| ------- | ------ | ------- | ------ |
| Csim correctness | 27.0 | 27 | |
| Trans perf 32x32 | 8.0 | 8 | 287 |
| Trans perf 64x64 | 8.0 | 8 | 1171 |
| Trans perf 61x67 | 10.0 | 10 | 1992 |
| Total Points | 53.0 | 53 | |

## Shell Lab
Answer Path : [shlab/tsh.c](shlab/tsh.c)

## Malloc Lab
| trace | valid | util | ops | secs | Kops |
| ----- | ----- | ---- | --- | ---- | ---- |
| 0 | yes | 99% | 5694 | 0.000147 | 38787 |
| 1 | yes | 100% | 5848 | 0.000163 | 35899 |
| 2 | yes | 99% | 6648 | 0.000173 | 38494 |
| 3 | yes | 100% | 5380 | 0.000153 | 35072 |
| 4 | yes | 88% | 14400 | 0.000165 | 87114 |
| 5 | yes | 95% | 4800 | 0.000444 | 10811 |
| 6 | yes | 94% | 4800 | 0.000384 | 12500 |
| 7 | yes | 55% | 12000 | 0.001808 | 6637 |
| 8 | yes | 51% | 24000 | 0.000714 | 33623 |
| 9 | yes | 59% | 14401 | 0.000246 | 58469 |
| 10 | yes | 89% | 14401 | 0.000157 | 92019 |
| Total | | 85% | 112372 | 0.004554 | 24677 |

Perf index = 51 (util) + 40 (thru) = 91/100