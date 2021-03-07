#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * const argv[])
{	
	if (argc < 1) {
		fprintf(stderr, "error: not enough arguments\n");
		exit(EXIT_FAILURE);
	}
	SF_INFO sf_info;
	sf_info.format = 0;
	SNDFILE *input = sf_open(argv[1], SFM_READ, &sf_info);
	if (!input) {
		perror("input open");
		exit(EXIT_FAILURE);
	}

	sf_count_t minute = sf_info.samplerate * 60;

	double *chunk = calloc(minute * sf_info.channels, sizeof(*chunk));
	if (!chunk) {
		perror("chunk calloc");
		sf_close(input);
		exit(EXIT_FAILURE);
	}
	double *transformed = calloc(minute * sf_info.channels, sizeof(*transformed));
	if (!transformed) {
		perror("transformed calloc");
		free(chunk);
		sf_close(input);
		exit(EXIT_FAILURE);
	}
	
	register unsigned int i = 0;
	register unsigned int j = 0;

	sf_count_t chunk_size = 0;
	do {
		chunk_size = sf_readf_double(input, chunk, minute);
		for (i = 0; i < chunk_size * 2; ++i) {
			transformed[i] += chunk[i];
		}
		if (chunk_size) {
			++j;
		}
	} while (chunk_size);

	double n_1 = 1.0L / (double) j;
	for (i = 0; i < chunk_size * 2; ++i) {
		transformed[i] *= n_1;
	}

	SF_INFO out_info;
	out_info.samplerate = sf_info.samplerate;
	out_info.channels = sf_info.channels;
	out_info.format = sf_info.format;

	SNDFILE *output = sf_open("one_minute.wav", SFM_WRITE, &out_info);
	if (output) {
		sf_writef_double(output, transformed, minute);
		sf_close(output);
	} else {
		perror("output.wav open");
	}

	sf_close(input);

	free(chunk);
	free(transformed);
	
	exit(EXIT_SUCCESS);
}
