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
#include "mainRobot.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



RaspberryPi::RaspberryPi(const dzn::locator& dzn_locator)
: dzn_meta{"","RaspberryPi",0,0,{& sorting.meta,& taker.meta,& returner.meta,& heartbeatClock.meta,& heartbeat1.meta,& heartbeat2.meta,& heartbeat3.meta,& heartbeat4.meta},{},{[this]{controller.check_bindings();},[this]{sorting.check_bindings();},[this]{taker.check_bindings();},[this]{returner.check_bindings();},[this]{heartbeatClock.check_bindings();},[this]{heartbeat1.check_bindings();},[this]{heartbeat2.check_bindings();},[this]{heartbeat3.check_bindings();},[this]{heartbeat4.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::RaspberryPi::State::Idle)

, controller({{"controller",this,&dzn_meta},{"",0,0}})

, sorting({{"",0,0},{"sorting",this,&dzn_meta}})
, taker({{"",0,0},{"taker",this,&dzn_meta}})
, returner({{"",0,0},{"returner",this,&dzn_meta}})
, heartbeatClock({{"",0,0},{"heartbeatClock",this,&dzn_meta}})
, heartbeat1({{"",0,0},{"heartbeat1",this,&dzn_meta}})
, heartbeat2({{"",0,0},{"heartbeat2",this,&dzn_meta}})
, heartbeat3({{"",0,0},{"heartbeat3",this,&dzn_meta}})
, heartbeat4({{"",0,0},{"heartbeat4",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  controller.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_start();}, this->controller.meta, "start");};
  controller.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_stop();}, this->controller.meta, "stop");};
  controller.in.emergencyStop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_emergencyStop();}, this->controller.meta, "emergencyStop");};
  controller.in.respondDiskCounters = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_respondDiskCounters();}, this->controller.meta, "respondDiskCounters");};
  controller.in.heartbeat1 = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_heartbeat1();}, this->controller.meta, "heartbeat1");};
  controller.in.heartbeat2 = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_heartbeat2();}, this->controller.meta, "heartbeat2");};
  controller.in.heartbeat3 = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_heartbeat3();}, this->controller.meta, "heartbeat3");};
  controller.in.heartbeat4 = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_heartbeat4();}, this->controller.meta, "heartbeat4");};
  controller.in.tookDisk_in = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_tookDisk_in();}, this->controller.meta, "tookDisk_in");};
  controller.in.error_in = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_error_in();}, this->controller.meta, "error_in");};
  controller.in.respondDisksTaken = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_respondDisksTaken();}, this->controller.meta, "respondDisksTaken");};
  controller.in.reboot = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_reboot();}, this->controller.meta, "reboot");};
  controller.in.resumeTaking = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_resumeTaking();}, this->controller.meta, "resumeTaking");};
  controller.in.pauseTaking = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->controller) = false; return controller_pauseTaking();}, this->controller.meta, "pauseTaking");};
  sorting.out.error = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sorting) = false; return sorting_error();}, this->sorting.meta, "error");};
  sorting.out.returnDisk = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sorting) = false; return sorting_returnDisk();}, this->sorting.meta, "returnDisk");};
  taker.out.tookDisk = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->taker) = false; return taker_tookDisk();}, this->taker.meta, "tookDisk");};
  taker.out.error = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->taker) = false; return taker_error();}, this->taker.meta, "error");};
  returner.out.error = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->returner) = false; return returner_error();}, this->returner.meta, "error");};
  returner.out.warning = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->returner) = false; return returner_warning();}, this->returner.meta, "warning");};
  heartbeatClock.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->heartbeatClock) = false; return heartbeatClock_timeout();}, this->heartbeatClock.meta, "timeout");};
  heartbeat1.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->heartbeat1) = false; return heartbeat1_timeout();}, this->heartbeat1.meta, "timeout");};
  heartbeat2.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->heartbeat2) = false; return heartbeat2_timeout();}, this->heartbeat2.meta, "timeout");};
  heartbeat3.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->heartbeat3) = false; return heartbeat3_timeout();}, this->heartbeat3.meta, "timeout");};
  heartbeat4.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->heartbeat4) = false; return heartbeat4_timeout();}, this->heartbeat4.meta, "timeout");};





}

void RaspberryPi::controller_start()
{
  if (state == ::RaspberryPi::State::Idle) 
  {
    state = ::RaspberryPi::State::Running;
    this->sorting.in.start();
    this->taker.in.start();
    this->returner.in.start();
    this->heartbeatClock.in.cancel();
    this->heartbeatClock.in.start(5000);
  }
  else if (state == ::RaspberryPi::State::Running) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if (state == ::RaspberryPi::State::Error) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::controller_stop()
{
  if (state == ::RaspberryPi::State::Idle) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if (state == ::RaspberryPi::State::Running) 
  {
    state = ::RaspberryPi::State::Idle;
    this->sorting.in.stop();
    this->taker.in.stop();
    this->returner.in.stop();
    this->heartbeatClock.in.cancel();
    this->heartbeat1.in.cancel();
    this->heartbeat2.in.cancel();
    this->heartbeat3.in.cancel();
    this->heartbeat4.in.cancel();
  }
  else if (state == ::RaspberryPi::State::Error) 
  {
    state = ::RaspberryPi::State::Idle;
    this->heartbeatClock.in.cancel();
  }
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::controller_emergencyStop()
{
  if (state == ::RaspberryPi::State::Idle) ;
  else if (state == ::RaspberryPi::State::Running) 
  {
    state = ::RaspberryPi::State::Idle;
    this->sorting.in.stop();
    this->taker.in.stop();
    this->returner.in.stop();
    this->heartbeatClock.in.cancel();
    this->heartbeat1.in.cancel();
    this->heartbeat2.in.cancel();
    this->heartbeat3.in.cancel();
    this->heartbeat4.in.cancel();
  }
  else if (state == ::RaspberryPi::State::Error) 
  {
    state = ::RaspberryPi::State::Idle;
    this->heartbeatClock.in.cancel();
  }
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::controller_respondDiskCounters()
{

  {
    ;
  }

  return;

}
void RaspberryPi::controller_heartbeat1()
{
  if (state == ::RaspberryPi::State::Idle) ;
  else if (state == ::RaspberryPi::State::Running) 
  {
    this->heartbeat1.in.cancel();
    this->heartbeat1.in.start(5000);
  }
  else if (state == ::RaspberryPi::State::Error) 
  {
    this->heartbeat1.in.cancel();
    this->heartbeat1.in.start(5000);
  }
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::controller_heartbeat2()
{
  if (state == ::RaspberryPi::State::Idle) ;
  else if (state == ::RaspberryPi::State::Running) 
  {
    this->heartbeat2.in.cancel();
    this->heartbeat2.in.start(5000);
  }
  else if (state == ::RaspberryPi::State::Error) 
  {
    this->heartbeat2.in.cancel();
    this->heartbeat2.in.start(5000);
  }
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::controller_heartbeat3()
{
  if (state == ::RaspberryPi::State::Idle) ;
  else if (state == ::RaspberryPi::State::Running) 
  {
    this->heartbeat3.in.cancel();
    this->heartbeat3.in.start(5000);
  }
  else if (state == ::RaspberryPi::State::Error) 
  {
    this->heartbeat3.in.cancel();
    this->heartbeat3.in.start(5000);
  }
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::controller_heartbeat4()
{
  if (state == ::RaspberryPi::State::Idle) ;
  else if (state == ::RaspberryPi::State::Running) 
  {
    this->heartbeat4.in.cancel();
    this->heartbeat4.in.start(5000);
  }
  else if (state == ::RaspberryPi::State::Error) 
  {
    this->heartbeat4.in.cancel();
    this->heartbeat4.in.start(5000);
  }
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::controller_tookDisk_in()
{

  {
    ;
  }

  return;

}
void RaspberryPi::controller_error_in()
{

  {
    ;
  }

  return;

}
void RaspberryPi::controller_respondDisksTaken()
{

  {
    ;
  }

  return;

}
void RaspberryPi::controller_reboot()
{
  if (state == ::RaspberryPi::State::Idle) ;
  else if (state == ::RaspberryPi::State::Running) ;
  else if (state == ::RaspberryPi::State::Error) 
  {
    state = ::RaspberryPi::State::Running;
    this->sorting.in.start();
    this->taker.in.start();
    this->returner.in.start();
    this->heartbeatClock.in.cancel();
    this->heartbeatClock.in.start(5000);
  }
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::controller_resumeTaking()
{
  if (state == ::RaspberryPi::State::Running) 
  {
    ::ITaking::TakingErrors::type takeState = this->taker.in.getErrorState();
    {
      if (takeState == ::ITaking::TakingErrors::none) 
      {
        this->taker.in.start();
      }
    }
  }
  else if (!(state == ::RaspberryPi::State::Running)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::controller_pauseTaking()
{
  if (state == ::RaspberryPi::State::Running) 
  {
    ::ITaking::TakingErrors::type takeState = this->taker.in.getErrorState();
    {
      if (takeState == ::ITaking::TakingErrors::none) 
      {
        this->taker.in.start();
      }
    }
  }
  else if (state == ::RaspberryPi::State::Error) ;
  else if ((!(state == ::RaspberryPi::State::Error) && !(state == ::RaspberryPi::State::Running))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::sorting_error()
{
  if (state == ::RaspberryPi::State::Running) 
  {
    ::ISorter::SortingErrors::type sortError = this->sorting.in.getErrorState();
    {
      if (sortError == ::ISorter::SortingErrors::S000) 
      {
        this->controller.out.S000();
        this->controller.out.error_out();
        state = ::RaspberryPi::State::Error;
        this->sorting.in.stop();
        this->taker.in.stop();
        this->returner.in.stop();
      }
      else {
        if (sortError == ::ISorter::SortingErrors::S010) 
        {
          this->controller.out.S010();
          this->controller.out.error_out();
          state = ::RaspberryPi::State::Error;
          this->sorting.in.stop();
          this->taker.in.stop();
          this->returner.in.stop();
        }
        else {
          if (sortError == ::ISorter::SortingErrors::S100) 
          {
            this->controller.out.S100();
            this->controller.out.error_out();
            state = ::RaspberryPi::State::Error;
            this->sorting.in.stop();
            this->taker.in.stop();
            this->returner.in.stop();
          }
          else ;
        }
      }
    }
  }
  else if (!(state == ::RaspberryPi::State::Running)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::sorting_returnDisk()
{
  if (state == ::RaspberryPi::State::Running) 
  {
    this->returner.in.addDisk();
  }
  else if (!(state == ::RaspberryPi::State::Running)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::taker_tookDisk()
{
  if (state == ::RaspberryPi::State::Running) 
  {
    this->controller.out.tookDisk_out();
  }
  else if (!(state == ::RaspberryPi::State::Running)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::taker_error()
{
  if (state == ::RaspberryPi::State::Running) 
  {
    ::ITaking::TakingErrors::type takeError = this->taker.in.getErrorState();
    {
      if (takeError == ::ITaking::TakingErrors::T010) 
      {
        this->controller.out.T010();
        state = ::RaspberryPi::State::Idle;
        this->controller.out.emergency();
        this->sorting.in.stop();
        this->taker.in.stop();
        this->returner.in.stop();
      }
      else {
        if (takeError == ::ITaking::TakingErrors::T000) 
        {
          this->controller.out.T000();
          state = ::RaspberryPi::State::Error;
          this->sorting.in.stop();
          this->taker.in.stop();
          this->returner.in.stop();
          this->controller.out.error_out();
        }
        else ;
      }
    }
  }
  else if (!(state == ::RaspberryPi::State::Running)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::returner_error()
{
  if (state == ::RaspberryPi::State::Running) 
  {
    ::IReturner::ReturnerErrors::type returnError = this->returner.in.getErrorState();
    {
      if (returnError == ::IReturner::ReturnerErrors::R100) 
      {
        this->controller.out.R100();
        this->controller.out.error_out();
        state = ::RaspberryPi::State::Error;
        this->sorting.in.stop();
        this->taker.in.stop();
        this->returner.in.stop();
      }
      else {
        if (returnError == ::IReturner::ReturnerErrors::R101) 
        {
          this->controller.out.R101();
          this->controller.out.error_out();
          state = ::RaspberryPi::State::Error;
          this->sorting.in.stop();
          this->taker.in.stop();
          this->returner.in.stop();
        }
        else {
          if (returnError == ::IReturner::ReturnerErrors::R290) 
          {
            this->controller.out.R290();
            this->controller.out.error_out();
            state = ::RaspberryPi::State::Error;
            this->sorting.in.stop();
            this->taker.in.stop();
            this->returner.in.stop();
          }
          else ;
        }
      }
    }
  }
  else if (!(state == ::RaspberryPi::State::Running)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::returner_warning()
{
  if (state == ::RaspberryPi::State::Running) 
  {
    this->controller.out.R200();
  }
  else if (!(state == ::RaspberryPi::State::Running)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::heartbeatClock_timeout()
{
  if (state == ::RaspberryPi::State::Idle) ;
  else if (state == ::RaspberryPi::State::Running) 
  {
    this->heartbeatClock.in.start(5000);
    this->controller.out.heartbeat();
  }
  else if (state == ::RaspberryPi::State::Error) ;
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::heartbeat1_timeout()
{
  if (state == ::RaspberryPi::State::Idle) ;
  else if (state == ::RaspberryPi::State::Running) ;
  else if (state == ::RaspberryPi::State::Error) ;
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::heartbeat2_timeout()
{
  if (state == ::RaspberryPi::State::Idle) ;
  else if (state == ::RaspberryPi::State::Running) ;
  else if (state == ::RaspberryPi::State::Error) ;
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::heartbeat3_timeout()
{
  if (state == ::RaspberryPi::State::Idle) ;
  else if (state == ::RaspberryPi::State::Running) ;
  else if (state == ::RaspberryPi::State::Error) ;
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void RaspberryPi::heartbeat4_timeout()
{
  if (state == ::RaspberryPi::State::Idle) ;
  else if (state == ::RaspberryPi::State::Running) ;
  else if (state == ::RaspberryPi::State::Error) ;
  else if ((!(state == ::RaspberryPi::State::Error) && (!(state == ::RaspberryPi::State::Running) && !(state == ::RaspberryPi::State::Idle)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void RaspberryPi::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void RaspberryPi::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

Robot::Robot(const dzn::locator& dzn_locator)
: dzn_meta{"","Robot",0,0,{},{& controller.dzn_meta,& taker.dzn_meta,& sorter.dzn_meta,& returner.dzn_meta,& heartbeatClock.dzn_meta,& heartbeat1.dzn_meta,& heartbeat2.dzn_meta,& heartbeat3.dzn_meta,& heartbeat4.dzn_meta},{[this]{robot.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, controller(dzn_locator)
, taker(dzn_locator)
, sorter(dzn_locator)
, returner(dzn_locator)
, heartbeatClock(dzn_locator)
, heartbeat1(dzn_locator)
, heartbeat2(dzn_locator)
, heartbeat3(dzn_locator)
, heartbeat4(dzn_locator)

, robot(controller.controller)

{


  controller.dzn_meta.parent = &dzn_meta;
  controller.dzn_meta.name = "controller";
  taker.dzn_meta.parent = &dzn_meta;
  taker.dzn_meta.name = "taker";
  sorter.dzn_meta.parent = &dzn_meta;
  sorter.dzn_meta.name = "sorter";
  returner.dzn_meta.parent = &dzn_meta;
  returner.dzn_meta.name = "returner";
  heartbeatClock.dzn_meta.parent = &dzn_meta;
  heartbeatClock.dzn_meta.name = "heartbeatClock";
  heartbeat1.dzn_meta.parent = &dzn_meta;
  heartbeat1.dzn_meta.name = "heartbeat1";
  heartbeat2.dzn_meta.parent = &dzn_meta;
  heartbeat2.dzn_meta.name = "heartbeat2";
  heartbeat3.dzn_meta.parent = &dzn_meta;
  heartbeat3.dzn_meta.name = "heartbeat3";
  heartbeat4.dzn_meta.parent = &dzn_meta;
  heartbeat4.dzn_meta.name = "heartbeat4";


  connect(taker.takingSystem, controller.taker);
  connect(sorter.sortingSystem, controller.sorting);
  connect(returner.returningSystem, controller.returner);
  connect(heartbeatClock.iTimer, controller.heartbeatClock);
  connect(heartbeat1.iTimer, controller.heartbeat1);
  connect(heartbeat2.iTimer, controller.heartbeat2);
  connect(heartbeat3.iTimer, controller.heartbeat3);
  connect(heartbeat4.iTimer, controller.heartbeat4);

  dzn::rank(robot.meta.provides.meta, 0);

}

void Robot::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void Robot::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1