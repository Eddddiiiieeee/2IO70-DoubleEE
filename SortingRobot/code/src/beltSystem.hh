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
#ifndef IBELT_HH
#define IBELT_HH



struct IBelt
{
#ifndef ENUM_IBelt_State
#define ENUM_IBelt_State 1


  struct State
  {
    enum type
    {
      Idle,Running
    };
  };


#endif // ENUM_IBelt_State

  struct
  {
    std::function< void()> start;
    std::function< void()> stop;
  } in;

  struct
  {
    std::function< void()> error;
  } out;

  dzn::port::meta meta;
  inline IBelt(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.start) throw dzn::binding_error(meta, "in.start");
    if (! in.stop) throw dzn::binding_error(meta, "in.stop");

    if (! out.error) throw dzn::binding_error(meta, "out.error");

  }
};

inline void connect (IBelt& provided, IBelt& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_IBelt_State
#define ENUM_TO_STRING_IBelt_State 1
inline std::string to_string(::IBelt::State::type v)
{
  switch(v)
  {
    case ::IBelt::State::Idle: return "State_Idle";
    case ::IBelt::State::Running: return "State_Running";

  }
  return "";
}
#endif // ENUM_TO_STRING_IBelt_State

#ifndef STRING_TO_ENUM_IBelt_State
#define STRING_TO_ENUM_IBelt_State 1
inline ::IBelt::State::type to_IBelt_State(std::string s)
{
  static std::map<std::string, ::IBelt::State::type> m = {
    {"State_Idle", ::IBelt::State::Idle},
    {"State_Running", ::IBelt::State::Running},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IBelt_State


#endif // IBELT_HH

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
#ifndef BELTCONTROLLER_HH
#define BELTCONTROLLER_HH

#include "interfaces.hh"
#include "interfaces.hh"
#include "ITimer.hh"



struct BeltController
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;
#ifndef ENUM_BeltController_State
#define ENUM_BeltController_State 1


  struct State
  {
    enum type
    {
      Idle,Running,Error
    };
  };


#endif // ENUM_BeltController_State

  int interval;
  ::BeltController::State::type state;


  std::function<void ()> out_controller;

  ::IBelt controller;

  ::IMotor motor;
  ::IBasicSensor sensor;
  ::ITimer timer;


  BeltController(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const BeltController& m)  {
    (void)m;
    return os << "[" << m.interval <<", " << m.state <<"]" ;
  }
  private:
  void controller_start();
  void controller_stop();
  void sensor_triggered();
  void timer_timeout();

};

#endif // BELTCONTROLLER_HH

/********************************** COMPONENT *********************************/
/********************************** COMPONENT *********************************/
#ifndef MOTOR_HH
#define MOTOR_HH

#include "interfaces.hh"



struct Motor
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;
#ifndef ENUM_Motor_State
#define ENUM_Motor_State 1


  struct State
  {
    enum type
    {
      Idle,Running
    };
  };


#endif // ENUM_Motor_State

  ::Motor::State::type state;


  std::function<void ()> out_motor;

  ::IMotor motor;



  Motor(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const Motor& m)  {
    (void)m;
    return os << "[" << m.state <<"]" ;
  }
  private:
  void motor_start();
  void motor_stop();

};

#endif // MOTOR_HH

/********************************** COMPONENT *********************************/
/***********************************  SYSTEM  ***********************************/
#ifndef BELT_HH
#define BELT_HH


#include <dzn/locator.hh>

#include "Button.hh"
#include "Timer.hh"



struct Belt
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::BeltController controller;
  ::Motor motor;
  ::Button button;
  ::Timer timer;

  ::IBelt& belt;


  Belt(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os=std::clog) const;
};

#endif // BELT_HH

/***********************************  SYSTEM  ***********************************/


//version: 2.9.1
