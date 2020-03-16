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
#include "sortingSystem.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



BeltMotor::BeltMotor(const dzn::locator& dzn_locator)
: dzn_meta{"","BeltMotor",0,0,{},{},{[this]{motor.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::IMotor::State::Idle)

, motor({{"motor",this,&dzn_meta},{"",0,0}})



{
  dzn_rt.performs_flush(this) = true;

  motor.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->motor) = false; return motor_start();}, this->motor.meta, "start");};
  motor.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->motor) = false; return motor_stop();}, this->motor.meta, "stop");};





}

void BeltMotor::motor_start()
{

  {
    state = ::IMotor::State::Running;
  }

  return;

}
void BeltMotor::motor_stop()
{

  {
    state = ::IMotor::State::Idle;
  }

  return;

}


void BeltMotor::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void BeltMotor::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}


SortingPiston::SortingPiston(const dzn::locator& dzn_locator)
: dzn_meta{"","SortingPiston",0,0,{},{},{[this]{piston.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::IPiston::State::Retracted)

, piston({{"piston",this,&dzn_meta},{"",0,0}})



{
  dzn_rt.performs_flush(this) = true;

  piston.in.extend = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->piston) = false; return piston_extend();}, this->piston.meta, "extend");};
  piston.in.retract = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->piston) = false; return piston_retract();}, this->piston.meta, "retract");};





}

void SortingPiston::piston_extend()
{
  if (state == ::IPiston::State::Retracted) 
  {
    state = ::IPiston::State::Extended;
  }
  else if (state == ::IPiston::State::Extended) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if ((!(state == ::IPiston::State::Extended) && !(state == ::IPiston::State::Retracted))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void SortingPiston::piston_retract()
{
  if (state == ::IPiston::State::Retracted) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if (state == ::IPiston::State::Extended) 
  {
    state = ::IPiston::State::Retracted;
  }
  else if ((!(state == ::IPiston::State::Extended) && !(state == ::IPiston::State::Retracted))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void SortingPiston::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void SortingPiston::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

SortingSystem::SortingSystem(const dzn::locator& dzn_locator)
: dzn_meta{"","SortingSystem",0,0,{},{& sorter.dzn_meta,& diskSensor.dzn_meta,& colorSensor.dzn_meta,& beltMotor.dzn_meta,& sortingPiston.dzn_meta},{[this]{sortingSystem.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, sorter(dzn_locator)
, diskSensor(dzn_locator)
, colorSensor(dzn_locator)
, beltMotor(dzn_locator)
, sortingPiston(dzn_locator)

, sortingSystem(sorter.sorter)

{


  sorter.dzn_meta.parent = &dzn_meta;
  sorter.dzn_meta.name = "sorter";
  diskSensor.dzn_meta.parent = &dzn_meta;
  diskSensor.dzn_meta.name = "diskSensor";
  colorSensor.dzn_meta.parent = &dzn_meta;
  colorSensor.dzn_meta.name = "colorSensor";
  beltMotor.dzn_meta.parent = &dzn_meta;
  beltMotor.dzn_meta.name = "beltMotor";
  sortingPiston.dzn_meta.parent = &dzn_meta;
  sortingPiston.dzn_meta.name = "sortingPiston";


  connect(diskSensor.sensor, sorter.diskSensor);
  connect(colorSensor.colorSensor, sorter.colorSensor);
  connect(beltMotor.motor, sorter.beltMotor);
  connect(sortingPiston.piston, sorter.sortingPiston);

  dzn::rank(sortingSystem.meta.provides.meta, 0);

}

void SortingSystem::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void SortingSystem::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1