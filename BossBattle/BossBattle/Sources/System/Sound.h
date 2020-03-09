#pragma once
#include"Resource.h"
#include<xaudio2.h>


class Sound
{
	Sound(const Sound&) = delete;
	Sound operator = (const Sound&) = delete;
	Sound() {};


	// チャンクデータの基本構造 
	struct Chunk
	{
		char	id[4]; // チャンク毎のID
		int32_t	size;  // チャンクサイズ
	};

	// RIFFヘッダー
	struct RiffHeader
	{
		Chunk	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		Chunk		chunk; // "fmt "
		WAVEFORMAT	fmt;   // 波形フォーマット
	};

	//IXAudio2* pXAudio;
	ComPtr<IXAudio2> pXAudio;
	IXAudio2MasteringVoice* pMasteringVoice;

public:

	HRESULT Init();

	void Load();

	//void Play();

	void Release();

	static Sound& GetInstance();


};