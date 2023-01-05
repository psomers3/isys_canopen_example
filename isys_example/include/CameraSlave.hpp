#pragma once
#include <lely/coapp/slave.hpp>
#include <bits/stdc++.h>



using namespace lely;


class CameraSlave : public canopen::BasicSlave
{
public:
    using BasicSlave::BasicSlave;
    
    /**
     * Fill this function with things needed to create the camera
     * and other fancy things you need for your math. It is
     * probably better to call this somewhere that it is called
     * by either a received command or on startup, but I'm too
     * lazy to look for those things right now. I would also move
     * the thread creation to a different function.
     * NOTE: Make sure to initialize all private member variables
     * to something as soon as possible (i.e. here). Normally this
     * is done in the constructor, but I didn't feel like overwriting
     * all the different parent constructors since I don't know 
     * which one you'll use. Really it's only important for pointers,
     * but hey, good practices are good practices.
     * 
     **/
    void initialize();

protected:

    /**This function gets called every time a value is read from the local object
     * dictionary by an SDO or RPDO.
     **/
    std::error_code on_read();

private:
    uint16_t _read_value_idx;  // not currently used.
    uint16_t _write_value_idx;  // index to send values to
    uint8_t _value_subidx;  // sub index for sending values
    std::thread _camera_thread;  // thread to manage the task
    bool _last_value_read;  // whether the last sent value has been read yet
    std::mutex _value_mutex;  // mutex protecting _last_value_read

    /**
     * The thread that runs in the background. You can do whatever here.
     **/
    void worker();
};