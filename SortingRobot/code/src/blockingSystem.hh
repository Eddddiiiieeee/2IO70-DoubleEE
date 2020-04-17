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
#ifndef IBLOCKER_HH
#define IBLOCKER_HH



struct IBlocker
{
#ifndef ENUM_IBlocker_BlockerErrors
#define ENUM_IBlocker_BlockerErrors 1


  struct BlockerErrors
  {
    enum type
    {
      none,pistonExtended,pistonRetracted
    };
  };


#endif // ENUM_IBlocker_BlockerErrors
#ifndef ENUM_IBlocker_State
#define ENUM_IBlocker_State 1


  struct State
  {
    enum type
    {
      Retracted,Extended,Error
    };
  };


#endif // ENUM_IBlocker_State

  struct
  {
    std::function< void(int,int)> trigger;
    std::function< void()> stop;
    std::function< ::IBlocker::BlockerErrors::type()> getErrorState;
  } in;

  struct
  {
    std::function< void()> error;
  } out;

  dzn::port::meta meta;
  inline IBlocker(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.trigger) throw dzn::binding_error(meta, "in.trigger");
    if (! in.stop) throw dzn::binding_error(meta, "in.stop");
    if (! in.getErrorState) throw dzn::binding_error(meta, "in.getErrorState");

    if (! out.error) throw dzn::binding_error(meta, "out.error");

  }
};

inline void connect (IBlocker& provided, IBlocker& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_IBlocker_State
#define ENUM_TO_STRING_IBlocker_State 1
inline std::string to_string(::IBlocker::State::type v)
{
  switch(v)
  {
    case ::IBlocker::State::Retracted: return "State_Retracted";
    case ::IBlocker::State::Extended: return "State_Extended";
    case ::IBlocker::State::Error: return "State_Error";

  }
  return "";
}
#endif // ENUM_TO_STRING_IBlocker_State
#ifndef ENUM_TO_STRING_IBlocker_BlockerErrors
#define ENUM_TO_STRING_IBlocker_BlockerErrors 1
inline std::string to_string(::IBlocker::BlockerErrors::type v)
{
  switch(v)
  {
    case ::IBlocker::BlockerErrors::none: return "BlockerErrors_none";
    case ::IBlocker::BlockerErrors::pistonExtended: return "BlockerErrors_pistonExtended";
    case ::IBlocker::BlockerErrors::pistonRetracted: return "BlockerErrors_pistonRetracted";

  }
  return "";
}
#endif // ENUM_TO_STRING_IBlocker_BlockerErrors

#ifndef STRING_TO_ENUM_IBlocker_State
#define STRING_TO_ENUM_IBlocker_State 1
inline ::IBlocker::State::type to_IBlocker_State(std::string s)
{
  static std::map<std::string, ::IBlocker::State::type> m = {
    {"State_Retracted", ::IBlocker::State::Retracted},
    {"State_Extended", ::IBlocker::State::Extended},
    {"State_Error", ::IBlocker::State::Error},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IBlocker_State
#ifndef STRING_TO_ENUM_IBlocker_BlockerErrors
#define STRING_TO_ENUM_IBlocker_BlockerErrors 1
inline ::IBlocker::BlockerErrors::type to_IBlocker_BlockerErrors(std::string s)
{
  static std::map<std::string, ::IBlocker::BlockerErrors::type> m = {
    {"BlockerErrors_none", ::IBlocker::BlockerErrors::none},
    {"BlockerErrors_pistonExtended", ::IBlocker::BlockerErrors::pistonExtended},
    {"BlockerErrors_pistonRetracted", ::IBlocker::BlockerErrors::pistonRetracted},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IBlocker_BlockerErrors


#endif // IBLOCKER_HH

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
#ifndef BLOCKERCONTROLLER_HH
#define BLOCKERCONTROLLER_HH

#include "interfaces.hh"
#include "interfaces.hh"
#include "ITimer.hh"
#include "ITimer.hh"



struct BlockerController
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;
#ifndef ENUM_BlockerController_State
#define ENUM_BlockerController_State 1


  struct State
  {
    enum type
    {
      Retracted,Extended,Error
    };
  };


#endif // ENUM_BlockerController_State

  bool pistonExtended;
  ::IBlocker::BlockerErrors::type errorState;
  ::BlockerController::State::type state;

  ::IBlocker::BlockerErrors::type reply_IBlocker_BlockerErrors;

  std::function<void ()> out_controller;

  ::IBlocker controller;

  ::IPiston piston;
  ::IBasicSensor sensor;
  ::ITimer pistonTimer;
  ::ITimer timeout;


  BlockerController(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const BlockerController& m)  {
    (void)m;
    return os << "[" << m.pistonExtended <<", " << m.errorState <<", " << m.state <<"]" ;
  }
  private:
  void controller_trigger(int extendTime,int timeoutTime);
  void controller_stop();
  ::IBlocker::BlockerErrors::type controller_getErrorState();
  void sensor_triggered();
  void pistonTimer_timeout();
  void timeout_timeout();

};

#endif // BLOCKERCONTROLLER_HH

/********************************** COMPONENT *********************************/
/********************************** COMPONENT *********************************/
#ifndef PISTON_HH
#define PISTON_HH

#include "interfaces.hh"



struct Piston
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;

  ::IPiston::State::type state;


  std::function<void ()> out_piston;

  ::IPiston piston;



  Piston(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const Piston& m)  {
    (void)m;
    return os << "[" << m.state <<"]" ;
  }
  private:
  void piston_extend();
  void piston_retract();

};

#endif // PISTON_HH

/********************************** COMPONENT *********************************/
/***********************************  SYSTEM  ***********************************/
#ifndef BLOCKER_HH
#define BLOCKER_HH


#include <dzn/locator.hh>

#include "Button.hh"
#include "Timer.hh"
#include "Timer.hh"



struct Blocker
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::BlockerController controller;
  ::Piston piston;
  ::Button button;
  ::Timer pistonTimer;
  ::Timer timeoutTimer;

  ::IBlocker& blocker;


  Blocker(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os=std::clog) const;
};

#endif // BLOCKER_HH

/***********************************  SYSTEM  ***********************************/


//version: 2.9.1
