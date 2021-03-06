// VolumeChange.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include "wave/wave.h"
#include "argparse.h"

// 입력 방식이 어떤지 명시
static const char* const usage[] = {
	"VolumeChange [options] [[--] args]",
	"VolumeChange [options]",
	NULL,
};

enum Mode {    // 열거형 정의
	MODIFY = 0,         // 초깃값 할당
	RAW,
	WAV,
};

//Shift operation
#define PERM_READ  (1<<0)	// 1
#define PERM_WRITE (1<<1)	// 2
#define PERM_EXEC  (1<<2)	// 4

Fmt_Header fmtHeader;

short* AudioData;

void Usage(int flag)
{
	switch (flag) {
	case MODIFY:
		printf("error: Not enough parameter [-options] [arg1] [arg2]\n");
		printf("Usage : VolumeChange -m -f 0.5 infile.wav outfile.wav Volume(float)\n");
		break;
	case RAW:
		printf("error: Not enough parameter [-options] [arg1] [arg2]\n");
		printf("Usage : VolumeChange infile.wav outfile.raw\n");
		break;
	case WAV:
		printf("error: Not enough parameter [-options] [arg1] [arg2]\n");
		printf("Usage : VolumeChange infile.raw outfile.wav\n");
		break;
	}
}

/*
argc는 매개변수 카운트 아무 정보도 전달하지 않으면 기본적으로 argv의 첫번째 값으로
실행경로가 들어가 있으며 실행경로 정보 1개 뿐이라 argv[1]부터 이용하는게 바람직하다.
*/
int main(int argc, char* argv[])
{
	FILE* fp_in, * fp_out;
	int nSamples;
	short* pAudioData;
	float VolumeFactor;

	int modify = 0;
	int raw = 0;
	int wav = 0;
	// when flt is zero it can't mute wav file
	float flt = -1;
	bool check = false;
	const char* path = NULL;
	struct argparse_option options[] = {
		OPT_HELP(),
		OPT_GROUP("Basic options"),
		OPT_BOOLEAN('m', "modify", &modify, "modify WAV Volume. (need -f option)"),
		OPT_BOOLEAN('r', "raw", &raw, "make RAW file"),
		OPT_BOOLEAN('w', "wav", &wav, "make WAV file"),
		OPT_FLOAT('f', "float", &flt, "selected float"),
		OPT_END(),
	};

	struct argparse argparse;
	argparse_init(&argparse, options, usage, 0);
	argparse_describe(&argparse, "\nA brief description of what the program does and how it works.", "\nAdditional description of the program after the description of the arguments.");
	argc = argparse_parse(&argparse, argc, argv);
	if (argc < 1) {
		argparse_help_cb(&argparse, options);
	}
	// modify wav file's volume
	if (modify != 0 && flt != -1) {
		// wrong parameter
		if (argc < 2) {
			Usage(MODIFY);
			return 0;
		}
		check = true;
		//VolumeFactor = (float)atof(argv[2]);
		VolumeFactor = flt;
		nSamples = read_wav(argv[0], &fmtHeader, &AudioData);
		pAudioData = AudioData;
		for (int i = 0; i < nSamples; i++)
		{
			*pAudioData = (short)(*pAudioData * VolumeFactor);
			pAudioData++;
		}

		write_wav(argv[1], nSamples, AudioData, 16000);

		//	fwrite(AudioData, sizeof(short), nSamples, fp_out);

		free_audio(AudioData);
	}
	// convert to raw file(without header. just data = wav - 44bytes(header size))
	if (raw != 0) {
		if (argc < 2) {
			Usage(RAW);
			return 0;
		}
		check = true;
		nSamples = read_wav(argv[0],&fmtHeader, &AudioData);
		pAudioData = AudioData;
		write_raw(argv[1], nSamples, AudioData);

		free_audio(AudioData);
	}
	// convert raw file to wav
	if (wav != 0) {
		if (argc < 2) {
			Usage(RAW);
			return 0;
		}
		check = true;
		nSamples = read_raw(argv[0], &AudioData);
		pAudioData = AudioData;
		write_wav(argv[1], nSamples, AudioData, 16000);

		free_audio(AudioData);
	}
	// split and save wav
	/*if (wav != 0) {
		if (argc < 2) {
			Usage(RAW);
			return 0;
		}
		check = true;
		nSamples = read_wav(argv[0], &fmtHeader, &AudioData, WAV);
		pAudioData = AudioData;
		write_wav(argv[1], nSamples, AudioData, 16000, MODIFY);

		free_audio(AudioData);
	}*/
	if (!check) {
		argparse_help_cb(&argparse, options);
	}
	// for check argc,argv
	if (argc != 0) {
		printf("argc: %d\n", argc);
		int i;
		for (i = 0; i < argc; i++) {
			printf("argv[%d]: %s\n", i, *(argv + i));
		}
	}

#if 0
	fread(&waveHeader, sizeof(Wave_Header), 1, fp_in);
	if (waveHeader.length_of_fmt == 18)
	{
		unsigned char dummy[2];
		fread(dummy, sizeof(unsigned char), 2, fp_in);
		waveHeader.length_of_fmt = 16;
	}

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
	AudioData = (short*)malloc(sizeof(short) * nSamples);
	fread(AudioData, sizeof(short), nSamples, fp_in);
	fclose(fp_in);
#endif

	return 1;
}