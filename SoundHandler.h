#pragma once
#include "WND.h"
#include <memory>
#include <stdexcept>
#include <fstream>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <xaudio2.h>
#include <mutex>
#include <string>
#include <atomic>
#include <wrl\client.h>

class SoundHandler
{
public:
	class Error : public std::runtime_error
	{
	public:
		Error(std::string s)
			:
			runtime_error(std::string("SoundHandler::Error: ") + s)
		{}
	};
	class FileError : public Error
	{
	public:
		FileError(std::string s)
			:
			Error(std::string("SoundHandler::FileError: ") + s)
		{}
	};
public:
	class Channel
	{
	private:
		class VoiceCallback : public IXAudio2VoiceCallback
		{
		public:
			void STDMETHODCALLTYPE OnStreamEnd() override
			{}
			void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override
			{}
			void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) override
			{}
			void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) override;
			void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) override
			{}
			void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) override
			{}
			void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) override
			{}
		};
	public:
		Channel(SoundHandler& sys)
		{
			static VoiceCallback vcb;
			ZeroMemory(&xaBuffer, sizeof(xaBuffer));
			xaBuffer.pContext = this;
			sys.pEngine->CreateSourceVoice(&pSource, &sys.format, 0u, 2.0f, &vcb);
		}
		Channel(const Channel&) = delete;
		~Channel()
		{
			if (!pSound)
				pSound = nullptr;

				pSource->DestroyVoice();
				pSource = nullptr;
			
		}
		void PlaySoundBuffer(class Sound& s, float freqMod, float vol);
		void Stop()
		{
			assert(pSource && pSound);
			pSource->Stop();
			pSource->FlushSourceBuffers();
		}
	private:
		XAUDIO2_BUFFER xaBuffer;
		IXAudio2SourceVoice* pSource = nullptr;
		class Sound* pSound = nullptr;
	};
public:
	SoundHandler(const SoundHandler&) = delete; //Usuwamy Copy Konstruktor dla SoundHandlera
	static SoundHandler& Get() noexcept; //funkcja która upewnia nas ze system zostanie zainicjalizowany tylko raz
	static WAVEFORMATEX& GetFormat()
	{
		return Get().format;
	}
	void PlaySoundBuffer(class Sound& s, float freqMod, float vol)
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (idleChannelPtrs.size() > 0)
		{
			activeChannelPtrs.push_back(std::move(idleChannelPtrs.back()));
			idleChannelPtrs.pop_back();
			activeChannelPtrs.back()->PlaySoundBuffer(s, freqMod, vol);
		}
	}
private:
	SoundHandler();
	void DeactivateChannel(Channel& Chan)
	{
		std::lock_guard<std::mutex> lock(mutex);
		auto i = std::find_if(activeChannelPtrs.begin(), activeChannelPtrs.end(),
			[&Chan](const std::unique_ptr<Channel>& pChan) -> bool
			{
				return &Chan == pChan.get();
			});
		idleChannelPtrs.push_back(std::move(*i));
		activeChannelPtrs.erase(i);
	}
private:
	Microsoft::WRL::ComPtr<IXAudio2> pEngine;
	IXAudio2MasteringVoice* pMaster = nullptr;
	WAVEFORMATEX format;
	const int nChannels = 8;
	std::mutex mutex;
	std::vector<std::unique_ptr<Channel>> idleChannelPtrs;
	std::vector<std::unique_ptr<Channel>> activeChannelPtrs;
};

class Sound
{
	friend SoundHandler::Channel;
public:
	Sound(const Sound& other) //Copy Konstruktor Dla Klasy DŸwiêku
		: nBytes(other.nBytes), pData(new BYTE[other.nBytes]), activeChannelPtrs(other.activeChannelPtrs) {
		// Kopiowanie danych z innego obiektu do obecnego obiektu
		std::copy(other.pData.get(), other.pData.get() + other.nBytes, pData.get());
	}

	void Stop()
	{
		std::lock_guard<std::mutex> lock(mutex);
		for (auto pChannel : activeChannelPtrs)
		{
			pChannel->Stop();
		}
	}

	Sound(const std::wstring& fileName)
	{
		unsigned int fileSize = 0;
		std::unique_ptr<BYTE[]> pFileIn;
		try
		{
			{
				std::ifstream file;
				file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
				try
				{
					file.open(fileName, std::ios::binary);
				}
				catch (std::exception e)
				{
					throw SoundHandler::FileError(
						std::string("B³¹d Podczas Próby Otworzenia Pliku: ") + e.what());
				}

				{
					int fourcc;
					file.read(reinterpret_cast<char*>(&fourcc), 4);
					if (fourcc != 'FFIR')
					{
						throw SoundHandler::FileError("bad fourCC");
					}
				}

				file.read(reinterpret_cast<char*>(&fileSize), 4);
				fileSize += 8; 
				if (fileSize <= 44)
				{
					throw SoundHandler::FileError("Plik jest za ma³y"); // nie ma fourcc albo plik pusty
				}

				file.seekg(0, std::ios::beg);
				pFileIn = std::make_unique<BYTE[]>(fileSize);
				file.read(reinterpret_cast<char*>(pFileIn.get()), fileSize);
			}

			if (*reinterpret_cast<const int*>(&pFileIn[8]) != 'EVAW')
			{
				throw SoundHandler::FileError("Format to nie WAVE");
			}

			//Szukamy 'fmt ' chunk id
			WAVEFORMATEX format;
			bool bFilledFormat = false;
			for (unsigned int i = 12; i < fileSize; )
			{
				if (*reinterpret_cast<const int*>(&pFileIn[i]) == ' tmf')
				{
					memcpy(&format, &pFileIn[i + 8], sizeof(format));
					bFilledFormat = true;
					break;
				}
				// chunk size + size entry size + chunk id entry size + word padding
				i += (*reinterpret_cast<const int*>(&pFileIn[i + 4]) + 9) & 0xFFFFFFFE;
			}
			if (!bFilledFormat)
			{
				throw SoundHandler::FileError("fmt chunk nie znaleziony");
			}

			//Porównanie formatu pliku z formatem przyjmowanym przez soundhandler
			{
				const WAVEFORMATEX sysFormat = SoundHandler::GetFormat();

				if (format.nChannels != sysFormat.nChannels)
				{
					throw SoundHandler::FileError("Z³y Format Pliku (nChannels)");
				}
				else if (format.wBitsPerSample != sysFormat.wBitsPerSample)
				{
					throw SoundHandler::FileError("Z³y Format Pliku (wBitsPerSample)");
				}
				else if (format.nSamplesPerSec != sysFormat.nSamplesPerSec)
				{
					throw SoundHandler::FileError("Z³y Format Pliku (nSamplesPerSec)");
				}
				else if (format.wFormatTag != sysFormat.wFormatTag)
				{
					throw SoundHandler::FileError("Z³y Format Pliku (wFormatTag)");
				}
				else if (format.nBlockAlign != sysFormat.nBlockAlign)
				{
					throw SoundHandler::FileError("Z³y Format Pliku (nBlockAlign)");
				}
				else if (format.nAvgBytesPerSec != sysFormat.nAvgBytesPerSec)
				{
					throw SoundHandler::FileError("Z³y Format Pliku (nAvgBytesPerSec)");
				}
			}
			bool bFilledData = false;
			for (unsigned int i = 12; i < fileSize; )
			{
				const int chunkSize = *reinterpret_cast<const int*>(&pFileIn[i + 4]);
				if (*reinterpret_cast<const int*>(&pFileIn[i]) == 'atad')
				{
					pData = std::make_unique<BYTE[]>(chunkSize);
					nBytes = chunkSize;
					memcpy(pData.get(), &pFileIn[i + 8], nBytes);

					bFilledData = true;
					break;
				}
				// chunk size + size entry size + chunk id entry size + word padding
				i += (chunkSize + 9) & 0xFFFFFFFE;
			}
			if (!bFilledData)
			{
				throw SoundHandler::FileError("data chunk not found");
			}
		}
		catch (SoundHandler::Error e)
		{
			throw e;
		}
		catch (std::exception e)
		{
			throw SoundHandler::FileError(e.what());
		}
	}
	Sound(Sound&& donor) noexcept
		:
		nBytes(donor.nBytes),
		pData(std::move(donor.pData)),
		activeChannelPtrs(std::move(donor.activeChannelPtrs))
	{}
	void Play(float freqMod, float vol)
	{
		SoundHandler::Get().PlaySoundBuffer(*this, freqMod, vol);
	}
	~Sound()
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			for (auto pChannel : activeChannelPtrs)
			{
				pChannel->Stop();
			}
		}

		bool allChannelsDeactivated = false;
		do
		{
			std::lock_guard<std::mutex> lock(mutex);
			allChannelsDeactivated = activeChannelPtrs.size() == 0;
		} while (!allChannelsDeactivated);
	}
private:
	void RemoveChannel(SoundHandler::Channel& channel)
	{
		std::lock_guard<std::mutex> lock(mutex);
		activeChannelPtrs.erase(std::find(
			activeChannelPtrs.begin(), activeChannelPtrs.end(), &channel));
	}
	void AddChannel(SoundHandler::Channel& channel)
	{
		std::lock_guard<std::mutex> lock(mutex);
		activeChannelPtrs.push_back(&channel);
	}
private:
	UINT32 nBytes = 0;
	std::unique_ptr<BYTE[]> pData;
	std::mutex mutex;
	std::vector<SoundHandler::Channel*> activeChannelPtrs;
};