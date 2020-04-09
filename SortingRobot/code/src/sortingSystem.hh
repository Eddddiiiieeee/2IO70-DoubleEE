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


/********************************** INTERFACE *********************************/
#ifndef ISORTER_HH
#define ISORTER_HH



struct ISorter
{
#ifndef ENUM_ISorter_State
#define ENUM_ISorter_State 1


  struct State
  {
    enum type
    {
      Running,Stopped,Error
    };
  };


#endif // ENUM_ISorter_State
#ifndef ENUM_ISorter_SortingErrors
#define ENUM_ISorter_SortingErrors 1


  struct SortingErrors
  {
    enum type
    {
      none,S000,S010,S100
    };
  };


#endif // ENUM_ISorter_SortingErrors

  struct
  {
    std::function< void()> start;
    std::function< void()> stop;
    std::function< ::ISorter::SortingErrors::type()> getErrorState;
  } in;

  struct
  {
    std::function< void()> error;
    std::function< void()> returnDisk;
  } out;

  dzn::port::meta meta;
  inline ISorter(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.start) throw dzn::binding_error(meta, "in.start");
    if (! in.stop) throw dzn::binding_error(meta, "in.stop");
    if (! in.getErrorState) throw dzn::binding_error(meta, "in.getErrorState");

    if (! out.error) throw dzn::binding_error(meta, "out.error");
    if (! out.returnDisk) throw dzn::binding_error(meta, "out.returnDisk");

  }
};

inline void connect (ISorter& provided, ISorter& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_ISorter_State
#define ENUM_TO_STRING_ISorter_State 1
inline std::string to_string(::ISorter::State::type v)
{
  switch(v)
  {
    case ::ISorter::State::Running: return "State_Running";
    case ::ISorter::State::Stopped: return "State_Stopped";
    case ::ISorter::State::Error: return "State_Error";

  }
  return "";
}
#endif // ENUM_TO_STRING_ISorter_State
#ifndef ENUM_TO_STRING_ISorter_SortingErrors
#define ENUM_TO_STRING_ISorter_SortingErrors 1
inline std::string to_string(::ISorter::SortingErrors::type v)
{
  switch(v)
  {
    case ::ISorter::SortingErrors::none: return "SortingErrors_none";
    case ::ISorter::SortingErrors::S000: return "SortingErrors_S000";
    case ::ISorter::SortingErrors::S010: return "SortingErrors_S010";
    case ::ISorter::SortingErrors::S100: return "SortingErrors_S100";

  }
  return "";
}
#endif // ENUM_TO_STRING_ISorter_SortingErrors

#ifndef STRING_TO_ENUM_ISorter_State
#define STRING_TO_ENUM_ISorter_State 1
inline ::ISorter::State::type to_ISorter_State(std::string s)
{
  static std::map<std::string, ::ISorter::State::type> m = {
    {"State_Running", ::ISorter::State::Running},
    {"State_Stopped", ::ISorter::State::Stopped},
    {"State_Error", ::ISorter::State::Error},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_ISorter_State
#ifndef STRING_TO_ENUM_ISorter_SortingErrors
#define STRING_TO_ENUM_ISorter_SortingErrors 1
inline ::ISorter::SortingErrors::type to_ISorter_SortingErrors(std::string s)
{
  static std::map<std::string, ::ISorter::SortingErrors::type> m = {
    {"SortingErrors_none", ::ISorter::SortingErrors::none},
    {"SortingErrors_S000", ::ISorter::SortingErrors::S000},
    {"SortingErrors_S010", ::ISorter::SortingErrors::S010},
    {"SortingErrors_S100", ::ISorter::SortingErrors::S100},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_ISorter_SortingErrors


#endif // ISORTER_HH

/********************************** INTERFACE *********************************/
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
/********************************** COMPONENT *********************************/
#ifndef SORTER_HH
#define SORTER_HH

#include "interfaces.hh"
#include "interfaces.hh"
#include "beltSystem.hh"
#include "blockingSystem.hh"
#include "ITimer.hh"
#include "ITimer.hh"



struct Sorter
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;

  unsigned int extendTime;
  unsigned int timeoutTime;
  unsigned int diskScanInterval;
  ::ISorter::SortingErrors::type errorState;
  ::ISorter::State::type state;

  ::ISorter::SortingErrors::type reply_ISorter_SortingErrors;
  ::IBlocker::BlockerErrors::type reply_IBlocker_BlockerErrors;

  std::function<void ()> out_sorter;

  ::ISorter sorter;

  ::IBasicSensor diskDetector;
  ::IBasicSensor colorSensor;
  ::IBelt belt;
  ::IBlocker blocker;
  ::ITimer detectorTimer;
  ::ITimer colorTimer;


  Sorter(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const Sorter& m)  {
    (void)m;
    return os << "[" << m.extendTime <<", " << m.timeoutTime <<", " << m.diskScanInterval <<", " << m.errorState <<", " << m.state <<"]" ;
  }
  private:
  void sorter_start();
  void sorter_stop();
  ::ISorter::SortingErrors::type sorter_getErrorState();
  void diskDetector_triggered();
  void colorSensor_triggered();
  void belt_error();
  void blocker_error();
  void detectorTimer_timeout();
  void colorTimer_timeout();

};

#endif // SORTER_HH

/********************************** COMPONENT *********************************/
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
/***********************************  SYSTEM  ***********************************/
#ifndef SORTINGSYSTEM_HH
#define SORTINGSYSTEM_HH


#include <dzn/locator.hh>

#include "DiskDetector.hh"
#include "ColorSensor.hh"
#include "beltSystem.hh"
#include "blockingSystem.hh"
#include "Timer.hh"
#include "Timer.hh"



struct SortingSystem
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::Sorter sorter;
  ::DiskDetector diskDetector;
  ::ColorSensor colorSensor;
  ::Belt belt;
  ::Blocker blocker;
  ::Timer detectorTimer;
  ::Timer colorTimer;

  ::ISorter& sortingSystem;


  SortingSystem(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os=std::clog) const;
};

#endif // SORTINGSYSTEM_HH

/***********************************  SYSTEM  ***********************************/


//version: 2.9.1
