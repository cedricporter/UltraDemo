#pragma once

#include <string>

class MusicPlayer 
{
public:

	void LoadMusicFromFile( const std::string& filename );

	void Play();

	void Stop();

	void Pause();

	void Resume();

};