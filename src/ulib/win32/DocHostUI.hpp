//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014 Michael Fink
//
/// \file DocHostUI.hpp doc host ui implementation for IE WebBrowser control embedding
//
#pragma once

// includes
#include <atliface.h>

/// default implementation for IDocHostUIHandlerDispatch interface
class IDocHostUIHandlerDispatchImpl: public IDocHostUIHandlerDispatch
{
public:
   /// called by the document host to display a shortcut menu
   virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(DWORD /*dwID*/, DWORD /*x*/, DWORD /*y*/, IUnknown* /*pcmdtReserved*/, IDispatch* /*pdispReserved*/, HRESULT* /*dwRetVal*/)
   {
      // let MSHTML display its context menu
      return S_FALSE;
   }

   /// \brief called to get the user interface capabilities of the host application
   /// \param pdwFlags one or more of the DOCHOSTUIFLAG flags
   /// \param pdwDoubleClick one or more of the DOCHOSTUIDBLCLK flags
   virtual HRESULT STDMETHODCALLTYPE GetHostInfo(DWORD* pdwFlags, DWORD* pdwDoubleClick)
   {
      pdwFlags; pdwDoubleClick;
      return E_NOTIMPL;
   }

   /// called to replace menus and toolbars
   virtual HRESULT STDMETHODCALLTYPE ShowUI(DWORD /*dwID*/, IUnknown* /*pActiveObject*/, IUnknown* /*pCommandTarget*/, IUnknown* /*pFrame*/, IUnknown* /*pDoc*/, HRESULT* /*dwRetVal*/)
   {
      return S_FALSE;
   }

   /// called to let the host remove its menus and toolbars
   virtual HRESULT STDMETHODCALLTYPE HideUI()
   {
      return E_NOTIMPL;
   }

   /// called to notify the host that the command state has changed
   virtual HRESULT STDMETHODCALLTYPE UpdateUI()
   {
      return E_NOTIMPL;
   }

   /// called when modeless UI is shown
   virtual HRESULT STDMETHODCALLTYPE EnableModeless(VARIANT_BOOL /*fEnable*/)
   {
      return E_NOTIMPL;
   }

   /// called when the document window has been activated or deactivated
   virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate(VARIANT_BOOL /*fActivate*/)
   {
      return E_NOTIMPL;
   }

   /// called when the frame window has been activated or deactivated 
   virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(VARIANT_BOOL /*fActivate*/)
   {
      return E_NOTIMPL;
   }

   /// called when the border has been resized
   virtual HRESULT STDMETHODCALLTYPE ResizeBorder(long /*left*/, long /*top*/, long /*right*/, long /*bottom*/, IUnknown* /*pUIWindow*/, VARIANT_BOOL /*fFrameWindow*/)
   {
      return S_OK;
   }

   /// called to translate accelerator keys when necessary
   virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(DWORD_PTR /*hWnd*/, DWORD /*nMessage*/, DWORD_PTR /*wParam*/, DWORD_PTR /*lParam*/, BSTR /*bstrGuidCmdGroup*/, DWORD /*nCmdID*/, HRESULT* /*dwRetVal*/)
   {
      return S_FALSE;
   }

   /// called to return a registry subkey path with options that override the IE settings
   virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath(BSTR* pbstrKey, DWORD /*dw*/)
   {
      *pbstrKey = NULL;
      return E_NOTIMPL;
   }

   /// called to supply an alternative IDropTarget interface
   virtual HRESULT STDMETHODCALLTYPE GetDropTarget(IUnknown* /*pDropTarget*/, IUnknown** ppDropTarget)
   {
      *ppDropTarget = NULL;
      return E_NOTIMPL;
   }

   /// called to retrieve the hosts IDispatch interface
   virtual HRESULT STDMETHODCALLTYPE GetExternal(IDispatch** ppDispatch)
   {
      *ppDispatch = NULL;
      return S_FALSE;
   }

   /// called to let the host modify the URL to be loaded
   virtual HRESULT STDMETHODCALLTYPE TranslateUrl(DWORD /*dwTranslate*/, BSTR /*bstrURLIn*/, BSTR* pbstrURLOut)
   {
      *pbstrURLOut = NULL;
      return S_FALSE;
   }

   /// called to let the host replace the mshtml data object
   virtual HRESULT STDMETHODCALLTYPE FilterDataObject(IUnknown* /*pDO*/, IUnknown** ppDORet)
   {
      *ppDORet = NULL;
      return S_FALSE;
   }

   // IDispatch methods

   /// returns type info count
   virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* /*ui*/)
   {
      return E_FAIL;
   }

   /// returns type info interface for given type info index
   virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT /*ui*/, LCID /*lcid*/, ITypeInfo** /*ppTypeInfo*/)
   {
      return E_FAIL;
   }

   /// returns IDs by given names
   virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(const IID&, LPOLESTR*, UINT, LCID, DISPID*)
   {
      return E_FAIL;
   }

   /// calls IDispatch methods
   virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID, const IID&, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*)
   {
      return E_FAIL;
   }

   /// ctor
   inline IDocHostUIHandlerDispatchImpl(): m_dwRef(0){}
   /// dtor
   inline virtual ~IDocHostUIHandlerDispatchImpl() { ATLASSERT(m_dwRef == 0); }

   // IUnknown methods

   /// adds reference
   virtual ULONG STDMETHODCALLTYPE AddRef()
   {
      return InterlockedIncrement(&m_dwRef);
   }

   /// releases reference
   virtual ULONG STDMETHODCALLTYPE Release()
   {
      unsigned long lResult = InterlockedDecrement(&m_dwRef);
      return lResult;
   }

   /// queries for interface
   virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject)
   {
      if (!ppvObject)
         return E_POINTER;

      if (riid == IID_IUnknown || riid == IID_IDocHostUIHandlerDispatch || riid == IID_IDispatch)
      {
         *ppvObject = static_cast<IDocHostUIHandlerDispatch*>(this);
         InterlockedIncrement(&m_dwRef);
         return S_OK;
      }

      // otherwise, incorrect IID, and thus error
      return E_NOINTERFACE;
   }

protected:
   /// reference count
   LONG m_dwRef;
};
