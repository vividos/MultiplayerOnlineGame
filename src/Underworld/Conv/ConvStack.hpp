//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/Conv/ConvStack.hpp Conversation code stack
//
#pragma once

// includes
#include <vector>

namespace Conv
{

/// \brief Conversation code stack
/// Implements a stack that is used in the conversation code virtual machine.
/// The stack pointer points to the last pushed element. The usual methods for
/// pushing, popping, setting and getting data are available.
class ConvStack
{
public:
   /// ctor
   ConvStack()
      :m_uiStackPtr(0xffff)
   {
   }

   /// reserves stack space
   void Init(Uint16 uiStackSize)
   {
      m_vecStack.clear();
      m_uiStackPtr = 0xffff;
      m_vecStack.resize(uiStackSize, 0);
   }

   /// pushes a value onto the stack
   void Push(Uint16 val)
   {
      UaAssert(Uint16(m_uiStackPtr+1) < m_vecStack.size());
      m_vecStack[++m_uiStackPtr] = val;
   }

   /// pops a value from the stack
   Uint16 Pop()
   {
      UaAssert(m_uiStackPtr < m_vecStack.size());
      return m_vecStack[m_uiStackPtr--];
   }

   /// returns value at stack position
   Uint16 At(Uint16 pos) const
   {
      UaAssert(pos < m_vecStack.size());
      return m_vecStack[pos];
   }

   /// returns value at stack position
   void Set(Uint16 pos, Uint16 val)
   {
      UaAssert(pos < m_vecStack.size());
      m_vecStack[pos]=val;
   }

   /// returns current size of stack
   Uint16 Size() const
   {
      return static_cast<Uint16>(m_vecStack.size());
   }

   /// gets current stack pointer
   Uint16 StackPtr() const
   {
      return m_uiStackPtr;
   }

   /// sets new stack pointer
   void StackPtr(Uint16 val)
   {
      UaAssert(val <= m_vecStack.size());
      m_uiStackPtr = val;
   }

protected:
   /// stack pointer; always points to top element of stack
   Uint16 m_uiStackPtr;

   /// the stack itself
   std::vector<Uint16> m_vecStack;
};

} // namespace Conv
