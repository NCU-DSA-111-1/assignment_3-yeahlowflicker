# Assignment 4 - ARCD & Huffman Comparison

## Compiling the test program

```bash
gcc -o compare_test.out compare_test.c -lm -lev ./arcd/arcd.c ./arcd/arcd_test.c -lev ./arcd/adaptive_model.c ./huffman/huffman_test.c -lev -lm ./huffman/huffman.c
```

## Running the test program

```bash
./compare_test.out <PATH_OF_TESTDATA> <ITERATIONS>
```

`ITERATIONS` is optional. It defaults to 100 if not provided.
