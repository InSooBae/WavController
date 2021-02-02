// VolumeChange.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "wave/wave.h"

Fmt_Header fmtHeader;

short* AudioData;

void Usage()
{
	printf("Usage : VolumeChange infile.wav outfile.wav Volume\n");
}

/* 
argc는 매개변수 카운트 아무 정보도 전달하지 않으면 기본적으로 argv의 첫번째 값으로
실행경로가 들어가 있으며 실행경로 정보 1개 뿐이라 argv1
*/
int main(int argc, char* argv[])
{
	FILE* fp_in, * fp_out;
	int nSamples;
	short* pAudioData;
	float VolumeFactor;

	if (argc != 4)
	{
		Usage();
		printf("Press enter to quit program\n");
		getchar();
		return 0;
	}
	VolumeFactor = (float)atof(argv[3]);

	nSamples = read_wav(argv[1], &fmtHeader, AudioData);

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

	pAudioData = AudioData;
	for (int i = 0; i < nSamples; i++)
	{
		*pAudioData = (short)(*pAudioData * VolumeFactor);
		pAudioData++;
	}

	write_wav(argv[2], nSamples, AudioData, 16000);

	//	fwrite(AudioData, sizeof(short), nSamples, fp_out);

	if (AudioData != NULL)
	{
		free(AudioData);
	}

	return 1;
}