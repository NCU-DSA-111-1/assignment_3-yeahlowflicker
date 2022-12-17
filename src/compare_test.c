#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "arcd/arcd_test.h"
#include "huffman/huffman_test.h"

#include <ev.h>

#define TEST_ITERATIONS 100

ev_tstamp tsStart, tsEnd;

float PrintAverage(const char* const prefix, const float* const array) {
    float sum = 0.0;
    for (int i = 0; i < TEST_ITERATIONS; ++i)
        sum += array[i];
    const float avg = sum / TEST_ITERATIONS;
    printf("%s %f ms\n", prefix, avg);

    return avg;
}

int main(int argc, char** argv) {

    if ((argc < 2)) {
        printf("Insufficient arguments.\n\nUsage:\n./compare_test.out <PATH_TO_TEST_DATA> <NO_OF_INTERATIONS>\n\n");
        return 0;
    }

    const char* testdata_path = argv[1];
    const int iterations = (argc > 2) ? atoi(argv[2]) : TEST_ITERATIONS;

    printf("\nRunning test using '%s' for %d iterations...\n\n===== Results =====\n", testdata_path, iterations);

    float arcdEncodeResults[TEST_ITERATIONS] = {0.0};
    float arcdDecodeResults[TEST_ITERATIONS] = {0.0};
    float huffmanEncodeResults[TEST_ITERATIONS] = {0.0};
    float huffmanDecodeResults[TEST_ITERATIONS] = {0.0};

    for (int i = 0; i < TEST_ITERATIONS; ++i) {
        arcdEncodeResults[i] = arcd_encode_test(testdata_path, "./tmp/encoded_arcd.txt");
        huffmanEncodeResults[i] = arcd_encode_test(testdata_path, "./tmp/encoded_huffman.txt");

        arcdDecodeResults[i] = arcd_decode_test("./tmp/encoded_arcd.txt", "./tmp/decoded_arcd.txt");
        huffmanDecodeResults[i] = arcd_decode_test("./tmp/encoded_huffman.txt", "./tmp/decoded_huffman.txt");
    }

    float arcd_encode_avg = PrintAverage("ARCD Encode: ", arcdEncodeResults);
    float huffman_encode_avg = PrintAverage("Huff Encode: ", huffmanEncodeResults);
    printf("Difference:   %f ms\n\n", fabs(arcd_encode_avg - huffman_encode_avg));

    float arcd_decode_avg = PrintAverage("ARCD Decode: ", arcdDecodeResults);
    float huffman_decode_avg = PrintAverage("Huff Decode: ", huffmanDecodeResults);
    printf("Difference:   %f ms\n\n", fabs(arcd_decode_avg - huffman_decode_avg));

    printf("\nTest complete.\n\n");

    return 0;
}
