#pragma once

#include "IPlayerViewModel.hpp"
#include "Vector3d.hpp"

namespace CharGenerator
{
   /// view model for character generator
   /// IPlayerViewModel implementation for non movable player
   class ViewModel : public IPlayerViewModel
   {
   public:
      /// ctor
      ViewModel() throw()
         :m_dMovementAngle(0.0)
      {
      }
      /// dtor
      virtual ~ViewModel() throw() {}

      /// returns movement angle
      double MovementAngle() const throw() { return m_dMovementAngle; }

   private:
      // virtual methods from IPlayerViewModel
      virtual const Vector3d& GetPlayerPosition() const override { return m_vPosition; }
      virtual double GetPlayerMovementAngle() const override { return m_dMovementAngle; }
      virtual bool IsPlayerViewAngleInControl() const override { return true; }
      virtual void SetPlayerTransparency(double /*dTransparency*/) override {}
      virtual void UpdatePlayerMovement(const MovementInfo& /*movementInfo*/) override {}
      virtual void UpdatePlayerPos() override {}
      virtual void UpdatePlayerViewAngle(double dViewAngle) override { m_dMovementAngle = dViewAngle; }

   private:
      /// position; always fixed
      Vector3d m_vPosition;

      /// movement/view angle
      double m_dMovementAngle;
   };

} // namespace CharGenerator
