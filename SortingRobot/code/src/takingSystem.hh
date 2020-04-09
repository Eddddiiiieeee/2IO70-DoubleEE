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
#ifndef ITAKING_HH
#define ITAKING_HH



struct ITaking
{
#ifndef ENUM_ITaking_State
#define ENUM_ITaking_State 1


  struct State
  {
    enum type
    {
      Running,Stopped,Error
    };
  };


#endif // ENUM_ITaking_State
#ifndef ENUM_ITaking_TakingErrors
#define ENUM_ITaking_TakingErrors 1


  struct TakingErrors
  {
    enum type
    {
      none,T000,T010
    };
  };


#endif // ENUM_ITaking_TakingErrors

  struct
  {
    std::function< void()> start;
    std::function< void()> stop;
    std::function< ::ITaking::TakingErrors::type()> getErrorState;
  } in;

  struct
  {
    std::function< void()> tookDisk;
    std::function< void()> error;
  } out;

  dzn::port::meta meta;
  inline ITaking(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.start) throw dzn::binding_error(meta, "in.start");
    if (! in.stop) throw dzn::binding_error(meta, "in.stop");
    if (! in.getErrorState) throw dzn::binding_error(meta, "in.getErrorState");

    if (! out.tookDisk) throw dzn::binding_error(meta, "out.tookDisk");
    if (! out.error) throw dzn::binding_error(meta, "out.error");

  }
};

inline void connect (ITaking& provided, ITaking& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_ITaking_State
#define ENUM_TO_STRING_ITaking_State 1
inline std::string to_string(::ITaking::State::type v)
{
  switch(v)
  {
    case ::ITaking::State::Running: return "State_Running";
    case ::ITaking::State::Stopped: return "State_Stopped";
    case ::ITaking::State::Error: return "State_Error";

  }
  return "";
}
#endif // ENUM_TO_STRING_ITaking_State
#ifndef ENUM_TO_STRING_ITaking_TakingErrors
#define ENUM_TO_STRING_ITaking_TakingErrors 1
inline std::string to_string(::ITaking::TakingErrors::type v)
{
  switch(v)
  {
    case ::ITaking::TakingErrors::none: return "TakingErrors_none";
    case ::ITaking::TakingErrors::T000: return "TakingErrors_T000";
    case ::ITaking::TakingErrors::T010: return "TakingErrors_T010";

  }
  return "";
}
#endif // ENUM_TO_STRING_ITaking_TakingErrors

#ifndef STRING_TO_ENUM_ITaking_State
#define STRING_TO_ENUM_ITaking_State 1
inline ::ITaking::State::type to_ITaking_State(std::string s)
{
  static std::map<std::string, ::ITaking::State::type> m = {
    {"State_Running", ::ITaking::State::Running},
    {"State_Stopped", ::ITaking::State::Stopped},
    {"State_Error", ::ITaking::State::Error},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_ITaking_State
#ifndef STRING_TO_ENUM_ITaking_TakingErrors
#define STRING_TO_ENUM_ITaking_TakingErrors 1
inline ::ITaking::TakingErrors::type to_ITaking_TakingErrors(std::string s)
{
  static std::map<std::string, ::ITaking::TakingErrors::type> m = {
    {"TakingErrors_none", ::ITaking::TakingErrors::none},
    {"TakingErrors_T000", ::ITaking::TakingErrors::T000},
    {"TakingErrors_T010", ::ITaking::TakingErrors::T010},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_ITaking_TakingErrors


#endif // ITAKING_HH

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
#ifndef TAKER_HH
#define TAKER_HH

#include "blockingSystem.hh"
#include "interfaces.hh"



struct Taker
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;

  unsigned int extendTime;
  unsigned int timeoutTime;
  ::ITaking::State::type state;
  ::ITaking::TakingErrors::type errorState;

  ::ITaking::TakingErrors::type reply_ITaking_TakingErrors;
  ::IBlocker::BlockerErrors::type reply_IBlocker_BlockerErrors;

  std::function<void ()> out_taker;

  ::ITaking taker;

  ::IBlocker blocker;
  ::IBasicSensor diskDetector;


  Taker(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const Taker& m)  {
    (void)m;
    return os << "[" << m.extendTime <<", " << m.timeoutTime <<", " << m.state <<", " << m.errorState <<"]" ;
  }
  private:
  void taker_start();
  void taker_stop();
  ::ITaking::TakingErrors::type taker_getErrorState();
  void blocker_error();
  void diskDetector_triggered();

};

#endif // TAKER_HH

/********************************** COMPONENT *********************************/
/***********************************  SYSTEM  ***********************************/
#ifndef TAKINGSYSTEM_HH
#define TAKINGSYSTEM_HH


#include <dzn/locator.hh>

#include "DiskDetector.hh"
#include "blockingSystem.hh"



struct TakingSystem
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::Taker taker;
  ::DiskDetector detector;
  ::Blocker blocker;

  ::ITaking& takingSystem;


  TakingSystem(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os=std::clog) const;
};

#endif // TAKINGSYSTEM_HH

/***********************************  SYSTEM  ***********************************/


//version: 2.9.1
