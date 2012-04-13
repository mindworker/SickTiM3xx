/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2003  
 *     Brian Gerkey, Andrew Howard
 *                      
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*
 * Desc: A simple example of how to write a driver that supports multiple interface.
 * Also demonstrates use of a driver as a loadable object.
 * Author: Andrew Howard
 * Date: 25 July 2004
 * CVS: $Id: multidriver.cc,v 1.2 2011/07/20 12:18:32 sfz-locher Exp $
 */


// ONLY if you need something that was #define'd as a result of configure 
// (e.g., HAVE_CFMAKERAW), then #include <config.h>, like so:
/*
#include <config.h>
*/

#if !defined (WIN32)
  #include <unistd.h>
  #include <netinet/in.h>
#endif
#include <string.h>

#include <libplayercore/playercore.h>


////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class Idefix : public ThreadedDriver
{
  public:
    
    // Constructor; need that
    Idefix(ConfigFile* cf, int section);

    virtual int ProcessMessage(QueuePointer & resp_queue, 
                               player_msghdr * hdr, 
                               void * data);

  private:
    // Main function for device thread.
    virtual void Main();
    virtual int MainSetup();
    virtual void MainQuit();

    // My position interface
    player_devaddr_t m_position_addr;
    // My laser interface
    player_devaddr_t m_sonar_addr;
};


// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver* MultiDriver_Init(ConfigFile* cf, int section)
{
  // Create and return a new instance of this driver
  return ((Driver*) (new Idefix(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void MultiDriver_Register(DriverTable* table)
{
  table->AddDriver("multidriver", MultiDriver_Init);
}


////////////////////////////////////////////////////////////////////////////////
// Extra stuff for building a shared object.

/* need the extern to avoid C++ name-mangling  */
extern "C"
{
  int player_driver_init(DriverTable* table)
  {
    MultiDriver_Register(table);
    return(0);
  }
}


////////////////////////////////////////////////////////////////////////////////
// Constructor.  Retrieve options from the configuration file and do any
// pre-Setup() setup.
Idefix::Idefix(ConfigFile* cf, int section)
    : ThreadedDriver(cf, section)
{
  // Create my position interface
  if (cf->ReadDeviceAddr(&(this->m_position_addr), section, 
                         "provides", PLAYER_POSITION2D_CODE, -1, NULL) != 0)
  {
    this->SetError(-1);
    return;
  }  
  if (this->AddInterface(this->m_position_addr))
  {
    this->SetError(-1);    
    return;
  }

  // Create my laser interface
  if (cf->ReadDeviceAddr(&(this->m_sonar_addr), section,
                         "provides", PLAYER_SONAR_CODE, -1, NULL) != 0)
  {
    this->SetError(-1);
    return;
  }    
  if (this->AddInterface(this->m_sonar_addr))
  {
    this->SetError(-1);        
    return;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int Idefix::MainSetup()
{   
  puts("Example driver initialising");

  // Here you do whatever else is necessary to setup the device, like open and
  // configure a serial port.
    
  puts("Example driver ready");
  return(0);
}


////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
void Idefix::MainQuit()
{
  puts("Shutting example driver down");


  // Here you would shut the device down by, for example, closing a
  // serial port.
}


////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void Idefix::Main() 
{
  // The main loop; interact with the device here
  for(;;)
  {
    // test if we are supposed to cancel
    pthread_testcancel();

    // Process incoming messages.  Calls ProcessMessage() on each pending
    // message.
    this->ProcessMessages();

    // Do work here.  
    //
    // Send out new messages with Driver::Publish()
    //
    // For example, to send a new position pose message:
    player_position2d_data_t posdata;
    posdata.pos.px = 43.2;
    posdata.pos.py = -12.2;
    posdata.pos.pa = M_PI/3.0;
    posdata.vel.px = 0.25;
    posdata.vel.py = 0.0;
    posdata.vel.pa = -M_PI/6.0;
    posdata.stall = 0;

    this->Publish(this->m_position_addr, 
                  PLAYER_MSGTYPE_DATA, PLAYER_POSITION2D_DATA_STATE,
                  (void*)&posdata, sizeof(posdata), NULL);

    
    // Sleep (or you might, for example, block on a read() instead)
    usleep(100000);
  }
  return;
}


int Idefix::ProcessMessage(QueuePointer & resp_queue, 
                                player_msghdr * hdr, 
                                void * data)
{
  // Tell the caller that you don't know how to handle this message
  return(-1);
}

