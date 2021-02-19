# CSAPP-Labs

## Current Status
- [x] Data Lab
- [x] Bomb Lab

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