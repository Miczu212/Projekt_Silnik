#include <assert.h>
#include "SoundHandler.h"

SoundHandler& SoundHandler::Get() noexcept
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
		IdleChannels.push_back(std::make_unique<Channel>(*this));

}

void SoundHandler::Channel::Callbacks::OnBufferEnd(void* pBufferContext)
{
	
	Channel& chan = *(Channel*)pBufferContext;
	if (!chan.pSound->Loop) 
	{
		SoundHandler& soundhandler = SoundHandler::Get();
		chan.Stop();
		chan.pSound->RemoveChannel(chan);
		chan.pSound = nullptr;
		soundhandler.DeactivateChannel(chan);
		if (soundhandler.IdleChannels.size() > soundhandler.nChannels)
			soundhandler.IdleChannels.pop_back();

		
	}
	else if(chan.pSound->ForcefulStop)
	{
		SoundHandler& soundhandler = SoundHandler::Get();
		chan.pSound->ForcefulStop = false;
		chan.pSound->Loop = false;
		chan.pSound->RemoveChannel(chan);
		chan.pSound = nullptr;
		soundhandler.DeactivateChannel(chan);

		if (soundhandler.IdleChannels.size() > soundhandler.nChannels)
			soundhandler.IdleChannels.pop_back();

		
	}
	else
	{
		// Ponownie odtwórz dŸwiêk
		chan.pSound->RemoveChannel(chan);
		chan.PlaySoundBuffer(*chan.pSound, 1.0f, 1.0f); // Przyk³adowe parametry freqMod i vol
	}
}

void SoundHandler::Channel::PlaySoundBuffer(Sound& s, float freqMod, float vol)
{
	if (s.Started == false && s.Loop == true) {
		SoundHandler& soundhandler = SoundHandler::Get();
		if (soundhandler.IdleChannels.size() == 0)
			soundhandler.IdleChannels.push_back(std::make_unique<Channel>(soundhandler));
		assert(pSource && !pSound);
		s.AddChannel(*this);
		pSound = &s;
		xaBuffer.pAudioData = s.pData.get();
		xaBuffer.AudioBytes = s.nBytes;
		pSource->SubmitSourceBuffer(&xaBuffer, nullptr);
		pSource->SetFrequencyRatio(freqMod);
		pSource->SetVolume(vol);
		pSource->Start();
		s.Started = true;
	}
	else if(s.Loop==true && s.Started==true)
	{
		SoundHandler& soundhandler = SoundHandler::Get();
		if (soundhandler.IdleChannels.size() == 0)
			soundhandler.IdleChannels.push_back(std::make_unique<Channel>(soundhandler));
		pSound = &s;
		s.AddChannel(*this);
		xaBuffer.pAudioData = s.pData.get();
		xaBuffer.AudioBytes = s.nBytes;
		pSource->SubmitSourceBuffer(&xaBuffer, nullptr);
		pSource->SetFrequencyRatio(freqMod);
		pSource->SetVolume(vol);
		pSource->Start();
	}
	else
	{
		SoundHandler& soundhandler = SoundHandler::Get();
		if (soundhandler.IdleChannels.size() == 0)
			soundhandler.IdleChannels.push_back(std::make_unique<Channel>(soundhandler));
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
}