# Binary-arithmetic-coding(QM-coder)
There are two parameters
A and C in the coder, where A records the size of the tag interval with a value between
0.75 (0x8000) and 1.5 (0x10000) and C is the lower bound (that stores the encoded bits.)

Table 1: State Change of Binary Arithmetic Coder

|State|Qc (Hex)|Qc (Dec)|Increase state by|Decrease state by|
|  0  |  59EB  |0.49582 |        1        |        S        |
|  1  |  5522  |0.46944 |        1        |        1        |
|  2  |  504F  |0.44283 |        1        |        1        |
| ... |  ...   |   ...  |       ...       |       ...       |
| ... |  ...   |   ...  |       ...       |       ...       |
| ... |  ...   |   ...  |       ...       |       ...       |
| ... |  ...   |   ...  |       ...       |       ...       |

S means that MPS and LPS must exchange because we made a wrong guess. For example,
if MPS=’1’, we change MPS from ‘1’ to ‘0’ when the encoder encounters “S.”
