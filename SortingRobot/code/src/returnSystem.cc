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
#include "returnSystem.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



Returner::Returner(const dzn::locator& dzn_locator)
: dzn_meta{"","Returner",0,0,{& diskDetector.meta,& firstBelt.meta,& elevatedBelt.meta,& timer.meta},{},{[this]{returner.check_bindings();},[this]{diskDetector.check_bindings();},[this]{firstBelt.check_bindings();},[this]{elevatedBelt.check_bindings();},[this]{timer.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::IReturner::State::Stopped), errorState(::IReturner::ReturnerErrors::none), returningTime(30000), queue(0)

, returner({{"returner",this,&dzn_meta},{"",0,0}})

, diskDetector({{"",0,0},{"diskDetector",this,&dzn_meta}})
, firstBelt({{"",0,0},{"firstBelt",this,&dzn_meta}})
, elevatedBelt({{"",0,0},{"elevatedBelt",this,&dzn_meta}})
, timer({{"",0,0},{"timer",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  returner.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->returner) = false; return returner_start();}, this->returner.meta, "start");};
  returner.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->returner) = false; return returner_stop();}, this->returner.meta, "stop");};
  returner.in.addDisk = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->returner) = false; return returner_addDisk();}, this->returner.meta, "addDisk");};
  diskDetector.out.triggered = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->diskDetector) = false; return diskDetector_triggered();}, this->diskDetector.meta, "triggered");};
  firstBelt.out.error = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->firstBelt) = false; return firstBelt_error();}, this->firstBelt.meta, "error");};
  elevatedBelt.out.error = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->elevatedBelt) = false; return elevatedBelt_error();}, this->elevatedBelt.meta, "error");};
  timer.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->timer) = false; return timer_timeout();}, this->timer.meta, "timeout");};


  returner.in.getErrorState = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->returner) = false; return returner_getErrorState();}, this->returner.meta, "getErrorState");};



}

void Returner::returner_start()
{
  if (state == ::IReturner::State::Stopped) 
  {
    this->diskDetector.in.activate();
    this->firstBelt.in.start();
    this->elevatedBelt.in.start();
    this->timer.in.start(returningTime);
    state = ::IReturner::State::Running;
  }
  else if (state == ::IReturner::State::Running) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if (state == ::IReturner::State::Error) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if ((!(state == ::IReturner::State::Error) && (!(state == ::IReturner::State::Running) && !(state == ::IReturner::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Returner::returner_stop()
{
  if (state == ::IReturner::State::Stopped) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if (state == ::IReturner::State::Running) 
  {
    this->diskDetector.in.deactivate();
    this->firstBelt.in.stop();
    this->elevatedBelt.in.stop();
    this->timer.in.cancel();
    state = ::IReturner::State::Stopped;
  }
  else if (state == ::IReturner::State::Error) 
  {
    state = ::IReturner::State::Stopped;
    errorState = ::IReturner::ReturnerErrors::none;
  }
  else if ((!(state == ::IReturner::State::Error) && (!(state == ::IReturner::State::Running) && !(state == ::IReturner::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Returner::returner_addDisk()
{
  if (state == ::IReturner::State::Stopped) ;
  else if ((state == ::IReturner::State::Running && queue != 63
  )) 
  {
    queue = queue + 1;
  }
  else if ((state == ::IReturner::State::Running && queue == 63)) ;
  else if (state == ::IReturner::State::Error) ;
  else if ((!(state == ::IReturner::State::Error) && (!((state == ::IReturner::State::Running && queue == 63)) && (!((state == ::IReturner::State::Running && queue != 63
  )) && !(state == ::IReturner::State::Stopped))))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
::IReturner::ReturnerErrors::type Returner::returner_getErrorState()
{
  if (state == ::IReturner::State::Stopped) 
  {
    { this->reply_IReturner_ReturnerErrors = errorState; }
  }
  else if (state == ::IReturner::State::Running) 
  {
    { this->reply_IReturner_ReturnerErrors = errorState; }
  }
  else if (state == ::IReturner::State::Error) 
  {
    { this->reply_IReturner_ReturnerErrors = errorState; }
  }
  else if ((!(state == ::IReturner::State::Error) && (!(state == ::IReturner::State::Running) && !(state == ::IReturner::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return this->reply_IReturner_ReturnerErrors;
}
void Returner::diskDetector_triggered()
{
  if ((state == ::IReturner::State::Running && queue == 0)) 
  {
    this->diskDetector.in.activate();
    this->returner.out.warning();
    errorState = ::IReturner::ReturnerErrors::R200;
  }
  else if ((state == ::IReturner::State::Running && queue > 0)) 
  {
    queue = queue - 1;
    this->timer.in.cancel();
    this->timer.in.start(returningTime);
    this->diskDetector.in.activate();
  }
  else if ((!((state == ::IReturner::State::Running && queue > 0)) && !((state == ::IReturner::State::Running && queue == 0)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Returner::firstBelt_error()
{
  if (state == ::IReturner::State::Stopped) ;
  else if (state == ::IReturner::State::Running) 
  {
    this->returner.out.error();
    this->diskDetector.in.deactivate();
    this->firstBelt.in.stop();
    this->elevatedBelt.in.stop();
    this->timer.in.cancel();
    state = ::IReturner::State::Error;
    errorState = ::IReturner::ReturnerErrors::R100;
  }
  else if (state == ::IReturner::State::Error) ;
  else if ((!(state == ::IReturner::State::Error) && (!(state == ::IReturner::State::Running) && !(state == ::IReturner::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Returner::elevatedBelt_error()
{
  if (state == ::IReturner::State::Stopped) ;
  else if (state == ::IReturner::State::Running) 
  {
    this->returner.out.error();
    this->diskDetector.in.deactivate();
    this->firstBelt.in.stop();
    this->elevatedBelt.in.stop();
    this->timer.in.cancel();
    state = ::IReturner::State::Error;
    errorState = ::IReturner::ReturnerErrors::R101;
  }
  else if (state == ::IReturner::State::Error) ;
  else if ((!(state == ::IReturner::State::Error) && (!(state == ::IReturner::State::Running) && !(state == ::IReturner::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Returner::timer_timeout()
{
  if (state == ::IReturner::State::Running) 
  {
    this->returner.out.error();
    this->diskDetector.in.deactivate();
    this->firstBelt.in.stop();
    this->elevatedBelt.in.stop();
    state = ::IReturner::State::Error;
    errorState = ::IReturner::ReturnerErrors::R290;
  }
  else if (!(state == ::IReturner::State::Running)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void Returner::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void Returner::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

ReturningSystem::ReturningSystem(const dzn::locator& dzn_locator)
: dzn_meta{"","ReturningSystem",0,0,{},{& returner.dzn_meta,& sensor.dzn_meta,& firstBelt.dzn_meta,& elevatedBelt.dzn_meta,& timer.dzn_meta},{[this]{returningSystem.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, returner(dzn_locator)
, sensor(dzn_locator)
, firstBelt(dzn_locator)
, elevatedBelt(dzn_locator)
, timer(dzn_locator)

, returningSystem(returner.returner)

{


  returner.dzn_meta.parent = &dzn_meta;
  returner.dzn_meta.name = "returner";
  sensor.dzn_meta.parent = &dzn_meta;
  sensor.dzn_meta.name = "sensor";
  firstBelt.dzn_meta.parent = &dzn_meta;
  firstBelt.dzn_meta.name = "firstBelt";
  elevatedBelt.dzn_meta.parent = &dzn_meta;
  elevatedBelt.dzn_meta.name = "elevatedBelt";
  timer.dzn_meta.parent = &dzn_meta;
  timer.dzn_meta.name = "timer";


  connect(sensor.sensor, returner.diskDetector);
  connect(firstBelt.belt, returner.firstBelt);
  connect(elevatedBelt.belt, returner.elevatedBelt);
  connect(timer.iTimer, returner.timer);

  dzn::rank(returningSystem.meta.provides.meta, 0);

}

void ReturningSystem::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void ReturningSystem::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1