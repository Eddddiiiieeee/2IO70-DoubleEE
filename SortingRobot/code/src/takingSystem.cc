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
#include "takingSystem.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



Taker::Taker(const dzn::locator& dzn_locator)
: dzn_meta{"","Taker",0,0,{& blocker.meta,& diskDetector.meta},{},{[this]{taker.check_bindings();},[this]{blocker.check_bindings();},[this]{diskDetector.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, extendTime(2500), timeoutTime(10000), state(::ITaking::State::Stopped), errorState(::ITaking::TakingErrors::none)

, taker({{"taker",this,&dzn_meta},{"",0,0}})

, blocker({{"",0,0},{"blocker",this,&dzn_meta}})
, diskDetector({{"",0,0},{"diskDetector",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  taker.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->taker) = false; return taker_start();}, this->taker.meta, "start");};
  taker.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->taker) = false; return taker_stop();}, this->taker.meta, "stop");};
  blocker.out.error = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->blocker) = false; return blocker_error();}, this->blocker.meta, "error");};
  diskDetector.out.triggered = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->diskDetector) = false; return diskDetector_triggered();}, this->diskDetector.meta, "triggered");};


  taker.in.getErrorState = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->taker) = false; return taker_getErrorState();}, this->taker.meta, "getErrorState");};



}

void Taker::taker_start()
{
  if (state == ::ITaking::State::Stopped) 
  {
    this->diskDetector.in.activate();
    state = ::ITaking::State::Running;
  }
  else if (state == ::ITaking::State::Running) ;
  else if (state == ::ITaking::State::Error) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if ((!(state == ::ITaking::State::Error) && (!(state == ::ITaking::State::Running) && !(state == ::ITaking::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Taker::taker_stop()
{
  if (state == ::ITaking::State::Stopped) ;
  else if (state == ::ITaking::State::Running) 
  {
    this->blocker.in.stop();
    this->diskDetector.in.deactivate();
    state = ::ITaking::State::Stopped;
  }
  else if (state == ::ITaking::State::Error) 
  {
    state = ::ITaking::State::Stopped;
    errorState = ::ITaking::TakingErrors::none;
  }
  else if ((!(state == ::ITaking::State::Error) && (!(state == ::ITaking::State::Running) && !(state == ::ITaking::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
::ITaking::TakingErrors::type Taker::taker_getErrorState()
{
  if (state == ::ITaking::State::Stopped) 
  {
    { this->reply_ITaking_TakingErrors = errorState; }
  }
  else if (state == ::ITaking::State::Running) 
  {
    { this->reply_ITaking_TakingErrors = errorState; }
  }
  else if (state == ::ITaking::State::Error) 
  {
    { this->reply_ITaking_TakingErrors = errorState; }
  }
  else if ((!(state == ::ITaking::State::Error) && (!(state == ::ITaking::State::Running) && !(state == ::ITaking::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return this->reply_ITaking_TakingErrors;
}
void Taker::blocker_error()
{
  if (state == ::ITaking::State::Stopped) ;
  else if (state == ::ITaking::State::Running) 
  {
    ::IBlocker::BlockerErrors::type pistonState = this->blocker.in.getErrorState();
    {
      if (pistonState == ::IBlocker::BlockerErrors::pistonRetracted) 
      {
        state = ::ITaking::State::Error;
        errorState = ::ITaking::TakingErrors::T000;
        this->taker.out.error();
        this->diskDetector.in.deactivate();
      }
      else {
        if (pistonState == ::IBlocker::BlockerErrors::pistonExtended) 
        {
          state = ::ITaking::State::Error;
          errorState = ::ITaking::TakingErrors::T010;
          this->taker.out.error();
          this->diskDetector.in.deactivate();
        }
        else ;
      }
    }
  }
  else if (state == ::ITaking::State::Error) ;
  else if ((!(state == ::ITaking::State::Error) && (!(state == ::ITaking::State::Running) && !(state == ::ITaking::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Taker::diskDetector_triggered()
{
  if (state == ::ITaking::State::Running) 
  {
    this->blocker.in.trigger(extendTime,timeoutTime);
    this->taker.out.tookDisk();
    this->diskDetector.in.activate();
  }
  else if (!(state == ::ITaking::State::Running)) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void Taker::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void Taker::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

TakingSystem::TakingSystem(const dzn::locator& dzn_locator)
: dzn_meta{"","TakingSystem",0,0,{},{& taker.dzn_meta,& detector.dzn_meta,& blocker.dzn_meta},{[this]{takingSystem.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, taker(dzn_locator)
, detector(dzn_locator)
, blocker(dzn_locator)

, takingSystem(taker.taker)

{


  taker.dzn_meta.parent = &dzn_meta;
  taker.dzn_meta.name = "taker";
  detector.dzn_meta.parent = &dzn_meta;
  detector.dzn_meta.name = "detector";
  blocker.dzn_meta.parent = &dzn_meta;
  blocker.dzn_meta.name = "blocker";


  connect(detector.sensor, taker.diskDetector);
  connect(blocker.blocker, taker.blocker);

  dzn::rank(takingSystem.meta.provides.meta, 0);

}

void TakingSystem::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void TakingSystem::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1