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
#ifndef IRETURNER_HH
#define IRETURNER_HH



struct IReturner
{
#ifndef ENUM_IReturner_State
#define ENUM_IReturner_State 1


  struct State
  {
    enum type
    {
      Running,Stopped,Error
    };
  };


#endif // ENUM_IReturner_State
#ifndef ENUM_IReturner_ReturnerErrors
#define ENUM_IReturner_ReturnerErrors 1


  struct ReturnerErrors
  {
    enum type
    {
      none,R100,R101,R290,R200
    };
  };


#endif // ENUM_IReturner_ReturnerErrors

  struct
  {
    std::function< void()> start;
    std::function< void()> stop;
    std::function< void()> addDisk;
    std::function< ::IReturner::ReturnerErrors::type()> getErrorState;
  } in;

  struct
  {
    std::function< void()> error;
    std::function< void()> warning;
  } out;

  dzn::port::meta meta;
  inline IReturner(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.start) throw dzn::binding_error(meta, "in.start");
    if (! in.stop) throw dzn::binding_error(meta, "in.stop");
    if (! in.addDisk) throw dzn::binding_error(meta, "in.addDisk");
    if (! in.getErrorState) throw dzn::binding_error(meta, "in.getErrorState");

    if (! out.error) throw dzn::binding_error(meta, "out.error");
    if (! out.warning) throw dzn::binding_error(meta, "out.warning");

  }
};

inline void connect (IReturner& provided, IReturner& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_IReturner_State
#define ENUM_TO_STRING_IReturner_State 1
inline std::string to_string(::IReturner::State::type v)
{
  switch(v)
  {
    case ::IReturner::State::Running: return "State_Running";
    case ::IReturner::State::Stopped: return "State_Stopped";
    case ::IReturner::State::Error: return "State_Error";

  }
  return "";
}
#endif // ENUM_TO_STRING_IReturner_State
#ifndef ENUM_TO_STRING_IReturner_ReturnerErrors
#define ENUM_TO_STRING_IReturner_ReturnerErrors 1
inline std::string to_string(::IReturner::ReturnerErrors::type v)
{
  switch(v)
  {
    case ::IReturner::ReturnerErrors::none: return "ReturnerErrors_none";
    case ::IReturner::ReturnerErrors::R100: return "ReturnerErrors_R100";
    case ::IReturner::ReturnerErrors::R101: return "ReturnerErrors_R101";
    case ::IReturner::ReturnerErrors::R290: return "ReturnerErrors_R290";
    case ::IReturner::ReturnerErrors::R200: return "ReturnerErrors_R200";

  }
  return "";
}
#endif // ENUM_TO_STRING_IReturner_ReturnerErrors

#ifndef STRING_TO_ENUM_IReturner_State
#define STRING_TO_ENUM_IReturner_State 1
inline ::IReturner::State::type to_IReturner_State(std::string s)
{
  static std::map<std::string, ::IReturner::State::type> m = {
    {"State_Running", ::IReturner::State::Running},
    {"State_Stopped", ::IReturner::State::Stopped},
    {"State_Error", ::IReturner::State::Error},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IReturner_State
#ifndef STRING_TO_ENUM_IReturner_ReturnerErrors
#define STRING_TO_ENUM_IReturner_ReturnerErrors 1
inline ::IReturner::ReturnerErrors::type to_IReturner_ReturnerErrors(std::string s)
{
  static std::map<std::string, ::IReturner::ReturnerErrors::type> m = {
    {"ReturnerErrors_none", ::IReturner::ReturnerErrors::none},
    {"ReturnerErrors_R100", ::IReturner::ReturnerErrors::R100},
    {"ReturnerErrors_R101", ::IReturner::ReturnerErrors::R101},
    {"ReturnerErrors_R290", ::IReturner::ReturnerErrors::R290},
    {"ReturnerErrors_R200", ::IReturner::ReturnerErrors::R200},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IReturner_ReturnerErrors


#endif // IRETURNER_HH

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
      iTimer.in.start = [&](int milliseconds){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->iTimer) = false; return iTimer_start(milliseconds);}, this->iTimer.meta, "start");};
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
    virtual void iTimer_start (int milliseconds) = 0;
    virtual void iTimer_cancel () = 0;

  };
}

#endif // TIMER_HH

/***********************************  FOREIGN  **********************************/
/********************************** COMPONENT *********************************/
#ifndef RETURNER_HH
#define RETURNER_HH

#include "interfaces.hh"
#include "beltSystem.hh"
#include "beltSystem.hh"
#include "ITimer.hh"



struct Returner
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;

  ::IReturner::State::type state;
  ::IReturner::ReturnerErrors::type errorState;
  int returningTime;
  int queue;

  ::IReturner::ReturnerErrors::type reply_IReturner_ReturnerErrors;

  std::function<void ()> out_returner;

  ::IReturner returner;

  ::IBasicSensor diskDetector;
  ::IBelt firstBelt;
  ::IBelt elevatedBelt;
  ::ITimer timer;


  Returner(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const Returner& m)  {
    (void)m;
    return os << "[" << m.state <<", " << m.errorState <<", " << m.returningTime <<", " << m.queue <<"]" ;
  }
  private:
  void returner_start();
  void returner_stop();
  void returner_addDisk();
  ::IReturner::ReturnerErrors::type returner_getErrorState();
  void diskDetector_triggered();
  void firstBelt_error();
  void elevatedBelt_error();
  void timer_timeout();

};

#endif // RETURNER_HH

/********************************** COMPONENT *********************************/
/***********************************  SYSTEM  ***********************************/
#ifndef RETURNINGSYSTEM_HH
#define RETURNINGSYSTEM_HH


#include <dzn/locator.hh>

#include "DiskDetector.hh"
#include "beltSystem.hh"
#include "beltSystem.hh"
#include "Timer.hh"



struct ReturningSystem
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::Returner returner;
  ::DiskDetector sensor;
  ::Belt firstBelt;
  ::Belt elevatedBelt;
  ::Timer timer;

  ::IReturner& returningSystem;


  ReturningSystem(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os=std::clog) const;
};

#endif // RETURNINGSYSTEM_HH

/***********************************  SYSTEM  ***********************************/


//version: 2.9.1
