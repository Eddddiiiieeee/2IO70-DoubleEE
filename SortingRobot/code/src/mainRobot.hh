// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include <dzn/meta.hh>

namespace dzn {
  struct locator;
  struct runtime;
}



#include <iostream>
#include <map>


/***********************************  FOREIGN  **********************************/
#ifndef SKEL_BUTTON_HH
#define SKEL_BUTTON_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "interfaces.hh"



namespace skel {
  struct Button
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::IBasicSensor sensor;


    Button(const dzn::locator& dzn_locator)
    : dzn_meta{"","Button",0,0,{},{},{[this]{sensor.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , sensor({{"sensor",this,&dzn_meta},{"",0,0}})


    {
      sensor.in.activate = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sensor) = false; return sensor_activate();}, this->sensor.meta, "activate");};
      sensor.in.deactivate = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sensor) = false; return sensor_deactivate();}, this->sensor.meta, "deactivate");};


    }
    virtual ~ Button() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const Button& m)  {
      return m.stream_members(os);
    }
    private:
    virtual void sensor_activate () = 0;
    virtual void sensor_deactivate () = 0;

  };
}

#endif // BUTTON_HH

/***********************************  FOREIGN  **********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_DISKDETECTOR_HH
#define SKEL_DISKDETECTOR_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "interfaces.hh"



namespace skel {
  struct DiskDetector
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::IBasicSensor sensor;


    DiskDetector(const dzn::locator& dzn_locator)
    : dzn_meta{"","DiskDetector",0,0,{},{},{[this]{sensor.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , sensor({{"sensor",this,&dzn_meta},{"",0,0}})


    {
      sensor.in.activate = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sensor) = false; return sensor_activate();}, this->sensor.meta, "activate");};
      sensor.in.deactivate = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sensor) = false; return sensor_deactivate();}, this->sensor.meta, "deactivate");};


    }
    virtual ~ DiskDetector() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const DiskDetector& m)  {
      return m.stream_members(os);
    }
    private:
    virtual void sensor_activate () = 0;
    virtual void sensor_deactivate () = 0;

  };
}

#endif // DISKDETECTOR_HH

/***********************************  FOREIGN  **********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_TIMER_HH
#define SKEL_TIMER_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "ITimer.hh"



namespace skel {
  struct Timer
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::ITimer iTimer;


    Timer(const dzn::locator& dzn_locator)
    : dzn_meta{"","Timer",0,0,{},{},{[this]{iTimer.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , iTimer({{"iTimer",this,&dzn_meta},{"",0,0}})


    {
      iTimer.in.start = [&](unsigned int milliseconds){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->iTimer) = false; return iTimer_start(milliseconds);}, this->iTimer.meta, "start");};
      iTimer.in.cancel = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->iTimer) = false; return iTimer_cancel();}, this->iTimer.meta, "cancel");};


    }
    virtual ~ Timer() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const Timer& m)  {
      return m.stream_members(os);
    }
    private:
    virtual void iTimer_start (unsigned int milliseconds) = 0;
    virtual void iTimer_cancel () = 0;

  };
}

#endif // TIMER_HH

/***********************************  FOREIGN  **********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_COLORSENSOR_HH
#define SKEL_COLORSENSOR_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "interfaces.hh"



namespace skel {
  struct ColorSensor
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::IBasicSensor colorSensor;


    ColorSensor(const dzn::locator& dzn_locator)
    : dzn_meta{"","ColorSensor",0,0,{},{},{[this]{colorSensor.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , colorSensor({{"colorSensor",this,&dzn_meta},{"",0,0}})


    {
      colorSensor.in.activate = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->colorSensor) = false; return colorSensor_activate();}, this->colorSensor.meta, "activate");};
      colorSensor.in.deactivate = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->colorSensor) = false; return colorSensor_deactivate();}, this->colorSensor.meta, "deactivate");};


    }
    virtual ~ ColorSensor() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const ColorSensor& m)  {
      return m.stream_members(os);
    }
    private:
    virtual void colorSensor_activate () = 0;
    virtual void colorSensor_deactivate () = 0;

  };
}

#endif // COLORSENSOR_HH

/***********************************  FOREIGN  **********************************/
/********************************** COMPONENT *********************************/
#ifndef RASPBERRYPI_HH
#define RASPBERRYPI_HH

#include "IRobotCommunication.hh"
#include "sortingSystem.hh"
#include "takingSystem.hh"
#include "returnSystem.hh"
#include "ITimer.hh"
#include "ITimer.hh"
#include "ITimer.hh"
#include "ITimer.hh"
#include "ITimer.hh"



struct RaspberryPi
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;
#ifndef ENUM_RaspberryPi_State
#define ENUM_RaspberryPi_State 1


  struct State
  {
    enum type
    {
      Idle,Running,Error
    };
  };


#endif // ENUM_RaspberryPi_State

  ::RaspberryPi::State::type state;

  ::ISorter::SortingErrors::type reply_ISorter_SortingErrors;
  ::ITaking::TakingErrors::type reply_ITaking_TakingErrors;
  ::IReturner::ReturnerErrors::type reply_IReturner_ReturnerErrors;

  std::function<void ()> out_controller;

  ::IRobotCommunication controller;

  ::ISorter sorting;
  ::ITaking taker;
  ::IReturner returner;
  ::ITimer heartbeatClock;
  ::ITimer heartbeat1;
  ::ITimer heartbeat2;
  ::ITimer heartbeat3;
  ::ITimer heartbeat4;


  RaspberryPi(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const RaspberryPi& m)  {
    (void)m;
    return os << "[" << m.state <<"]" ;
  }
  private:
  void controller_start();
  void controller_stop();
  void controller_emergencyStop();
  void controller_respondDiskCounters();
  void controller_heartbeat1();
  void controller_heartbeat2();
  void controller_heartbeat3();
  void controller_heartbeat4();
  void controller_tookDisk_in();
  void controller_error_in();
  void controller_respondDisksTaken();
  void controller_reboot();
  void controller_resumeTaking();
  void controller_pauseTaking();
  void sorting_error();
  void sorting_returnDisk();
  void taker_tookDisk();
  void taker_error();
  void returner_error();
  void returner_warning();
  void heartbeatClock_timeout();
  void heartbeat1_timeout();
  void heartbeat2_timeout();
  void heartbeat3_timeout();
  void heartbeat4_timeout();

};

#endif // RASPBERRYPI_HH

/********************************** COMPONENT *********************************/
/***********************************  SYSTEM  ***********************************/
#ifndef ROBOT_HH
#define ROBOT_HH


#include <dzn/locator.hh>

#include "takingSystem.hh"
#include "sortingSystem.hh"
#include "returnSystem.hh"
#include "Timer.hh"
#include "Timer.hh"
#include "Timer.hh"
#include "Timer.hh"
#include "Timer.hh"



struct Robot
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::RaspberryPi controller;
  ::TakingSystem taker;
  ::SortingSystem sorter;
  ::ReturningSystem returner;
  ::Timer heartbeatClock;
  ::Timer heartbeat1;
  ::Timer heartbeat2;
  ::Timer heartbeat3;
  ::Timer heartbeat4;

  ::IRobotCommunication& robot;


  Robot(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os=std::clog) const;
};

#endif // ROBOT_HH

/***********************************  SYSTEM  ***********************************/


//version: 2.9.1
