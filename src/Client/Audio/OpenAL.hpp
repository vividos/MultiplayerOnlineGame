//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file OpenAL.hpp OpenAL wrapper classes
//
#pragma once

// includes
#include "Vector3.hpp"
#include <al.h>
#include <alc.h>

/// \brief OpenAL 1.1 wrapper classes
/// \details OpenAL is a cross-platform audio API. It allows us to position
/// audio sources in 3d around a listener. The audio sources can play from
/// contents of a buffer (which may be shared among sources). OpenAL may find
/// none, one or more devices allowing for playback.
namespace OpenAL
{
/// buffer smart pointer
typedef std::shared_ptr<class Buffer> BufferPtr;

/// source smart pointer
typedef std::shared_ptr<class Source> SourcePtr;

/// clears OpenAL errors
#define AL_CLEAR_ERROR() \
   alGetError();

/// checks OpenAL error value and throws exception on error
#define AL_CHECK_ERROR(FuncName) \
{ \
   ALenum enError = alGetError(); \
   if (enError != AL_NO_ERROR) \
   { \
      ATLTRACE(_T("%hs(%u): %s in %s"), __FILE__, __LINE__, Exception::ErrorText(enError), FuncName); \
      throw Exception(FuncName, enError, __FILE__, __LINE__); \
   } \
}

/// checks OpenAL error value and outputs a debug message on error
#define AL_TRACE_ERROR(FuncName) \
{ \
   ALenum enError = alGetError(); \
   if (enError != AL_NO_ERROR) \
      ATLTRACE(_T("%hs(%u): %s in %s"), __FILE__, __LINE__, Exception::ErrorText(enError), FuncName); \
}



/// OpenAL exception
class Exception: public ::Exception
{
public:
   /// base class type
   typedef ::Exception BaseClass;

   /// ctor
   Exception(LPCTSTR pszFunctionName, ALenum enError, LPCSTR pszSourceFile, UINT uiSourceLine) throw()
      :BaseClass(
         ErrorText(enError) + _T(", in ") + pszFunctionName,
         pszSourceFile, uiSourceLine),
       m_enError(enError)
   {
   }

   /// returns error
   ALenum Error() const throw() { return m_enError; }

   /// converts error number to text
   static CString ErrorText(ALenum enError) throw()
   {
      switch(enError)
      {
      case AL_NO_ERROR: return _T("OpenAL: no error");//  there is not currently an error 
      case AL_INVALID_NAME: return _T("OpenAL: invalid name");//  a bad name (ID) was passed to an OpenAL function 
      case AL_INVALID_ENUM: return _T("OpenAL: invalid enum");//  an invalid enum value was passed to an OpenAL function 
      case AL_INVALID_VALUE: return _T("OpenAL: invalid value");//  an invalid value was passed to an OpenAL function 
      case AL_INVALID_OPERATION: return _T("OpenAL: invalid operation");//  the requested operation is not valid 
      case AL_OUT_OF_MEMORY: return _T("OpenAL: out of memory");//  the requested operation resulted in OpenAL running out of memory
      default:
         return _T("OpenAL: unknown");
      }
   }

private:
   ALenum m_enError; ///< error number
};

/// \brief listener
/// \details can only be obtained via Device
class Listener
{
public:
   // set methods

   /// sets listener gain
   void Gain(float fValue)
   {
      AL_CLEAR_ERROR();

      alListenerf(AL_GAIN, fValue);
      AL_CHECK_ERROR(_T("alListener(AL_GAIN)"));
   }

   /// sets listener position
   void Position(const Vector3d& vPos)
   {
      AL_CLEAR_ERROR();

      alListener3f(AL_POSITION,
         static_cast<ALfloat>(vPos.X()),
         static_cast<ALfloat>(vPos.Y()),
         static_cast<ALfloat>(vPos.Z()));
      AL_CHECK_ERROR(_T("alListener(AL_POSITION)"));
   }

   /// sets listener velocity vector
   void Velocity(const Vector3d& vDir)
   {
      AL_CLEAR_ERROR();

      alListener3f(AL_VELOCITY,
         static_cast<ALfloat>(vDir.X()),
         static_cast<ALfloat>(vDir.Y()),
         static_cast<ALfloat>(vDir.Z()));
      AL_CHECK_ERROR(_T("alListener(AL_VELOCITY)"));
   }

   // get methods

   /// returns listener gain
   float Gain() const
   {
      AL_CLEAR_ERROR();

      float fValue = 0.0;
      alGetListenerf(AL_GAIN, &fValue);
      AL_CHECK_ERROR(_T("alGetListener(AL_GAIN)"));

      return fValue;
   }

private:
   friend class Device;

   /// ctor
   Listener() throw()
   {
   }
};


/// \brief buffer that holds sample data
/// \details can only be created using Device::CreateBuffer()
class Buffer
{
public:
   /// dtor
   ~Buffer() throw()
   {
      AL_CLEAR_ERROR();

      alDeleteBuffers(1, &m_uiBuffer);
      AL_TRACE_ERROR(_T("alDeleteBuffers")); // can't throw in dtor
   }

   // get methods

   /// returns number of channels
   unsigned int Channels() const
   {
      AL_CLEAR_ERROR();

      ALint iValue = 0;
      alGetBufferi(m_uiBuffer, AL_CHANNELS, &iValue);
      AL_CHECK_ERROR(_T("alGetBuffer(AL_CHANNELS)"));

      return iValue;
   }

   // set methods

   /// \brief sets stereo sample data
   /// \details some notes:
   /// * sets 16-bit signed samples
   /// * left and right channels interleaved
   /// * no 3D effects possible
   void SetStereo(const short* paSamplesInterleaved, size_t uiSizeInSamples, unsigned int uiSampleFreq)
   {
      ATLASSERT(m_uiBuffer != 0);

      AL_CLEAR_ERROR();

      alBufferData(m_uiBuffer, AL_FORMAT_STEREO16,
         reinterpret_cast<const ALvoid *>(paSamplesInterleaved),
         uiSizeInSamples * 2, // convert to number of bytes
         uiSampleFreq);
      AL_CHECK_ERROR(_T("alBufferData(AL_FORMAT_STEREO16)"));

      ATLASSERT(Channels() == 2);
   }

   /// \brief sets mono sample data
   /// \details some notes:
   /// * sets 16-bit signed samples
   /// * mono channel
   /// * 3D effect possible
   void SetMono(const short* paSamples, size_t uiSizeInSamples, unsigned int uiSampleFreq)
   {
      ATLASSERT(m_uiBuffer != 0);

      AL_CLEAR_ERROR();

      alBufferData(m_uiBuffer, AL_FORMAT_MONO16,
         reinterpret_cast<const ALvoid *>(paSamples),
         uiSizeInSamples * 2, // convert to number of bytes
         uiSampleFreq); 
      AL_CHECK_ERROR(_T("alBufferData(AL_FORMAT_MONO16)"));

      ATLASSERT(Channels() == 1);
   }

private:
   friend class Device;
   friend class Source;

   /// ctor
   Buffer()
      :m_uiBuffer(0)
   {
      AL_CLEAR_ERROR();

      alGenBuffers(1, &m_uiBuffer);
      AL_CHECK_ERROR(_T("alGenBuffers"));
   }

   /// returns buffer id
   unsigned int Id() const throw() { return m_uiBuffer; }

private:
   ALuint m_uiBuffer;   ///< buffer id
};

/// \brief source that may emit audio
/// \details a buffer can be attached to the source
class Source
{
public:
   /// dtor
   ~Source() throw()
   {
      AL_CLEAR_ERROR();
      alSourceStop(m_uiSource);
      AL_TRACE_ERROR(_T("alSourceStop")); // can't throw in dtor

      // detach buffer
      try
      {
         Attach(BufferPtr());
      }
      catch(...)
      {
      }

      // free stuff
      AL_CLEAR_ERROR();
      alDeleteSources(1, &m_uiSource);
      AL_TRACE_ERROR(_T("alDeleteSources")); // can't throw in dtor
   }

   /// attaches a buffer to given source
   void Attach(BufferPtr spBuffer)
   {
      AL_CLEAR_ERROR();

      alSourcei(m_uiSource, AL_BUFFER, spBuffer == NULL ? NULL : spBuffer->Id());
      AL_CHECK_ERROR(CString(_T("alSource(AL_BUFFER, )")) + (spBuffer == NULL ? _T("0)") : _T("id)")));

      m_spCurrentBuffer = spBuffer;
   }

   /// plays the attached buffer
   void Play()
   {
      AL_CLEAR_ERROR();

      alSourcePlay(m_uiSource);
      AL_CHECK_ERROR(_T("alSourcePlay"));
   }

   /// stops playing
   void Stop()
   {
      AL_CLEAR_ERROR();

      alSourceStop(m_uiSource);
      AL_CHECK_ERROR(_T("alSourceStop"));
   }

   // get methods

   /// returns if source is currently playing
   bool IsPlaying() const throw()
   {
      AL_CLEAR_ERROR();

      ALint iState = 0;
      alGetSourcei(m_uiSource, AL_SOURCE_STATE, &iState);
      AL_TRACE_ERROR(_T("alGetSourcei(AL_SOURCE_STATE)")); // can't throw in here

      return iState == AL_PLAYING;
   }

   /// returns source gain
   float Gain() const
   {
      AL_CLEAR_ERROR();

      float fValue = 1.0;

      alGetSourcef(m_uiSource, AL_GAIN, &fValue);
      AL_CHECK_ERROR(_T("alGetSource(AL_GAIN)"));

      return fValue;
   }

   // set methods

   /// sets source gain
   void Gain(float fValue)
   {
      AL_CLEAR_ERROR();

      alSourcef(m_uiSource, AL_GAIN, fValue);
      AL_CHECK_ERROR(_T("alSource(AL_GAIN)"));
   }

   /// sets source position
   void Position(const Vector3d& vPos)
   {
      AL_CLEAR_ERROR();

      alSource3f(m_uiSource, AL_POSITION,
         static_cast<ALfloat>(vPos.X()),
         static_cast<ALfloat>(vPos.Y()),
         static_cast<ALfloat>(vPos.Z()));
      AL_CHECK_ERROR(_T("alSource(AL_POSITION)"));
   }

   /// sets source velocity
   void Velocity(const Vector3d& vDir)
   {
      AL_CLEAR_ERROR();

      alSource3f(m_uiSource, AL_VELOCITY,
         static_cast<ALfloat>(vDir.X()),
         static_cast<ALfloat>(vDir.Y()),
         static_cast<ALfloat>(vDir.Z()));
      AL_CHECK_ERROR(_T("alSource(AL_VELOCITY)"));
   }

   /// sets if source should loop buffer
   void Looping(bool bLooping)
   {
      AL_CLEAR_ERROR();

      alSourcei(m_uiSource, AL_LOOPING, bLooping ? AL_TRUE : AL_FALSE );
      AL_CHECK_ERROR(_T("alSource(AL_LOOPING)"));
   }

private:
   friend class Device;

   /// ctor
   Source()
      :m_uiSource(0)
   {
      AL_CLEAR_ERROR();

      alGenSources(1, &m_uiSource);
      AL_CHECK_ERROR(_T("alGenSources"));
   }

private:
   ALuint m_uiSource;            ///< source id
   BufferPtr m_spCurrentBuffer;  ///< currently assigned buffer
};

/// audio device
class Device
{
public:
   /// distance type for distance model
   enum T_enDistanceType
   {
      distanceNone = AL_NONE,                   ///< no distance type
      distanceInverse = AL_INVERSE_DISTANCE,    ///< inverse distance
      distanceLinear = AL_LINEAR_DISTANCE,      ///< linear distance
      distanceExponent = AL_EXPONENT_DISTANCE,  ///< exponential distance
   };

   /// opens device; when the device name is empty, opens the default device
   Device(const CString& cszDeviceName = _T(""))
   {
      AL_CLEAR_ERROR();

      USES_CONVERSION;
      ALCdevice* pDevice = alcOpenDevice(cszDeviceName.IsEmpty() ? NULL : T2CA(cszDeviceName));
      if (pDevice == NULL)
         throw Exception(_T("alcOpenDevice"), alGetError(), __FILE__, __LINE__);

      m_spDevice.reset(pDevice, &::alcCloseDevice);

      AL_CLEAR_ERROR();

      ALCcontext* pContext = alcCreateContext(pDevice, NULL);
      if (pContext == NULL)
         throw Exception(_T("alcCreateContext"), alGetError(), __FILE__, __LINE__);

      m_spContext.reset(pContext, &::alcDestroyContext);

      ATLVERIFY(ALC_TRUE == alcMakeContextCurrent(pContext));

      AL_CLEAR_ERROR();

      // set up default orientation; "at" is negative z, "up" is y
      float afOrientation[6] = { 0.f, 0.f, -1.f, 0.f, 1.f, 0.f };
      alListenerfv(AL_ORIENTATION, afOrientation);
   }

   /// dtor
   ~Device() throw()
   {
      alcMakeContextCurrent(NULL);
   }

   /// returns default device name
   static CString DefaultDeviceName()
   {
      ATLASSERT(AL_TRUE == alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"));
      const ALCchar* paStr = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
      return CString(paStr);
   }

   /// enumerates devices; returns list of names
   static std::vector<CString> EnumDevices()
   {
      const ALCchar* paStr = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

      std::vector<CString> vecDeviceNames;
      while (paStr != NULL && *paStr != NULL)
      {
         vecDeviceNames.push_back(CString(paStr));
         paStr += strlen(paStr) + 1;
      }
      return vecDeviceNames;
   }

   // methods

   /// creates a new buffer
   BufferPtr CreateBuffer() const { return BufferPtr(new Buffer); }

   /// creates a new source
   SourcePtr CreateSource() const { return SourcePtr(new Source); }

   // set methods

   /// sets distance model
   void DistanceModel(T_enDistanceType enDistanceType, bool bClamped)
   {
      ALenum distanceModel = static_cast<ALenum>(enDistanceType);

      if (bClamped && distanceModel != AL_NONE)
         distanceModel++;

      AL_CLEAR_ERROR();
      alDistanceModel(distanceModel);
      AL_CHECK_ERROR(_T("alDistanceModel"));
   }

   // get methods

   /// returns the (only) listener
   Listener GetListener(){ return Listener(); }

   /// returns doppler factor
   float DopplerFactor() const throw()
   {
      return alGetFloat(AL_DOPPLER_FACTOR);
   }

   /// returns speed of sound, in m/s
   float SpeedOfSound() const throw()
   {
      return alGetFloat(AL_SPEED_OF_SOUND);
   }

   /// returns vendor string
   CString Vendor() const throw()
   {
      return CString(alGetString(AL_VENDOR));
   }

   /// returns version string
   CString Version() const throw()
   {
      return CString(alGetString(AL_VERSION));
   }

   /// returns renderer string
   CString Renderer() const throw()
   {
      return CString(alGetString(AL_RENDERER));
   }

private:
   std::shared_ptr<ALCdevice> m_spDevice;     ///< device
   std::shared_ptr<ALCcontext> m_spContext;   ///< device context
};

} // namespace OpenAL
