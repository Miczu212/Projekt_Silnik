#pragma once
#include "WND.h"
#include <xaudio2.h>
#include <vector>
class Sound
{
public:
	Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
	IXAudio2SourceVoice* pSourceVoice;
	WAVEFORMATEX waveFormat = {};
	std::vector<BYTE> audioData;
	XAUDIO2_BUFFER buffer = {};
};
