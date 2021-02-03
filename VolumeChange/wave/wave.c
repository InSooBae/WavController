#include <stdio.h>
#include <assert.h>
#include "wave.h"

Wave_Header waveHeader;
Sub_Chunk subChunk;

void write_little_endian(unsigned int word, int num_bytes, FILE* wav_file)
{
	unsigned buf;
	while (num_bytes > 0)
	{
		buf = word & 0xff;
		fwrite(&buf, 1, 1, wav_file);
		num_bytes--;
		word >>= 8;
	}
}

int* read_wav(char* filename, Fmt_Header* fmtHeader, short** data)
{
	FILE* fp_in;
	int nSamples;

	fp_in = fopen(filename, "rb");
	if (fp_in == NULL)
	{
		printf("file open error : %s\n", filename);
		return 0;
	}

	fread(&waveHeader, sizeof(Wave_Header), 1, fp_in);
	fread(&subChunk, sizeof(Sub_Chunk), 1, fp_in);
	fread(fmtHeader, sizeof(Fmt_Header), 1, fp_in);

	fseek(fp_in, subChunk.Size - sizeof(Fmt_Header), SEEK_CUR);

	while (1)
	{
		fread(&subChunk, sizeof(Sub_Chunk), 1, fp_in);
		if (*(unsigned int*)&subChunk.ID == 0x61746164)		// subChunk ID 가 "data" 인지 확인
		{
			break;
		}
		// "data" Chunk 가 아닐 경우 chunk data byte 만큼 skip 한다.
		fseek(fp_in, subChunk.Size, SEEK_CUR);
	}

	nSamples = subChunk.Size / 2;
	*data = (short*)malloc(sizeof(short) * nSamples);
	fread(*data, sizeof(short), nSamples, fp_in);
	fclose(fp_in);

	return nSamples;
}

void write_wav(char* filename, unsigned long num_samples, short* data, int s_rate, int mode)
{
	FILE* wav_file=NULL;
	unsigned int sample_rate;
	unsigned int num_channels;
	unsigned int bytes_per_sample;
	unsigned int byte_rate;
	unsigned long i;    /* counter for samples */

	num_channels = 1;   /* monoaural */
	bytes_per_sample = 2;

	if (s_rate <= 0) sample_rate = 44100;
	else sample_rate = (unsigned int)s_rate;

	byte_rate = sample_rate * num_channels * bytes_per_sample;

	wav_file = fopen(filename, "wb");
	assert(wav_file);   /* make sure it opened */
	if (mode == 0) {
		/* write RIFF header */
		fwrite("RIFF", 1, 4, wav_file);
		write_little_endian(36 + bytes_per_sample * num_samples * num_channels, 4, wav_file);
		fwrite("WAVE", 1, 4, wav_file);

		/* write fmt  subchunk */
		fwrite("fmt ", 1, 4, wav_file);
		write_little_endian(16, 4, wav_file);   /* SubChunk1Size is 16 */
		write_little_endian(1, 2, wav_file);    /* PCM is format 1 */
		write_little_endian(num_channels, 2, wav_file);
		write_little_endian(sample_rate, 4, wav_file);
		write_little_endian(byte_rate, 4, wav_file);
		write_little_endian(num_channels * bytes_per_sample, 2, wav_file);  /* block align */
		write_little_endian(8 * bytes_per_sample, 2, wav_file);  /* bits/sample */

																 /* write data subchunk */
		fwrite("data", 1, 4, wav_file);
		write_little_endian(bytes_per_sample * num_samples * num_channels, 4, wav_file);
	}
	//for (i = 0; i< num_samples; i++)
	//{
	//	write_little_endian((unsigned int)(data[i]), bytes_per_sample, wav_file);
	//}

	fwrite(data, sizeof(short), num_samples, wav_file);

	fclose(wav_file);
}