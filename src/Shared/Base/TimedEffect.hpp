//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TimedEffect.hpp Timed effect class
//
#pragma once

// includes
#include <ulib/Timer.hpp>

/// \brief Timed effect class
/// \details This class can be used to manage timed effects. This is useful
/// while rendering: the class uses a timer to measure elapsed time from last
/// frame and calls the T::OnTick() function. This function can then perform
/// the timed effect. Some examples:
/// * moving a 3d model's bones
/// * moving a player or mobile on a predefined path
/// * accelerated movement, e.g. falling player
/// * moving the camera along a curve
/// The class is used as follows:
/// \code
/// class MyCameraMovement: public TimedEffect<MyCameraMovement, Timer> {
///    void OnTick(double dElapsed){ ... do something ... }
/// };
/// MyCameraMovement m_movement;
/// m_movement.Init();
/// m_movement.Tick();
/// \endcode
/// \tparam T class to derive from
/// \tparam TimerType timer type; use Timer or HighResolutionTimer, or
/// implement your own
template <typename T, typename TimerType = Timer>
class TimedEffect
{
public:
   /// starts timed effect
   void Init()
   {
      m_timer.Start();
   }

   /// call this on every occasion, e.g. when a new frame is rendered
   void Tick()
   {
      m_timer.Stop();
      double dElapsed = m_timer.TotalElapsed();

      T* pT = static_cast<T*>(this);
      pT->OnTick(dElapsed);

      m_timer.Restart();
   }

   /// called when a tick happened; implement this in your own derived class
   void OnTick(double dElapsed){ dElapsed; }

private:
   /// timer
   TimerType m_timer;
};
