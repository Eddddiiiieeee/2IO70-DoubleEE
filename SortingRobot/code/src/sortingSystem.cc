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



Sorter::Sorter(const dzn::locator& dzn_locator)
: dzn_meta{"","Sorter",0,0,{& diskDetector.meta,& colorSensor.meta,& belt.meta,& blocker.meta,& detectorTimer.meta,& colorTimer.meta},{},{[this]{sorter.check_bindings();},[this]{diskDetector.check_bindings();},[this]{colorSensor.check_bindings();},[this]{belt.check_bindings();},[this]{blocker.check_bindings();},[this]{detectorTimer.check_bindings();},[this]{colorTimer.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, extendTime(5000), timeoutTime(2500), diskScanInterval(5000), errorState(::ISorter::SortingErrors::none), state(::ISorter::State::Stopped)

, sorter({{"sorter",this,&dzn_meta},{"",0,0}})

, diskDetector({{"",0,0},{"diskDetector",this,&dzn_meta}})
, colorSensor({{"",0,0},{"colorSensor",this,&dzn_meta}})
, belt({{"",0,0},{"belt",this,&dzn_meta}})
, blocker({{"",0,0},{"blocker",this,&dzn_meta}})
, detectorTimer({{"",0,0},{"detectorTimer",this,&dzn_meta}})
, colorTimer({{"",0,0},{"colorTimer",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  sorter.in.start = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sorter) = false; return sorter_start();}, this->sorter.meta, "start");};
  sorter.in.stop = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sorter) = false; return sorter_stop();}, this->sorter.meta, "stop");};
  diskDetector.out.triggered = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->diskDetector) = false; return diskDetector_triggered();}, this->diskDetector.meta, "triggered");};
  colorSensor.out.triggered = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->colorSensor) = false; return colorSensor_triggered();}, this->colorSensor.meta, "triggered");};
  belt.out.error = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->belt) = false; return belt_error();}, this->belt.meta, "error");};
  blocker.out.error = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->blocker) = false; return blocker_error();}, this->blocker.meta, "error");};
  detectorTimer.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->detectorTimer) = false; return detectorTimer_timeout();}, this->detectorTimer.meta, "timeout");};
  colorTimer.out.timeout = [&](){return dzn::call_out(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->colorTimer) = false; return colorTimer_timeout();}, this->colorTimer.meta, "timeout");};


  sorter.in.getErrorState = [&](){return dzn::call_in(this,[=]{ dzn_locator.get<dzn::runtime>().skip_block(&this->sorter) = false; return sorter_getErrorState();}, this->sorter.meta, "getErrorState");};



}

void Sorter::sorter_start()
{
  if (state == ::ISorter::State::Stopped) 
  {
    this->diskDetector.in.activate();
    this->belt.in.start();
    state = ::ISorter::State::Running;
  }
  else if (state == ::ISorter::State::Running) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if (state == ::ISorter::State::Error) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if ((!(state == ::ISorter::State::Error) && (!(state == ::ISorter::State::Running) && !(state == ::ISorter::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Sorter::sorter_stop()
{
  if (state == ::ISorter::State::Stopped) dzn_locator.get<dzn::illegal_handler>().illegal();
  else if (state == ::ISorter::State::Running) 
  {
    this->diskDetector.in.deactivate();
    this->colorSensor.in.deactivate();
    this->belt.in.stop();
    this->colorTimer.in.cancel();
    state = ::ISorter::State::Stopped;
  }
  else if (state == ::ISorter::State::Error) 
  {
    state = ::ISorter::State::Stopped;
    errorState = ::ISorter::SortingErrors::none;
  }
  else if ((!(state == ::ISorter::State::Error) && (!(state == ::ISorter::State::Running) && !(state == ::ISorter::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
::ISorter::SortingErrors::type Sorter::sorter_getErrorState()
{
  if (state == ::ISorter::State::Stopped) 
  {
    { this->reply_ISorter_SortingErrors = errorState; }
  }
  else if (state == ::ISorter::State::Running) 
  {
    { this->reply_ISorter_SortingErrors = errorState; }
  }
  else if (state == ::ISorter::State::Error) 
  {
    { this->reply_ISorter_SortingErrors = errorState; }
  }
  else if ((!(state == ::ISorter::State::Error) && (!(state == ::ISorter::State::Running) && !(state == ::ISorter::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return this->reply_ISorter_SortingErrors;
}
void Sorter::diskDetector_triggered()
{
  if (state == ::ISorter::State::Stopped) ;
  else if (state == ::ISorter::State::Running) 
  {
    this->colorTimer.in.start(diskScanInterval);
    this->colorSensor.in.activate();
  }
  else if (state == ::ISorter::State::Error) ;
  else if ((!(state == ::ISorter::State::Error) && (!(state == ::ISorter::State::Running) && !(state == ::ISorter::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Sorter::colorSensor_triggered()
{
  if (state == ::ISorter::State::Stopped) ;
  else if (state == ::ISorter::State::Running) 
  {
    this->blocker.in.trigger(extendTime,timeoutTime);
  }
  else if (state == ::ISorter::State::Error) ;
  else if ((!(state == ::ISorter::State::Error) && (!(state == ::ISorter::State::Running) && !(state == ::ISorter::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Sorter::belt_error()
{
  if (state == ::ISorter::State::Stopped) ;
  else if (state == ::ISorter::State::Running) 
  {
    this->diskDetector.in.deactivate();
    this->colorSensor.in.deactivate();
    this->belt.in.stop();
    this->blocker.in.stop();
    this->colorTimer.in.cancel();
    this->sorter.out.error();
    state = ::ISorter::State::Error;
    errorState = ::ISorter::SortingErrors::S100;
  }
  else if (state == ::ISorter::State::Error) ;
  else if ((!(state == ::ISorter::State::Error) && (!(state == ::ISorter::State::Running) && !(state == ::ISorter::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Sorter::blocker_error()
{
  if (state == ::ISorter::State::Stopped) ;
  else if (state == ::ISorter::State::Running) 
  {
    ::IBlocker::BlockerErrors::type pistonState = this->blocker.in.getErrorState();
    {
      if (pistonState == ::IBlocker::BlockerErrors::pistonRetracted) 
      {
        this->diskDetector.in.deactivate();
        this->colorSensor.in.deactivate();
        this->belt.in.stop();
        this->blocker.in.stop();
        this->colorTimer.in.cancel();
        this->sorter.out.error();
        state = ::ISorter::State::Error;
        errorState = ::ISorter::SortingErrors::S000;
      }
      else 
      {
        this->diskDetector.in.deactivate();
        this->colorSensor.in.deactivate();
        this->belt.in.stop();
        this->blocker.in.stop();
        this->colorTimer.in.cancel();
        this->sorter.out.error();
        state = ::ISorter::State::Error;
        errorState = ::ISorter::SortingErrors::S010;
      }
    }
  }
  else if (state == ::ISorter::State::Error) ;
  else if ((!(state == ::ISorter::State::Error) && (!(state == ::ISorter::State::Running) && !(state == ::ISorter::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Sorter::detectorTimer_timeout()
{
  if (state == ::ISorter::State::Stopped) ;
  else if (state == ::ISorter::State::Error) ;
  else if ((!(state == ::ISorter::State::Error) && !(state == ::ISorter::State::Stopped))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}
void Sorter::colorTimer_timeout()
{
  if (state == ::ISorter::State::Stopped) ;
  else if (state == ::ISorter::State::Running) 
  {
    this->colorSensor.in.deactivate();
    this->sorter.out.returnDisk();
  }
  else if (state == ::ISorter::State::Error) ;
  else if ((!(state == ::ISorter::State::Error) && (!(state == ::ISorter::State::Running) && !(state == ::ISorter::State::Stopped)))) dzn_locator.get<dzn::illegal_handler>().illegal();
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;

}


void Sorter::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void Sorter::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

//SYSTEM

SortingSystem::SortingSystem(const dzn::locator& dzn_locator)
: dzn_meta{"","SortingSystem",0,0,{},{& sorter.dzn_meta,& diskDetector.dzn_meta,& colorSensor.dzn_meta,& belt.dzn_meta,& blocker.dzn_meta,& detectorTimer.dzn_meta,& colorTimer.dzn_meta},{[this]{sortingSystem.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, sorter(dzn_locator)
, diskDetector(dzn_locator)
, colorSensor(dzn_locator)
, belt(dzn_locator)
, blocker(dzn_locator)
, detectorTimer(dzn_locator)
, colorTimer(dzn_locator)

, sortingSystem(sorter.sorter)

{


  sorter.dzn_meta.parent = &dzn_meta;
  sorter.dzn_meta.name = "sorter";
  diskDetector.dzn_meta.parent = &dzn_meta;
  diskDetector.dzn_meta.name = "diskDetector";
  colorSensor.dzn_meta.parent = &dzn_meta;
  colorSensor.dzn_meta.name = "colorSensor";
  belt.dzn_meta.parent = &dzn_meta;
  belt.dzn_meta.name = "belt";
  blocker.dzn_meta.parent = &dzn_meta;
  blocker.dzn_meta.name = "blocker";
  detectorTimer.dzn_meta.parent = &dzn_meta;
  detectorTimer.dzn_meta.name = "detectorTimer";
  colorTimer.dzn_meta.parent = &dzn_meta;
  colorTimer.dzn_meta.name = "colorTimer";


  connect(diskDetector.sensor, sorter.diskDetector);
  connect(colorSensor.colorSensor, sorter.colorSensor);
  connect(belt.belt, sorter.belt);
  connect(blocker.blocker, sorter.blocker);
  connect(detectorTimer.iTimer, sorter.detectorTimer);
  connect(colorTimer.iTimer, sorter.colorTimer);

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