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
#include "blockingSystem.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



BlockerController::BlockerController(const dzn::locator& dzn_locator)
: dzn_meta{"","BlockerController",0,0,{& piston.meta,& sensor.meta,& pistonTimer.meta,& timeout.meta},{},{[this]{controller.check_bindings();},[this]{piston.check_bindings();},[this]{sensor.check_bindings();},[this]{pistonTimer.check_bindings();},[this]{timeout.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, pistonExtended(false), errorState(::IBlocker::BlockerErrors::none), state(::BlockerController::State::Retracted)

, controller({{"controller",this,&dzn_meta},{"",0,0}})

, piston({{"",0,0},{"piston",this,&dzn_meta}})
, sensor({{"",0,0},{"sensor",this,&dzn_meta}})
, pistonTimer({{"",0,0},{"pistonTimer",this,&dzn_meta}})
, timeout({{"",0,0},{"timeout",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  controller.in.trigger = [&](int extendTime,int timeoutTime){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_trigger(extendTime,timeoutTime);}, this->controller.meta, "trigger");};
  controller.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_stop();}, this->controller.meta, "stop");};
  sensor.out.triggered = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sensor) = false; return sensor_triggered();}, this->sensor.meta, "triggered");};
  pistonTimer.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->pistonTimer) = false; return pistonTimer_timeout();}, this->pistonTimer.meta, "timeout");};
  timeout.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->timeout) = false; return timeout_timeout();}, this->timeout.meta, "timeout");};


  controller.in.getErrorState = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_getErrorState();}, this->controller.meta, "getErrorState");};



}

void BlockerController::controller_trigger(int extendTime,int timeoutTime)
{
  if (state == ::BlockerController::State::Retracted) 
  {
    this->sensor.in.deactivate();
    this->sensor.in.activate();
    this->pistonTimer.in.start(extendTime);
    this->timeout.in.start(timeoutTime);
    this->piston.in.extend();
    state = ::BlockerController::State::Extended;
  }
  else if ((state == ::BlockerController::State::Extended && pistonExtended)) 
  {
    this->pistonTimer.in.cancel();
    this->pistonTimer.in.start(extendTime);
  }
  else if ((state == ::BlockerController::State::Extended && !(pistonExtended))) 
  {
    this->pistonTimer.in.cancel();
    this->timeout.in.cancel();
    this->sensor.in.deactivate();
    this->pistonTimer.in.start(extendTime);
    this->timeout.in.start(timeoutTime);
    this->sensor.in.activate();
  }
  else if (state == ::BlockerController::State::Error) ;
  else if ((!(state == ::BlockerController::State::Error) && (!((state == ::BlockerController::State::Extended && !(pistonExtended))) && (!((state == ::BlockerController::State::Extended && pistonExtended)) && !(state == ::BlockerController::State::Retracted))))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void BlockerController::controller_stop()
{
  if (state == ::BlockerController::State::Retracted) ;
  else if (state == ::BlockerController::State::Extended) 
  {
    this->piston.in.retract();
    pistonExtended = false;
    this->pistonTimer.in.cancel();
    this->timeout.in.cancel();
    this->sensor.in.deactivate();
    state = ::BlockerController::State::Retracted;
    this->sensor.in.activate();
  }
  else if ((state == ::BlockerController::State::Error && !(pistonExtended))) 
  {
    state = ::BlockerController::State::Retracted;
    this->sensor.in.activate();
    errorState = ::IBlocker::BlockerErrors::none;
  }
  else if ((state == ::BlockerController::State::Error && pistonExtended)) 
  {
    this->piston.in.retract();
    pistonExtended = !(pistonExtended);
    state = ::BlockerController::State::Retracted;
    this->sensor.in.activate();
    errorState = ::IBlocker::BlockerErrors::none;
  }
  else if ((!((state == ::BlockerController::State::Error && pistonExtended)) && (!((state == ::BlockerController::State::Error && !(pistonExtended))) && (!(state == ::BlockerController::State::Extended) && !(state == ::BlockerController::State::Retracted))))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
::IBlocker::BlockerErrors::type BlockerController::controller_getErrorState()
{
  if (state == ::BlockerController::State::Retracted) 
  {
    { this->reply_IBlocker_BlockerErrors = errorState; }
  }
  else if (state == ::BlockerController::State::Extended) 
  {
    { this->reply_IBlocker_BlockerErrors = errorState; }
  }
  else if (state == ::BlockerController::State::Error) 
  {
    { this->reply_IBlocker_BlockerErrors = errorState; }
  }
  else if ((!(state == ::BlockerController::State::Error) && (!(state == ::BlockerController::State::Extended) && !(state == ::BlockerController::State::Retracted)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return this->reply_IBlocker_BlockerErrors;
}
void BlockerController::sensor_triggered()
{
  if (state == ::BlockerController::State::Retracted) 
  {
    this->controller.out.error();
    this->pistonTimer.in.cancel();
    this->timeout.in.cancel();
    this->sensor.in.deactivate();
    state = ::BlockerController::State::Error;
    errorState = ::IBlocker::BlockerErrors::pistonExtended;
  }
  else if (state == ::BlockerController::State::Extended) 
  {
    pistonExtended = true;
    this->timeout.in.cancel();
  }
  else if (state == ::BlockerController::State::Error) ;
  else if ((!(state == ::BlockerController::State::Error) && (!(state == ::BlockerController::State::Extended) && !(state == ::BlockerController::State::Retracted)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void BlockerController::pistonTimer_timeout()
{
  if (state == ::BlockerController::State::Retracted) ;
  else if ((state == ::BlockerController::State::Extended && pistonExtended)) 
  {
    this->piston.in.retract();
    pistonExtended = !(pistonExtended);
    state = ::BlockerController::State::Retracted;
    this->sensor.in.activate();
  }
  else if ((state == ::BlockerController::State::Extended && !(pistonExtended))) 
  {
    this->controller.out.error();
    this->timeout.in.cancel();
    this->pistonTimer.in.cancel();
    this->sensor.in.deactivate();
    this->piston.in.retract();
    state = ::BlockerController::State::Error;
    errorState = ::IBlocker::BlockerErrors::pistonRetracted;
  }
  else if (state == ::BlockerController::State::Error) ;
  else if ((!(state == ::BlockerController::State::Error) && (!((state == ::BlockerController::State::Extended && !(pistonExtended))) && (!((state == ::BlockerController::State::Extended && pistonExtended)) && !(state == ::BlockerController::State::Retracted))))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void BlockerController::timeout_timeout()
{
  if (state == ::BlockerController::State::Retracted) ;
  else if (state == ::BlockerController::State::Extended) 
  {
    this->controller.out.error();
    this->pistonTimer.in.cancel();
    this->sensor.in.deactivate();
    this->piston.in.retract();
    state = ::BlockerController::State::Error;
    errorState = ::IBlocker::BlockerErrors::pistonRetracted;
  }
  else if (state == ::BlockerController::State::Error) ;
  else if ((!(state == ::BlockerController::State::Error) && (!(state == ::BlockerController::State::Extended) && !(state == ::BlockerController::State::Retracted)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void BlockerController::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void BlockerController::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}


Piston::Piston(const dzn::locator& dzn_locator)
: dzn_meta{"","Piston",0,0,{},{},{[this]{piston.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::IPiston::State::Retracted)

, piston({{"piston",this,&dzn_meta},{"",0,0}})



{
  dzn_rt.performs_flush(this) = true;

  piston.in.extend = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->piston) = false; return piston_extend();}, this->piston.meta, "extend");};
  piston.in.retract = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->piston) = false; return piston_retract();}, this->piston.meta, "retract");};





}

void Piston::piston_extend()
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
void Piston::piston_retract()
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


void Piston::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void Piston::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

Blocker::Blocker(const dzn::locator& dzn_locator)
: dzn_meta{"","Blocker",0,0,{},{& controller.dzn_meta,& piston.dzn_meta,& button.dzn_meta,& pistonTimer.dzn_meta,& timeoutTimer.dzn_meta},{[this]{blocker.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, controller(dzn_locator)
, piston(dzn_locator)
, button(dzn_locator)
, pistonTimer(dzn_locator)
, timeoutTimer(dzn_locator)

, blocker(controller.controller)

{


  controller.dzn_meta.parent = &dzn_meta;
  controller.dzn_meta.name = "controller";
  piston.dzn_meta.parent = &dzn_meta;
  piston.dzn_meta.name = "piston";
  button.dzn_meta.parent = &dzn_meta;
  button.dzn_meta.name = "button";
  pistonTimer.dzn_meta.parent = &dzn_meta;
  pistonTimer.dzn_meta.name = "pistonTimer";
  timeoutTimer.dzn_meta.parent = &dzn_meta;
  timeoutTimer.dzn_meta.name = "timeoutTimer";


  connect(piston.piston, controller.piston);
  connect(button.sensor, controller.sensor);
  connect(pistonTimer.iTimer, controller.pistonTimer);
  connect(timeoutTimer.iTimer, controller.timeout);

  dzn::rank(blocker.meta.provides.meta, 0);

}

void Blocker::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void Blocker::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1