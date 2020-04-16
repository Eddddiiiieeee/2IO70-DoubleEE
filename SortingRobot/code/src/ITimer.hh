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
#ifndef ITIMER_HH
#define ITIMER_HH



struct ITimer
{
#ifndef ENUM_ITimer_State
#define ENUM_ITimer_State 1


  struct State
  {
    enum type
    {
      Idle,Running
    };
  };


#endif // ENUM_ITimer_State

  struct
  {
    std::function< void(int)> start;
    std::function< void()> cancel;
  } in;

  struct
  {
    std::function< void()> timeout;
  } out;

  dzn::port::meta meta;
  inline ITimer(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.start) throw dzn::binding_error(meta, "in.start");
    if (! in.cancel) throw dzn::binding_error(meta, "in.cancel");

    if (! out.timeout) throw dzn::binding_error(meta, "out.timeout");

  }
};

inline void connect (ITimer& provided, ITimer& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_ITimer_State
#define ENUM_TO_STRING_ITimer_State 1
inline std::string to_string(::ITimer::State::type v)
{
  switch(v)
  {
    case ::ITimer::State::Idle: return "State_Idle";
    case ::ITimer::State::Running: return "State_Running";

  }
  return "";
}
#endif // ENUM_TO_STRING_ITimer_State

#ifndef STRING_TO_ENUM_ITimer_State
#define STRING_TO_ENUM_ITimer_State 1
inline ::ITimer::State::type to_ITimer_State(std::string s)
{
  static std::map<std::string, ::ITimer::State::type> m = {
    {"State_Idle", ::ITimer::State::Idle},
    {"State_Running", ::ITimer::State::Running},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_ITimer_State


#endif // ITIMER_HH

/********************************** INTERFACE *********************************/
/***********************************  FOREIGN  **********************************/
#ifndef SKEL_TIMER_HH
#define SKEL_TIMER_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>




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


//version: 2.9.1
