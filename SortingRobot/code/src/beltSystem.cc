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
#include "beltSystem.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



BeltController::BeltController(const dzn::locator& dzn_locator)
: dzn_meta{"","BeltController",0,0,{& motor.meta,& sensor.meta,& timer.meta},{},{[this]{controller.check_bindings();},[this]{motor.check_bindings();},[this]{sensor.check_bindings();},[this]{timer.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, interval(10000), state(::BeltController::State::Idle)

, controller({{"controller",this,&dzn_meta},{"",0,0}})

, motor({{"",0,0},{"motor",this,&dzn_meta}})
, sensor({{"",0,0},{"sensor",this,&dzn_meta}})
, timer({{"",0,0},{"timer",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  controller.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_start();}, this->controller.meta, "start");};
  controller.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_stop();}, this->controller.meta, "stop");};
  sensor.out.triggered = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sensor) = false; return sensor_triggered();}, this->sensor.meta, "triggered");};
  timer.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->timer) = false; return timer_timeout();}, this->timer.meta, "timeout");};





}

void BeltController::controller_start()
{
  if (state == ::BeltController::State::Idle) 
  {
    this->motor.in.start();
    this->timer.in.start(interval);
    state = ::BeltController::State::Running;
  }
  else if (state == ::BeltController::State::Running) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if ((!(state == ::BeltController::State::Running) && !(state == ::BeltController::State::Idle))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void BeltController::controller_stop()
{
  if (state == ::BeltController::State::Idle) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if (state == ::BeltController::State::Running) 
  {
    this->motor.in.stop();
    this->timer.in.cancel();
    state = ::BeltController::State::Idle;
  }
  else if (state == ::BeltController::State::Error) 
  {
    state = ::BeltController::State::Idle;
  }
  else if ((!(state == ::BeltController::State::Error) && (!(state == ::BeltController::State::Running) && !(state == ::BeltController::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void BeltController::sensor_triggered()
{
  if (state == ::BeltController::State::Idle) ;
  else if (state == ::BeltController::State::Running) 
  {
    this->timer.in.cancel();
    this->timer.in.start(interval);
  }
  else if (state == ::BeltController::State::Error) ;
  else if ((!(state == ::BeltController::State::Error) && (!(state == ::BeltController::State::Running) && !(state == ::BeltController::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void BeltController::timer_timeout()
{
  if (state == ::BeltController::State::Idle) ;
  else if (state == ::BeltController::State::Running) 
  {
    state = ::BeltController::State::Error;
    this->controller.out.error();
    this->motor.in.stop();
  }
  else if (state == ::BeltController::State::Error) ;
  else if ((!(state == ::BeltController::State::Error) && (!(state == ::BeltController::State::Running) && !(state == ::BeltController::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void BeltController::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void BeltController::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}


Motor::Motor(const dzn::locator& dzn_locator)
: dzn_meta{"","Motor",0,0,{},{},{[this]{motor.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::Motor::State::Idle)

, motor({{"motor",this,&dzn_meta},{"",0,0}})



{
  dzn_rt.performs_flush(this) = true;

  motor.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->motor) = false; return motor_start();}, this->motor.meta, "start");};
  motor.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->motor) = false; return motor_stop();}, this->motor.meta, "stop");};





}

void Motor::motor_start()
{
  if (state == ::Motor::State::Idle) state = ::Motor::State::Running;
  else if (!(state == ::Motor::State::Idle)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Motor::motor_stop()
{
  if (state == ::Motor::State::Running) state = ::Motor::State::Idle;
  else if (!(state == ::Motor::State::Running)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void Motor::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void Motor::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

Belt::Belt(const dzn::locator& dzn_locator)
: dzn_meta{"","Belt",0,0,{},{& controller.dzn_meta,& motor.dzn_meta,& button.dzn_meta,& timer.dzn_meta},{[this]{belt.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, controller(dzn_locator)
, motor(dzn_locator)
, button(dzn_locator)
, timer(dzn_locator)

, belt(controller.controller)

{


  controller.dzn_meta.parent = &dzn_meta;
  controller.dzn_meta.name = "controller";
  motor.dzn_meta.parent = &dzn_meta;
  motor.dzn_meta.name = "motor";
  button.dzn_meta.parent = &dzn_meta;
  button.dzn_meta.name = "button";
  timer.dzn_meta.parent = &dzn_meta;
  timer.dzn_meta.name = "timer";


  connect(motor.motor, controller.motor);
  connect(button.sensor, controller.sensor);
  connect(timer.iTimer, controller.timer);

  dzn::rank(belt.meta.provides.meta, 0);

}

void Belt::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void Belt::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1