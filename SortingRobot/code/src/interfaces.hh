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
#ifndef IBASICSENSOR_HH
#define IBASICSENSOR_HH



struct IBasicSensor
{

  struct
  {
    std::function< void()> activate;
    std::function< void()> deactivate;
  } in;

  struct
  {
    std::function< void()> triggered;
  } out;

  dzn::port::meta meta;
  inline IBasicSensor(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.activate) throw dzn::binding_error(meta, "in.activate");
    if (! in.deactivate) throw dzn::binding_error(meta, "in.deactivate");

    if (! out.triggered) throw dzn::binding_error(meta, "out.triggered");

  }
};

inline void connect (IBasicSensor& provided, IBasicSensor& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}





#endif // IBASICSENSOR_HH

/********************************** INTERFACE *********************************/
/********************************** INTERFACE *********************************/
#ifndef IMOTOR_HH
#define IMOTOR_HH



struct IMotor
{
#ifndef ENUM_IMotor_State
#define ENUM_IMotor_State 1


  struct State
  {
    enum type
    {
      Idle,Running
    };
  };


#endif // ENUM_IMotor_State

  struct
  {
    std::function< void()> start;
    std::function< void()> stop;
  } in;

  struct
  {
  } out;

  dzn::port::meta meta;
  inline IMotor(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.start) throw dzn::binding_error(meta, "in.start");
    if (! in.stop) throw dzn::binding_error(meta, "in.stop");


  }
};

inline void connect (IMotor& provided, IMotor& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_IMotor_State
#define ENUM_TO_STRING_IMotor_State 1
inline std::string to_string(::IMotor::State::type v)
{
  switch(v)
  {
    case ::IMotor::State::Idle: return "State_Idle";
    case ::IMotor::State::Running: return "State_Running";

  }
  return "";
}
#endif // ENUM_TO_STRING_IMotor_State

#ifndef STRING_TO_ENUM_IMotor_State
#define STRING_TO_ENUM_IMotor_State 1
inline ::IMotor::State::type to_IMotor_State(std::string s)
{
  static std::map<std::string, ::IMotor::State::type> m = {
    {"State_Idle", ::IMotor::State::Idle},
    {"State_Running", ::IMotor::State::Running},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IMotor_State


#endif // IMOTOR_HH

/********************************** INTERFACE *********************************/
/********************************** INTERFACE *********************************/
#ifndef IPISTON_HH
#define IPISTON_HH



struct IPiston
{
#ifndef ENUM_IPiston_State
#define ENUM_IPiston_State 1


  struct State
  {
    enum type
    {
      Extended,Retracted
    };
  };


#endif // ENUM_IPiston_State

  struct
  {
    std::function< void()> extend;
    std::function< void()> retract;
  } in;

  struct
  {
  } out;

  dzn::port::meta meta;
  inline IPiston(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.extend) throw dzn::binding_error(meta, "in.extend");
    if (! in.retract) throw dzn::binding_error(meta, "in.retract");


  }
};

inline void connect (IPiston& provided, IPiston& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_IPiston_State
#define ENUM_TO_STRING_IPiston_State 1
inline std::string to_string(::IPiston::State::type v)
{
  switch(v)
  {
    case ::IPiston::State::Extended: return "State_Extended";
    case ::IPiston::State::Retracted: return "State_Retracted";

  }
  return "";
}
#endif // ENUM_TO_STRING_IPiston_State

#ifndef STRING_TO_ENUM_IPiston_State
#define STRING_TO_ENUM_IPiston_State 1
inline ::IPiston::State::type to_IPiston_State(std::string s)
{
  static std::map<std::string, ::IPiston::State::type> m = {
    {"State_Extended", ::IPiston::State::Extended},
    {"State_Retracted", ::IPiston::State::Retracted},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IPiston_State


#endif // IPISTON_HH

/********************************** INTERFACE *********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_BUTTON_HH
#define SKEL_BUTTON_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>




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


//version: 2.9.1
