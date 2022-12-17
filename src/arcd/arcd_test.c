#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "arcd.h"
#include "adaptive_model.h"

#include <ev.h>

extern ev_tstamp tsStart, tsEnd;


void output(const arcd_buf_t buf, const unsigned buf_bits, void *const io)
{
	(void)buf_bits;
	FILE *const f = (FILE *)io;
	fwrite(&buf, sizeof(buf), 1, f);
}

unsigned input(arcd_buf_t *const buf, void *const io)
{
	FILE *const f = (FILE *)io;
	return 8 * fread(buf, sizeof(*buf), 1, f);
}

void usage(FILE *const out)
{
	fprintf(out, "Usage:\n");
	fprintf(out, "    arcd_stream [-e | -d | -h]\n\n");
	fprintf(out, "-e - encode stdin to stdout\n");
	fprintf(out, "-d - decode stdin to stdout\n");
	fprintf(out, "-h - help\n\n");
	fflush(out);
}

typedef unsigned char symbol_t;
static const arcd_char_t EOS = 1 << (8 * sizeof(symbol_t));


float arcd_encode_test(const char* const inputPath, const char* const encodedPath) {
	adaptive_model model;

	adaptive_model_create(&model, EOS + 1);

	FILE* const fInput = fopen(inputPath, "rb");
	FILE* fEncoded = fopen(encodedPath, "wb+");

	tsStart = ev_time();

	arcd_enc enc;
	arcd_enc_init(&enc, adaptive_model_getprob, &model, output, fEncoded);
	symbol_t sym;
	while (0 < fread(&sym, sizeof(sym), 1, fInput))
		arcd_enc_put(&enc, sym);
	arcd_enc_put(&enc, EOS);
	arcd_enc_fin(&enc);

	adaptive_model_free(&model);

	tsEnd = ev_time();

	fclose(fInput);
	fclose(fEncoded);

	return (tsEnd - tsStart) * 1000;
}

float arcd_decode_test(const char* const encodedPath, const char* const decodedPath) {
	adaptive_model model;

	adaptive_model_create(&model, EOS + 1);

	FILE* const fEncoded = fopen(encodedPath, "rb");
	FILE* const fDecoded = fopen(decodedPath, "wb+");

	tsStart = ev_time();

	arcd_dec dec;
	arcd_dec_init(&dec, adaptive_model_getch, &model, input, fEncoded);
	arcd_char_t ch;
	while (EOS != (ch = arcd_dec_get(&dec)))
	{
		const symbol_t sym = (unsigned char)ch;
		fwrite(&sym, sizeof(sym), 1, fDecoded);
	}

	adaptive_model_free(&model);

	tsEnd = ev_time();

	fclose(fEncoded);
	fclose(fDecoded);

	return (tsEnd - tsStart) * 1000;
}

void arcd_test(const char* const inputPath, const char* const encodedPath, const char* const decodedPath)
{
	adaptive_model model;

    // Encoding
	printf("Encoding using ARCD...\n");

	adaptive_model_create(&model, EOS + 1);

    FILE* const fInput = fopen(inputPath, "rb");
    FILE* fEncoded = fopen(encodedPath, "wb+");

	arcd_enc enc;
	arcd_enc_init(&enc, adaptive_model_getprob, &model, output, fEncoded);
	symbol_t sym;
	while (0 < fread(&sym, sizeof(sym), 1, fInput))
		arcd_enc_put(&enc, sym);
	arcd_enc_put(&enc, EOS);
	arcd_enc_fin(&enc);

    adaptive_model_free(&model);
    fclose(fInput);
    fclose(fEncoded);



    // Decoding
	printf("Decoding using ARCD...\n");

    adaptive_model_create(&model, EOS + 1);

    fEncoded = fopen(encodedPath, "rb");
    FILE* const fDecoded = fopen(decodedPath, "wb+");

	arcd_dec dec;
	arcd_dec_init(&dec, adaptive_model_getch, &model, input, fEncoded);
	arcd_char_t ch;
	while (EOS != (ch = arcd_dec_get(&dec)))
	{
		const symbol_t sym = (unsigned char)ch;
		fwrite(&sym, sizeof(sym), 1, fDecoded);
	}

	adaptive_model_free(&model);
	fclose(fEncoded);
	fclose(fDecoded);

	printf("ARCD test complete.\n\n");
}
