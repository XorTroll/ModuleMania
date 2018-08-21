
#include "SysModule.hpp"
#include "DebugTypes.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "json.hpp"
using namespace nlohmann;

struct AudioEntry
{
    u64 TitleID;
    string Audio;
};

u64 playid = 0;

void init()
{
	SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
}

AudioEntry cur;
Mix_Music *audio;
bool cplay = false;

int main()
{
	init();
	vector<AudioEntry> ents;
    ifstream ifs("sdmc:/xor.play.json");
    if(ifs.good())
    {
        json parser;
        ifs >> parser;
        for(auto i = parser.begin(); i != parser.end(); i++)
        {
            string stid = i.key();
			u64 tid = strtoull(stid.c_str(), NULL, 16);
			if(tid < 0x0100000000002000) tid = 0;
            string aud = i.value().get<string>();
            AudioEntry ent = { tid, aud };
            ents.push_back(ent);
        }
    }
	ifs.close();
	if(ents.empty()) return 0;
	while(appletMainLoop())
	{
		Result rc;
		u64 pid;
		u64 tid = 0;
		int count;
		u64* pids = new u64[256];
		u32 pid_count;
		Handle debug_handle;
		DebugEventInfo *d = new DebugEventInfo;
		svcGetProcessList(&pid_count, pids, 256);
		for(int i = 0; i < pid_count; i++)
		{
			if(true) // (pids[i] >= 0x70)
			{
				rc = svcDebugActiveProcess(&debug_handle, pids[i]);
				if(R_SUCCEEDED(rc))
				{
					pid = pids[i];
					while(R_SUCCEEDED(svcGetDebugEvent((u8*)d, debug_handle))) if(d->type == DebugEventType::AttachProcess) break;
					tid = d->info.attach_process.title_id;
					svcCloseHandle(debug_handle);
					break;
				}
			}
		}
		if(Mix_PlayingMusic() != 1) cplay = false;
		if(!cplay)
		{
			for(int i = 0; i < ents.size(); i++)
			{
				AudioEntry eent = ents[i];
				if(tid == eent.TitleID)
				{
					audio = Mix_LoadMUS(eent.Audio.c_str());
					Mix_PlayMusic(audio, 1);
					playid = tid;
					cplay = true;
					break;
				}
			}
		}
		else
		{
			if(tid != playid)
			{
				Mix_HaltMusic();
				Mix_FreeMusic(audio);
				cplay = false;
			}
		}
		svcSleepThread(30000000L);
	}
	Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
	return 0;
}