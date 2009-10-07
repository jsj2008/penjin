#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED


#include <SDL/SDL.h>


#include "PenjinTypes.h"

enum TimerScalers
{
    //	Various modes that the timer can be put in.
    FIFTEEN_FRAMES,
    THIRTY_FRAMES,
    FIFTY_FRAMES,
    SIXTY_FRAMES,
    MILLI_SECONDS,
    CENTI_SECONDS,
    DECI_SECONDS,
    SECONDS,
    MINUTES,
    HOURS,
    CUSTOM
};

const static int MY_CLOCKS = 1000 ;

class Timer
{
    public:
        Timer()
        {
            startTicks = 0;
            pausedTicks = 0;
            is_Paused = false;
            is_Started = false;
            mode = SIXTY_FRAMES;
            #ifdef PENJIN_FIXED
                scaler = 0;
            #else
                scaler = 0.0f;
            #endif
        }
        Timer(const TimerScalers& scale)
        {
            startTicks = 0;
            pausedTicks = 0;
            is_Paused = false;
            is_Started = false;
            mode = scale;
            calcScaler();
        }

        void setMode(const TimerScalers& mode)       // Set the timer scaler
        {
            this->mode = mode;
            calcScaler();
        }
        int getScaledTicks(const TimerScalers& mode)
        {
            setMode(mode);
            return getScaledTicks();
        }
        int getScaledTicks()              // Return the remaining coutdown time
        {
            #ifdef PENJIN_FIXED
                return Fixed(getTicks()/scaler).intValue>>16;
            #else
                return (int)((float)getTicks() / scaler);
            #endif
        }
        int getTicks()
        {
            //If the timer is running
            if(is_Started)
            {
                //If the timer is paused
                if(is_Paused)
                {
                    //Return the number of ticks when the the timer was paused
                    return pausedTicks;
                }
                else
                {
                    //Return the current time minus the start time
                    return SDL_GetTicks() - startTicks;
                }
            }
            //If the timer isn't running
            return 0;
        }

        void start()
        {
            //Start the timer
            is_Started = true;
            //Unpause the timer
            is_Paused = false;
            //Get the current clock time
            startTicks = SDL_GetTicks();
        }
        void stop()
        {
            is_Started = false;
            is_Paused = false;
        }
        void pause()
        {
            //If the timer is running and isn't already paused
            if(  is_Started && !is_Paused )
            {
                //Pause the timer
                is_Paused = true;
                //Calculate the paused ticks
                pausedTicks = SDL_GetTicks() - startTicks;
            }
        }
        void unpause()
        {
            //If the timer is paused
            if( is_Paused == true )
            {
                //Unpause the timer
                is_Paused = false;
                //Reset the starting ticks
                startTicks = SDL_GetTicks() - pausedTicks;
                //Reset the paused ticks
                pausedTicks = 0;
            }
        }

        bool isPaused(){return is_Paused;};
        bool isStarted(){return is_Started;};

        /// Set a custom timer resolution.
        #ifdef PENJIN_FIXED
            void setScaler(CRFixed scale){setMode(CUSTOM);scaler =scale;}
            Fixed getScaler(){return scaler;}
        #else
            void setScaler(CRfloat scale){setMode(CUSTOM);scaler =scale;}
            float getScaler(){return scaler;}
        #endif
    private:
        void calcScaler();   // Calculate the resolution of timer updates

        //  when we set a time limit we use the scaler to calc the number of ticks to be reached
        uint mode;
        int startTicks;
        int pausedTicks;
        bool is_Paused;
        bool is_Started;
        #ifdef PENJIN_FIXED
            Fixed scaler;
        #else
            float scaler;
        #endif
};
#endif // TIMER_H_INCLUDED
