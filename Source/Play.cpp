
#include "Kip.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <fstream>
#include <vector>
using namespace std;

static Mix_Music *audio;
static int idx = 0;
static vector<string> auds;

void Init()
{
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
}

void Exit()
{
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

void ExitModule()
{
    Exit();
    exit(0);
}

void PlayNext()
{
    audio = Mix_LoadMUS(auds[idx].c_str());
    if(!audio) ExitModule();
    Mix_PlayMusic(audio, 1);
}

int main()
{
    Init();
    ifstream play("sdmc:/sys-play.txt");
    if(play.good())
    {
        string buf;
        while(getline(play, buf))
        {
            buf.pop_back();
            string audfile = "sdmc:/media/" + buf;
            ifstream ifs(audfile);
            if(ifs.good()) auds.push_back(audfile);
            ifs.close();
        }
    }

    if(auds.empty()) ExitModule();

    PlayNext();

    while(appletMainLoop())
    {
        if(Mix_PlayingMusic() == 0)
        {
            if(idx < auds.size()) idx++;
            else idx = 0;
            PlayNext();
        }
        svcSleepThread(30000000L);
    }

    Exit();
    return 0;
}
