#pragma once
#include"Resource.h"
#include<xaudio2.h>


class Sound
{
	Sound(const Sound&) = delete;
	Sound operator = (const Sound&) = delete;
	Sound() {};


	// �`�����N�f�[�^�̊�{�\�� 
	struct Chunk
	{
		char	id[4]; // �`�����N����ID
		int32_t	size;  // �`�����N�T�C�Y
	};

	// RIFF�w�b�_�[
	struct RiffHeader
	{
		Chunk	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};

	// FMT�`�����N
	struct FormatChunk
	{
		Chunk		chunk; // "fmt "
		WAVEFORMAT	fmt;   // �g�`�t�H�[�}�b�g
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