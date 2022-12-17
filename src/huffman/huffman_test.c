#include <stdio.h>
#include "huffman.h"
#include <ev.h>

extern ev_tstamp tsStart, tsEnd;

float huffman_encode_test(const char* const inputPath, const char* const encodedPath) {
	FILE* fInput;
	FILE* fEncoded;

	fInput = fopen(inputPath, "rb");
	fEncoded = fopen(encodedPath, "wb+");

	tsStart = ev_time();
	huffman_encode_file(fInput, fEncoded);
	tsEnd = ev_time();

	fclose(fInput);
	fclose(fEncoded);

	return (tsEnd - tsStart) * 1000;
}

float huffman_decode_test(const char* const encodedPath, const char* const decodedPath) {
	FILE* fEncoded;
	FILE* fDecoded;

	fEncoded = fopen(encodedPath, "rb");
	fDecoded = fopen(decodedPath, "wb+");

	tsStart = ev_time();
	huffman_decode_file(fEncoded, fDecoded);
	tsEnd = ev_time();

	fclose(fEncoded);
	fclose(fDecoded);

	return (tsEnd - tsStart) * 1000;
}

void huffman_test(const char* const inputPath, const char* const encodedPath, const char* const decodedPath) {
	FILE* fInput;
	FILE* fEncoded;
	FILE* fDecoded;

	fInput = fopen(inputPath, "rb");

	printf("Encoding using Huffman...\n");
	fEncoded = fopen(encodedPath, "wb+");
	huffman_encode_file(fInput, fEncoded);
	fclose(fInput);
	fclose(fEncoded);

	printf("Decoding using Huffman...\n");
	fEncoded = fopen(encodedPath, "rb");
	fDecoded = fopen(decodedPath, "wb+");
	huffman_decode_file(fEncoded, fDecoded);
	fclose(fEncoded);
	fclose(fDecoded);

	printf("Huffman test complete.\n\n");
}
