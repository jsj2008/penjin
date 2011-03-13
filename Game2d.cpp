/*
	Penjin is Copyright (c)2005, 2006, 2007, 2008, 2009, 2010, 2011 Kevin Winfield-Pantoja

	This file is part of Penjin.

	Penjin is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Penjin is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Penjin.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "Game2d.h"
#include "StateTitle.h"
#include "GFX.h"
#include "StringUtility.h"
using Penjin::Game2d;
using Penjin::ApplicationState;

Game2d::Game2d()
{
    //ctor
    Penjin::Errors e = PENJIN_OK;
    Uint32 flags=0;
    #if defined (PENJIN_SDL_VIDEO) || defined(PENJIN_SDL)
        flags = flags | SDL_INIT_VIDEO;
    #endif
    #if defined (PENJIN_SDL_AUDIO) || defined(PENJIN_SDL)
        //  Set SDL_Mixer
        flags = flags | SDL_INIT_AUDIO;
    #endif
    #if defined (PENJIN_SDL_INPUT) || defined(PENJIN_SDL)
        //  Setup SDL input handling
        flags = flags | SDL_INIT_JOYSTICK;
    #endif
    #if defined (PENJIN_SDL_TIMER) || defined(PENJIN_SDL)
        //  Setup SDL timer
        flags = flags | SDL_INIT_TIMER;
    #endif
    if(flags != 0)
    {
        //  TODO handle error properly
        if(SDL_Init(flags) < 0)
            e = PENJIN_SDL_SOMETHING_FAILED;
    }

    //  Load video config
    if(e == PENJIN_OK)
    {
        Penjin::ConfigFile cfg;
        cfg.load(Penjin::CONFIG_FILE);
        Penjin::GFX::getInstance()->setWidth(Penjin::StringUtility::stringToInt(cfg.getValue("Video","Width")));
        Penjin::GFX::getInstance()->setHeight(Penjin::StringUtility::stringToInt(cfg.getValue("Video","Height")));
        Penjin::GFX::getInstance()->setBitsPerPixel(Penjin::StringUtility::stringToInt(cfg.getValue("Video","BitsPerPixel")));
        Penjin::GFX::getInstance()->setFullscreen(Penjin::StringUtility::stringToBool(cfg.getValue("Video","Fullscreen")));
    }
    if(flags)
        Penjin::GFX::getInstance()->applyVideoSettings();
}

Game2d::~Game2d()
{
    //dtor
}


void Game2d::loop()
{
    while(!state->getShouldQuit())
    {
        state->input();
        state->update();
        state->render();
        Penjin::GFX::getInstance()->blit();
    }
}
