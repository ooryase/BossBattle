#include"Sound.h"

HRESULT Sound::Init()
{
	//CoInitializeEx(0, )

	HRESULT hr = XAudio2Create(pXAudio.GetAddressOf());
	if (FAILED(hr))
		return hr;

	hr = pXAudio->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

void Sound::Release()
{
	delete pMasteringVoice;
	pXAudio.Reset();
}

void Sound::Load()
{
	FILE* file = NULL;

	if (fopen_s(&file, "sss.wav", "rb") != 0)
	{
		return;
	}

	RiffHeader riff;
	fread(&riff, sizeof(riff), 1, file);

	FormatChunk format;
	fread(&format, sizeof(format), 1, file);

	Chunk data;
	fread(&data, sizeof(data), 1, file);

	char* pBuffer = (char*)malloc(data.size);
	fread(pBuffer, data.size, 1, file);

	fclose(file);

	//////////////////////////////////////////

	WAVEFORMATEX wfex{};
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	// 波形フォーマットを元にSourceVoiceの生成
	if (FAILED(pXAudio->CreateSourceVoice(&pSourceVoice, &wfex))) {
		free(pBuffer);
		return;
	}

	//XAUDIO2_BUFFER buf{};
	//// 再生する波形データの設定
	//buf.pAudioData = (BYTE*)pBuffer;
	//buf.Flags = XAUDIO2_END_OF_STREAM;
	//buf.AudioBytes = data.size;
	//// 波形データの再生
	//sourceVoice->SubmitSourceBuffer(&buf);
	//sourceVoice->Start();
}

Sound& Sound::GetInstance()
{
	static Sound instance;
	return instance;
}