//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file BillboardMatrix.cpp Billboard sprite matrix classes
//

// includes
#include "StdAfx.h"
#include "BillboardMatrix.hpp"
#include "OpenGL.hpp"

/// \see http://nehe.gamedev.net/article/billboarding_how_to/18011/

/// This only needs to be done once for all the particles, because the view
/// matrix does not change any between them.  If you were to modify the view
/// matrix at all, you would need to recalculate the camera pos and up vector.
void BillboardMatrixBase::GetCameraPosAndUp(Vector3d& vCameraPos, Vector3d& vCameraUp)
{
   Matrix4d mView;
   glGetDoublev(GL_MODELVIEW_MATRIX, mView.Data());

   // The values in the view matrix for the camera are the negative values of
   // the camera. This is because of the way gluLookAt works; I'm don't fully
   // understand why gluLookAt does what it does, but I know doing this works:)
   // I know that gluLookAt creates a the look vector as (eye - center), the
   // resulting direction vector is a vector from center to eye (the oposite
   // of what are view direction really is).
   vCameraPos = -mView.Column(3);
   vCameraUp  = mView.Row(1);

   // zero the translation in the matrix, so we can use the matrix to transform
   // camera postion to world coordinates using the view matrix
   mView.Column(3, Vector3d());

   // the view matrix is how to get to the gluLookAt pos from what we gave as
   // input for the camera position, so to go the other way we need to reverse
   // the rotation.  Transposing the matrix will do this.
   mView.TransposeRotation();

   // get the correct position of the camera in world space
   vCameraPos = mView * vCameraPos;
}
