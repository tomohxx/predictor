# Predictor

Count the number of possible _tempai_ hands wating a tile.

## Usage

Include `predictor.hpp` and instantiate `Predictor` class. The instance can count the number of possible _tempai_ hands by being given left tiles and _furiten_ (cannot be called _rong_) tiles. See [src/test](src/test.cpp) for details on how to use it.

## Test

Build a test program using `Predictor`. You can edit [wall_river.txt](wall_river.txt) to specify the number of hand tiles, left tiles and _furiten_ tiles. The first line of this file is the number of hand tiles. The second and subsequent lines are composed of three columns, of which the first coulmun is labels, the second columns is the number of each left tiles (0 to 4), and the third coulumn is whether _furiten_ or not (1 or 0). 

```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
$ ./mkind
$ ./test ../wall_river.txt
```

Output:

```
$ ./test ../wall_river.txt 
Total   39270395383132
Wait    NormalHand      SevenPairs      ThirteenOrphans Proportion
0       1078779323540   206489564928    369098752       0.0327381
1       1497427489192   206414542080    0       0.0433874
2       1904785533776   206338959360    0       0.0537587
3       1809210481028   206265429504    0       0.051323
4       1770865960192   206270095104    0       0.0503467
5       1809210481028   206265429504    0       0.051323
6       1904785533776   206338959360    0       0.0537587
7       1497427489192   206414542080    0       0.0433874
8       1078779323540   206489564928    369098752       0.0327381
9       1078779323540   206489564928    369098752       0.0327381
10      1497427489192   206414542080    0       0.0433874
11      1904785533776   206338959360    0       0.0537587
12      1809210481028   206265429504    0       0.051323
13      1770865960192   206270095104    0       0.0503467
14      1809210481028   206265429504    0       0.051323
15      1904785533776   206338959360    0       0.0537587
16      1497427489192   206414542080    0       0.0433874
17      1078779323540   206489564928    369098752       0.0327381
18      1078779323540   206489564928    369098752       0.0327381
19      1497427489192   206414542080    0       0.0433874
20      1904785533776   206338959360    0       0.0537587
21      1809210481028   206265429504    0       0.051323
22      1770865960192   206270095104    0       0.0503467
23      1809210481028   206265429504    0       0.051323
24      1904785533776   206338959360    0       0.0537587
25      1497427489192   206414542080    0       0.0433874
26      1078779323540   206489564928    369098752       0.0327381
27      610581839388    206665737984    369098752       0.0208202
28      610581839388    206665737984    369098752       0.0208202
29      610581839388    206665737984    369098752       0.0208202
30      610581839388    206665737984    369098752       0.0208202
31      610581839388    206665737984    369098752       0.0208202
32      610581839388    206665737984    369098752       0.0208202
33      610581839388    206665737984    369098752       0.0208202
Time (msec.)    3
```

In above, the _Wait_ column (0 to 33) is the tile number (1m to 9m, 1p to 2p, 1s to 9s, east to north, and white to red).

## License

GNU General Public License v3.0.
