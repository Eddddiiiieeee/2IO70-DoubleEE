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
      Running,Stopped,ERROR
    };
  };


#endif // ENUM_ISorter_State

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
  inline ISorter(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.start) throw dzn::binding_error(meta, "in.start");
    if (! in.stop) throw dzn::binding_error(meta, "in.stop");

    if (! out.error) throw dzn::binding_error(meta, "out.error");

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
    case ::ISorter::State::ERROR: return "State_ERROR";

  }
  return "";
}
#endif // ENUM_TO_STRING_ISorter_State

#ifndef STRING_TO_ENUM_ISorter_State
#define STRING_TO_ENUM_ISorter_State 1
inline ::ISorter::State::type to_ISorter_State(std::string s)
{
  static std::map<std::string, ::ISorter::State::type> m = {
    {"State_Running", ::ISorter::State::Running},
    {"State_Stopped", ::ISorter::State::Stopped},
    {"State_ERROR", ::ISorter::State::ERROR},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_ISorter_State


#endif // ISORTER_HH

/********************************** INTERFACE *********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_SORTER_HH
#define SKEL_SORTER_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "interfaces.hh"
#include "interfaces.hh"
#include "interfaces.hh"
#include "interfaces.hh"



namespace skel {
  struct Sorter
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::ISorter sorter;

    ::IBasicSensor diskSensor;
    ::IColorSensor colorSensor;
    ::IMotor beltMotor;
    ::IPiston sortingPiston;

    Sorter(const dzn::locator& dzn_locator)
    : dzn_meta{"","Sorter",0,0,{& diskSensor.meta,& colorSensor.meta,& beltMotor.meta,& sortingPiston.meta},{},{[this]{sorter.check_bindings();},[this]{diskSensor.check_bindings();},[this]{colorSensor.check_bindings();},[this]{beltMotor.check_bindings();},[this]{sortingPiston.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , sorter({{"sorter",this,&dzn_meta},{"",0,0}})

    , diskSensor({{"",0,0},{"diskSensor",this,&dzn_meta}})
    , colorSensor({{"",0,0},{"colorSensor",this,&dzn_meta}})
    , beltMotor({{"",0,0},{"beltMotor",this,&dzn_meta}})
    , sortingPiston({{"",0,0},{"sortingPiston",this,&dzn_meta}})

    {
      sorter.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sorter) = false; return sorter_start();}, this->sorter.meta, "start");};
      sorter.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sorter) = false; return sorter_stop();}, this->sorter.meta, "stop");};
      diskSensor.out.triggered = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->diskSensor) = false; return diskSensor_triggered();}, this->diskSensor.meta, "triggered");};
      colorSensor.out.lightDisk = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->colorSensor) = false; return colorSensor_lightDisk();}, this->colorSensor.meta, "lightDisk");};
      colorSensor.out.darkDisk = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->colorSensor) = false; return colorSensor_darkDisk();}, this->colorSensor.meta, "darkDisk");};


    }
    virtual ~ Sorter() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const Sorter& m)  {
      return m.stream_members(os);
    }
    private:
    virtual void sorter_start () = 0;
    virtual void sorter_stop () = 0;
    virtual void diskSensor_triggered () = 0;
    virtual void colorSensor_lightDisk () = 0;
    virtual void colorSensor_darkDisk () = 0;

  };
}

#endif // SORTER_HH

/***********************************  FOREIGN  **********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_DISKSENSOR_HH
#define SKEL_DISKSENSOR_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include "interfaces.hh"



namespace skel {
  struct DiskSensor
  {
    dzn::meta dzn_meta;
    dzn::runtime& dzn_rt;
    dzn::locator const& dzn_locator;
    ::IBasicSensor sensor;


    DiskSensor(const dzn::locator& dzn_locator)
    : dzn_meta{"","DiskSensor",0,0,{},{},{[this]{sensor.check_bindings();}}}
    , dzn_rt(dzn_locator.get<dzn::runtime>())
    , dzn_locator(dzn_locator)

    , sensor({{"sensor",this,&dzn_meta},{"",0,0}})


    {
      sensor.in.activate = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sensor) = false; return sensor_activate();}, this->sensor.meta, "activate");};
      sensor.in.deactivate = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sensor) = false; return sensor_deactivate();}, this->sensor.meta, "deactivate");};


    }
    virtual ~ DiskSensor() {}
    virtual std::ostream& stream_members(std::ostream& os) const { return os; }
    void check_bindings() const;
    void dump_tree(std::ostream& os) const;
    void set_state(std::map<std::string,std::map<std::string,std::string> >){}
    void set_state(std::map<std::string,std::string>_alist){}
    friend std::ostream& operator << (std::ostream& os, const DiskSensor& m)  {
      return m.stream_members(os);
    }
    private:
    virtual void sensor_activate () = 0;
    virtual void sensor_deactivate () = 0;

  };
}

#endif // DISKSENSOR_HH

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
    ::IColorSensor colorSensor;


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
#ifndef BELTMOTOR_HH
#define BELTMOTOR_HH

#include "interfaces.hh"



struct BeltMotor
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;

  ::IMotor::State::type state;


  std::function<void ()> out_motor;

  ::IMotor motor;



  BeltMotor(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const BeltMotor& m)  {
    (void)m;
    return os << "[" << m.state <<"]" ;
  }
  private:
  void motor_start();
  void motor_stop();

};

#endif // BELTMOTOR_HH

/********************************** COMPONENT *********************************/
/********************************** COMPONENT *********************************/
#ifndef SORTINGPISTON_HH
#define SORTINGPISTON_HH

#include "interfaces.hh"



struct SortingPiston
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;

  ::IPiston::State::type state;


  std::function<void ()> out_piston;

  ::IPiston piston;



  SortingPiston(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const SortingPiston& m)  {
    (void)m;
    return os << "[" << m.state <<"]" ;
  }
  private:
  void piston_extend();
  void piston_retract();

};

#endif // SORTINGPISTON_HH

/********************************** COMPONENT *********************************/
/***********************************  SYSTEM  ***********************************/
#ifndef SORTINGSYSTEM_HH
#define SORTINGSYSTEM_HH


#include <dzn/locator.hh>

#include "Sorter.hh"
#include "DiskSensor.hh"
#include "ColorSensor.hh"



struct SortingSystem
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::Sorter sorter;
  ::DiskSensor diskSensor;
  ::ColorSensor colorSensor;
  ::BeltMotor beltMotor;
  ::SortingPiston sortingPiston;

  ::ISorter& sortingSystem;


  SortingSystem(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os=std::clog) const;
};

#endif // SORTINGSYSTEM_HH

/***********************************  SYSTEM  ***********************************/


//version: 2.9.1
