#include <assert.h>
#include "SoundHandler.h"

SoundHandler& SoundHandler::Get()
{
	static SoundHandler instance;
	return instance;
}

SoundHandler::SoundHandler()
{
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = 2;
	format.nSamplesPerSec = 44100;
	format.nAvgBytesPerSec = 176400;
	format.nBlockAlign = 4;
	format.wBitsPerSample = 16;
	format.cbSize = 0;
	XAudio2Create(&pEngine);
	pEngine->CreateMasteringVoice(&pMaster);
	for (int i = 0; i < nChannels; i++)
	{
		idleChannelPtrs.push_back(std::make_unique<Channel>(*this));
	}
}

void SoundHandler::Channel::VoiceCallback::OnBufferEnd(void* pBufferContext)
{
	Channel& chan = *(Channel*)pBufferContext;
	chan.Stop();
	chan.pSound->RemoveChannel(chan);
	chan.pSound = nullptr;
	SoundHandler::Get().DeactivateChannel(chan);
}

void SoundHandler::Channel::PlaySoundBuffer(Sound& s, float freqMod, float vol)
{
	assert(pSource && !pSound);
	s.AddChannel(*this);
	pSound = &s;
	xaBuffer.pAudioData = s.pData.get();
	xaBuffer.AudioBytes = s.nBytes;
	pSource->SubmitSourceBuffer(&xaBuffer, nullptr);
	pSource->SetFrequencyRatio(freqMod);
	pSource->SetVolume(vol);
	pSource->Start();
}