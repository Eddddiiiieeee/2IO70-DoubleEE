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
#ifndef IROBOTCOMMUNICATION_HH
#define IROBOTCOMMUNICATION_HH



struct IRobotCommunication
{
#ifndef ENUM_IRobotCommunication_State
#define ENUM_IRobotCommunication_State 1


  struct State
  {
    enum type
    {
      Idle,Running
    };
  };


#endif // ENUM_IRobotCommunication_State

  struct
  {
    std::function< void()> start;
    std::function< void()> stop;
    std::function< void()> emergencyStop;
    std::function< void()> respondDiskCounters;
    std::function< void()> heartbeat1;
    std::function< void()> heartbeat2;
    std::function< void()> heartbeat3;
    std::function< void()> heartbeat4;
    std::function< void()> tookDisk_in;
    std::function< void()> error_in;
    std::function< void()> respondDisksTaken;
    std::function< void()> reboot;
    std::function< void()> resumeTaking;
    std::function< void()> pauseTaking;
  } in;

  struct
  {
    std::function< void()> emergency;
    std::function< void()> requestDiskCounters;
    std::function< void()> heartbeat;
    std::function< void()> tookDisk_out;
    std::function< void()> error_out;
    std::function< void()> requestDisksTaken;
    std::function< void()> T010;
    std::function< void()> T000;
    std::function< void()> S000;
    std::function< void()> S010;
    std::function< void()> S100;
    std::function< void()> R100;
    std::function< void()> R101;
    std::function< void()> R290;
    std::function< void()> R200;
  } out;

  dzn::port::meta meta;
  inline IRobotCommunication(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.start) throw dzn::binding_error(meta, "in.start");
    if (! in.stop) throw dzn::binding_error(meta, "in.stop");
    if (! in.emergencyStop) throw dzn::binding_error(meta, "in.emergencyStop");
    if (! in.respondDiskCounters) throw dzn::binding_error(meta, "in.respondDiskCounters");
    if (! in.heartbeat1) throw dzn::binding_error(meta, "in.heartbeat1");
    if (! in.heartbeat2) throw dzn::binding_error(meta, "in.heartbeat2");
    if (! in.heartbeat3) throw dzn::binding_error(meta, "in.heartbeat3");
    if (! in.heartbeat4) throw dzn::binding_error(meta, "in.heartbeat4");
    if (! in.tookDisk_in) throw dzn::binding_error(meta, "in.tookDisk_in");
    if (! in.error_in) throw dzn::binding_error(meta, "in.error_in");
    if (! in.respondDisksTaken) throw dzn::binding_error(meta, "in.respondDisksTaken");
    if (! in.reboot) throw dzn::binding_error(meta, "in.reboot");
    if (! in.resumeTaking) throw dzn::binding_error(meta, "in.resumeTaking");
    if (! in.pauseTaking) throw dzn::binding_error(meta, "in.pauseTaking");

    if (! out.emergency) throw dzn::binding_error(meta, "out.emergency");
    if (! out.requestDiskCounters) throw dzn::binding_error(meta, "out.requestDiskCounters");
    if (! out.heartbeat) throw dzn::binding_error(meta, "out.heartbeat");
    if (! out.tookDisk_out) throw dzn::binding_error(meta, "out.tookDisk_out");
    if (! out.error_out) throw dzn::binding_error(meta, "out.error_out");
    if (! out.requestDisksTaken) throw dzn::binding_error(meta, "out.requestDisksTaken");
    if (! out.T010) throw dzn::binding_error(meta, "out.T010");
    if (! out.T000) throw dzn::binding_error(meta, "out.T000");
    if (! out.S000) throw dzn::binding_error(meta, "out.S000");
    if (! out.S010) throw dzn::binding_error(meta, "out.S010");
    if (! out.S100) throw dzn::binding_error(meta, "out.S100");
    if (! out.R100) throw dzn::binding_error(meta, "out.R100");
    if (! out.R101) throw dzn::binding_error(meta, "out.R101");
    if (! out.R290) throw dzn::binding_error(meta, "out.R290");
    if (! out.R200) throw dzn::binding_error(meta, "out.R200");

  }
};

inline void connect (IRobotCommunication& provided, IRobotCommunication& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_IRobotCommunication_State
#define ENUM_TO_STRING_IRobotCommunication_State 1
inline std::string to_string(::IRobotCommunication::State::type v)
{
  switch(v)
  {
    case ::IRobotCommunication::State::Idle: return "State_Idle";
    case ::IRobotCommunication::State::Running: return "State_Running";

  }
  return "";
}
#endif // ENUM_TO_STRING_IRobotCommunication_State

#ifndef STRING_TO_ENUM_IRobotCommunication_State
#define STRING_TO_ENUM_IRobotCommunication_State 1
inline ::IRobotCommunication::State::type to_IRobotCommunication_State(std::string s)
{
  static std::map<std::string, ::IRobotCommunication::State::type> m = {
    {"State_Idle", ::IRobotCommunication::State::Idle},
    {"State_Running", ::IRobotCommunication::State::Running},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IRobotCommunication_State


#endif // IROBOTCOMMUNICATION_HH

/********************************** INTERFACE *********************************/


//version: 2.9.1
