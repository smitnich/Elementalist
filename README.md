An grid-based 2d puzzle game for Windows, Linux, and the Wii game console<br>
<br>
<h3>Controls:</h3><ul>
Basic USB Keyboards and mice ARE supported on the Wii.<br>
<ul>
<li>Keyboard<br>
<li>Movement: WASD or arrow keys</li>
<li>Menu Selection: Z or X (or mouse)</li>
<li>Switch levels: Hold + or -</li>
<li>Reset level: Use menu button</li>
<li>Exit: Hold Escape</li>
</ul>
</li>
<li>Traditional Controller (PC Joypad, Gamecube, Classic Controller (PRO), and Wii U Pro Controllers<br>
<ul>
<li>Movement: Primary joystick or D-pad</li>
<li>Menu Selection: Face buttons</li>
<li>Switch Levels: Hold Left or right triggers</li>
<li>Reset Level: Hold right bumper</li>
<li>Exit: Hold Start/Home button</li>
</ul>
</li>
<li>Wii Remote (sideways)<br>
<ul>
<li>Movement: D-pad</li>
<li>Menu Selection 1 or 2 buttons</li>
<li>Switch Levels: Hold + or - buttons</li>
<li>Reset Level: Hold B button (back of controller)</li>
<li>Exit: Hold Home button</li>
</ul>
</li>
<li>Wii Remote (with Nunchuk<br>
<ul>
<li>Movement: D-pad or joystick</li>
<li>Menu Selection: C, Z, 1 or 2</li>
<li>Switch Levels: Hold + or - buttons</li>
<li>Reset Level: Hold B button</li>
<li>Exit: Hold Home button</li>
</ul>
</li>
<li>Mouse and Wii Remote can be used for selecting menu elements as well</li>
</ul>
<h3>Compilation:</h3><ul>
<li>Visual Studio:<br>
	Requires SDL 1.2.15, SDL_Image, SDL_TTF and SDL_Mixerlibraries be installed. You will need to change the Include and Library directories under VC++ directories to point to these files.</li>
<li> Wii:<br>
	Requires DevKitPro as well as the latest version of the Wii port of the SDL library.<br>
	Requires the following ported libraries:<br>
		<ul>
		<li>libjpeg</li>
		<li>libpng</li>
		<li>libz</li>
		<li>libvorbisidec<br>
		</ul>
	Additional Libraries:<br>
	libwupc - for Wii U Pro controller support<br>
</li>
<li>Linux:<br>
A Code::Blocks file that is used to build the project is provided.
</li>
</ul>
