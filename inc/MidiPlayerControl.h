#ifndef _MidiPlayer_
#define _MidiPlayer_
namespace MediaPlayer
{
	class MidiPlayer
	{
	public:
		static MidiPlayer * GetMidiPlayer();
		bool Init();
		bool DeInit();
		bool OpenSong(char *pSongFileName);
		bool CloseSong();
		bool StartPlay();
		bool StopPlay();
		bool Foreward(unsigned int ms);
		bool Backward(unsigned int ms);
		bool Pause();
		bool Resume();

		bool SetVolume(int volume);
		bool GetVolume(int &volume);
		bool GetTimeLength(unsigned int &ms);
		bool GetCurrentTime(unsigned int &ms);
	protected:
		MidiPlayer();
		~MidiPlayer();
	private:
		static MidiPlayer *MyMidiPlayer_;
		char SongFileName[512];
		int CurrentVolume;
		unsigned int CurrentSamples;
	};
}
#endif
