An in progress 2d puzzle game running on Windows and the Nintendo Wii videogame console.

Compilation:
	Visual Studio:
	Requires SDL 1.2.15, SDL_Image, SDL_TTF and SDL_Mixer libraries be installed. You will need to change the Include and Library directories under VC++ directories to point to these files.
	Wii:
	Requires DevKitPro as well as the latest version of the Wii port of the SDL library.
	Requires the following ported libraries:
		libjpeg
		libpng
		libz
		libvorbisidec
	Additional Libraries:
	libwupc - for Wii U Pro controller support