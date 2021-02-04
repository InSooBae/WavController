#pragma once

/*wave file format*/
#if 0
typedef struct {
	unsigned char riff[4];                      // RIFF string
	unsigned int overall_size;               // overall size of file in bytes
	unsigned char wave[4];                      // WAVE string
	unsigned char fmt_chunk_marker[4];          // fmt string with trailing null char
	unsigned int length_of_fmt;                 // length of the format data
	unsigned short format_type;                   // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	unsigned short channels;                      // no.of channels
	unsigned int sample_rate;                   // sampling rate (blocks per second)
	unsigned int byterate;                      // SampleRate * NumChannels * BitsPerSample/8
	unsigned short block_align;                   // NumChannels * BitsPerSample/8
	unsigned short bits_per_sample;               // bits per sample, 8- 8bits, 16- 16 bits etc
												  //unsigned char data_chunk_header[4];        // DATA string or FLLR string
												  //unsigned int data_size;                     // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
} Wave_Header;
#endif
/*

Watch the wave format structure at
http://soundfile.sapp.org/doc/WaveFormat/

*/

/*
The WAVE file's size is typically 0~44bytes
*/

/*
The canonical WAVE format starts with the RIFF header
(표준 WAVE 형식은 RIFF 헤더로 시작합니다.)
Resource InterChange File Format으로 avi나 wav같은 파일 형식을 나타내기 위해서 사용
*/
/*chunk descriptor*/
typedef struct {
	unsigned char riff[4];                    // RIFF string "RIFF" (0x52494646 Big-Endian이라 순서대로 RIFF입니다.)
	unsigned int overall_size;               // overall size of file in bytes  (파일 크기에서 Chunk ID와 Chunk Size(8) 만큼 뺀 값으로,﻿ ﻿파일 크기 - 8입니다.(Little-Endian이기 때문에 0x24080000 으로 되어 있으면
												// 거꾸로 0x00000824로 되어 파일 크기는 2084가 됩니다.)
	unsigned char wave[4];                 // WAVE string "WAVE" (파일의 포맷입니다. wav파일의 경우는 대문자로 'WAVE'가 들어갑니다. Big - Endian으로 기록되기 때문에 그냥 'WAVE'로 저장됩니다.)
} Wave_Header;

/*
The "WAVE" format consists of two subchunks: "fmt " and "data"
("WAVE"형식은 "fmt"및 "data"라는 두 개의 하위 청크로 구성됩니다.)
The "fmt " subchunk describes the sound data's format
("fmt"하위 청크는 사운드 데이터의 형식을 설명합니다.)
*/
/*fmt subchunk*/
typedef struct SUB_CHUNK
{
	char                ID[4];				//fmt string with trailing null char "fmt " (여기는 첫번째 Sub chunk로 재생에 필요한 해당 PCM의 형식을﻿ 저장하는 chunk의 시작부분입니다. ﻿여기에는 'fmt '가 들어가는데 fmt 뒤에 공백이 있습니다. ﻿즉, 0x666d7420가 Big - Endian으로 기록됩니다.)
	int                 Size;				//Subchunk1 Size (Subchunk1 Size는 실제 기록되는 데이터 포맷의 크기로﻿ 이 영역 밑에 기록된 Subchunk1의 크기를 나타내며 16 사이즈를 갖습니다(18일 수도). Little - Endian으로 0x10000000 입니다.﻿)
}Sub_Chunk;

typedef struct {
	unsigned short format_type;				// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law (PCM의 경우 1입니다. 주의하실 것은 이 부분은 크기가 2바이트 입니다. ﻿Little - Endian으로 0x0100 입니다.)
	unsigned short channels;					// no.(number) of channels (채널 수 입니다. Mono는 1, Stereo는 2입니다. 이 부분도 2바이트로 구성되어 있으며, Stereo의 경우 0x0200입니다.(Little - Endian))
	unsigned int sample_rate;					// sampling rate (blocks per second) (Sample Rate로 1초를 몇 개의 조각으로 세분화할 것인지입니다. 4바이트로 구성되며, ﻿22050의 경우에는 0x00005622인데 Little - Endian으로 0x22560000이 됩니다.)
	unsigned int byterate;						// SampleRate * NumChannels * BitsPerSample/8 (이것은 SampleRate * NumChannels * BitsPerSample / 8로 구해집니다.﻿BitsPerSample은 각 샘플을 몇개의 비트로 표현하는가 입니다. ﻿ByteRate는 쉽게 말하면 1초의 소리가 차지하는 바이트 수입니다.
													// SampleRate가 22050이며, Stereo(2)이며, 한 샘플을 16bit로 표현하면 ﻿22050 * 2 * 16 / 8 = 88280이 됩니다. ﻿4바이트 Little - Endian으로 되어 있기 때문에 0x88580100이 됩니다.)
	unsigned short block_align;					// NumChannels * BitsPerSample/8 (전체 채널을 포함하는 한 샘플의 크기입니다.채널이 2개(Stereo)면서, ﻿BitPerSample(샘플당 비트수)가 16bit이면 2 * 16 / 8(byte로 변환하기 위해서)입니다. ﻿즉, 크기는 4가 되고 Little - Endian 방식으로 2바이트로 표현되므로 0x0400이 됩니다.)
	unsigned short bits_per_sample;			// bits per sample, 8- 8bits, 16- 16 bits etc (위에 설명했듯이, 샘플당 비트수입니다. 1초를 샘플로 나누고 각각의 샘플을 몇 개의 비트로 표현할 지를 나타냅니다. 비트수가 커질 수록 음질은 나아지겠지만 용량을 그만큼 많이 차지하게 됩니다. 2바이트 Little - Endian 방식으로 표현되므로 16bit의 경우에는 0x1000이 됩니다.)
} Fmt_Header;

extern Sub_Chunk subChunk;
extern void write_wav(char* filename, unsigned long num_samples, short* data, int s_rate);
extern int* read_wav(char* filename, Fmt_Header* fmtHeader, short* data);
extern int read_raw(char* filename, short** data);
extern void write_raw(char* filename, unsigned long num_samples, short* data);
extern void free_audio(short* data);