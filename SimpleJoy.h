#ifndef SIMPLEJOY_H
#define SIMPLEJOY_H

#include "PenjinTypes.h"
#include "KeyMapper.h"

#if defined(PLATFORM_PANDORA)
#include <linux/input.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#endif

/*
TODO: Add Wii Controls/GBA/NDS etc
*/
class SimpleJoy
{
    public:
        enum sjSTATUS
        {
            sjRELEASED = 0,
            sjPRESSED = 1,
            sjHELD=2
        };
        SimpleJoy();
        ~SimpleJoy();

        PENJIN_ERRORS loadControlMap(CRstring filename)
        {
            PENJIN_ERRORS t = mapper.loadControlMap(filename);
            if(t == PENJIN_OK)
                mapLoaded = true;
            return t;
        }

        void update();					//	Get current status of keyboard

        sjSTATUS isStart()const{return Start;}
        sjSTATUS isSelect()const{return Select;}
        sjSTATUS isUp()const{return Up;}
        sjSTATUS isDown()const{return Down;}
        sjSTATUS isLeft()const{return Left;}
        sjSTATUS isRight()const{return Right;}
        sjSTATUS isA()const{return A;}
        sjSTATUS isB()const{return B;}
        sjSTATUS isX()const{return X;}
        sjSTATUS isY()const{return Y;}
        sjSTATUS isL()const{return L;}
        sjSTATUS isR()const{return R;}
        /// The fabled ANY button.
        sjSTATUS isAny()const
        {
            if((A || B || X || Y || L || R || Start || Select) == sjPRESSED)
                return sjPRESSED;
            else if((A || B || X || Y || L || R || Start || Select) == sjHELD)
                return sjHELD;
            return sjRELEASED;
        }
    #ifdef PLATFORM_PC
        sjSTATUS isQuit()const{return Quit;}
    #endif
    #if defined(PLATFORM_GP2X) || defined(PLATFORM_PC)
        /// GP2X Buttons
        sjSTATUS isClick()const{return Click;}
        sjSTATUS isVolumeUp()const{return VolumeUp;}
        sjSTATUS isVolumeDown()const{return VolumeDown;}
        sjSTATUS isUpLeft()const{return UpLeft;}
        sjSTATUS isUpRight()const{return UpRight;}
        sjSTATUS isDownLeft()const{return DownLeft;}
        sjSTATUS isDownRight()const{return DownRight;}
    #else
        sjSTATUS isUpLeft()const{return sjRELEASED;}
        sjSTATUS isUpRight()const{return sjRELEASED;}
        sjSTATUS isDownLeft()const{return sjRELEASED;}
        sjSTATUS isDownRight()const{return sjRELEASED;}
    #endif

        /// Joystick
        void setDeadZone(const Vector2di& zone){deadZone = zone;}
        void setDeadZoneX(CRint x){deadZone.x = x;}
        void setDeadZoneY(CRint y){deadZone.y = y;}
        void setScaler(CRfloat s){scaler = s;}
        Vector2di getLeftStick()const{return leftStick;}
        Vector2di getRightStick()const{return rightStick;}
        int getLeftStickX()const{return leftStick.x;}
        int getLeftStickY()const{return leftStick.y;}
        int getRightStickX()const{return rightStick.x;}
        int getRightStickY()const{return rightStick.y;}

        /// Joystick logical
        sjSTATUS isLeftStick()const
        {
            if((isLeftStickDown() || isLeftStickLeft() || isLeftStickRight() || isLeftStickUp())==sjHELD)
                return sjHELD;
            else if((isLeftStickDown() || isLeftStickLeft() || isLeftStickRight() || isLeftStickUp())==sjPRESSED)
                return sjPRESSED;
            return sjRELEASED;
        }
        sjSTATUS isLeftStickUp()const
        {
            if(getLeftStickY() < -deadZone.y)
                return sjHELD;
            return sjRELEASED;
        }
        sjSTATUS isLeftStickDown()const
        {
            if(getLeftStickY() > deadZone.y)
                return sjHELD;
            return sjRELEASED;
        }
        sjSTATUS isLeftStickLeft()const
        {
            if(getLeftStickX() < -deadZone.x)
                return sjHELD;
            return sjRELEASED;
        }
        sjSTATUS isLeftStickRight()const
        {
            if(getLeftStickX() > deadZone.x)
                return sjHELD;
            return sjRELEASED;
        }

        sjSTATUS isRightStick()const
        {
            if((isRightStickDown() || isRightStickLeft() || isRightStickRight() || isRightStickUp())==sjHELD)
                return sjHELD;
            else if((isRightStickDown() || isRightStickLeft() || isRightStickRight() || isRightStickUp())==sjPRESSED)
                return sjPRESSED;
            return sjRELEASED;
        }
        sjSTATUS isRightStickUp()const
        {
            if(getRightStickY() < -deadZone.y)
                return sjHELD;
            return sjRELEASED;
        }
        sjSTATUS isRightStickDown()const
        {
            if(getRightStickY() > deadZone.y)
                return sjHELD;
            return sjRELEASED;
        }
        sjSTATUS isRightStickLeft()const
        {
            if(getRightStickX() < -deadZone.x)
                return sjHELD;
            return sjRELEASED;
        }
        sjSTATUS isRightStickRight()const
        {
            if(getRightStickX() > deadZone.x)
                return sjHELD;
            return sjRELEASED;
        }

        /// Mouse
        Vector2di getMouse()const{return mouse;}
        int getMouseX()const{return mouse.x;}
        int getMouseY()const{return mouse.y;}
        sjSTATUS isLeftClick()const{return leftClick;}
        sjSTATUS isRightClick()const{return rightClick;}
        /// TouchScreen - Just a wrapper to mouse
        Vector2di getTouch()const{return mouse;}
        int getTouchX()const{return mouse.x;}
        int getTouchY()const{return mouse.y;}
        sjSTATUS isTouch()const{return leftClick;}

#if defined(PLATFORM_PANDORA)
        /// Nub's
        Vector2di getNubLeft()const{return nubL;}
        int getNubLeftX()const{return nubL.x;}
        int getNubLeftY()const{return nubL.y;}
        Vector2di getNubRight()const{return nubR;}
        int getNubRightX()const{return nubR.x;}
        int getNubRightY()const{return nubR.y;}
#endif

        void resetKeys();

        /// Status
        void joystickStatus();
    private:
        void mappedDown(const SIMPLEJOY_MAP& map);
        void mappedUp(const SIMPLEJOY_MAP& map);
        void mappedMouseAxes(const SIMPLEJOY_MAP& map,CRuchar axis);
        void mappedJoyAxes(const SIMPLEJOY_MAP& map);

        SDL_Joystick *Joy;		//	SDL joystick
        SDL_Event Event;
        sjSTATUS Start, Select, Up, Down, Left, Right, A, B, X, Y, L, R;
    #ifdef PLATFORM_PC
        sjSTATUS Quit;
    #endif
        Vector2di deadZone;
        float scaler;
        Vector2di leftStick;
        Vector2di rightStick;
        Vector2di mouse;
        Vector2di oldMouse;
        sjSTATUS leftClick, rightClick;

    #if defined(PLATFORM_GP2X) || defined(PLATFORM_PC)
        sjSTATUS Click, VolumeUp, VolumeDown,UpLeft, UpRight, DownLeft, DownRight;
    #endif
        KeyMapper mapper;
        bool mapLoaded;

    #if defined(PLATFORM_PANDORA)
        #define DEV_NUBL 1
        #define DEV_NUBR 2
        #define PND_NUBL "vsense66"
        #define PND_NUBR "vsense67"
        #define NUB_CUTOFF 5
        #define NUB_SCALE  10

        int PND_OpenEventDeviceByName( char device_name[] );
        void PND_ReadEvents( int fd, int device );
        void PND_CheckEvent( struct input_event *event, int device );

        int fd_nubL, fd_nubR, rd, version, i;
        struct input_event ev[64];
        char event_name[30];
        char dev_name[256]; //= "Unknown";
        unsigned short id[4];
        Vector2di nubL, nubR;
        // KEV nub code
        void MappedNubAxes(const SIMPLEJOY_MAP& map, CRint axis);

    #endif
};

#endif // SIMPLEJOY_H
