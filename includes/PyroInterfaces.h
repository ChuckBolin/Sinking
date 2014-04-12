/*
 * Copyright (c) 2002-2008 Fenomen Games.  All Rights Reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * Support: pyro-support@fenomen-games.com
 *
 */

/* $ Revision 1.3.10 $ */

#ifndef _PYRO_INTERFACES_H_INCLUDED
#define _PYRO_INTERFACES_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

#include <stdio.h>
#include "PyroTypes.h"
#include "PyroInterfaces.h"

namespace PyroParticles
{

#pragma pack(1)

struct CPyroVector3
{
	float x;
	float y;
	float z;
};

struct CPyroCreatedParticle
{
	CPyroVector3 Position;
	CPyroVector3 Direction;
	float Spin;
};

#pragma pack()

typedef void (*PyroParticleCreationCallback)(CPyroCreatedParticle *, void *pUserData);

class IPyroCollisionObject
{

public:

	virtual PyroBool CheckCollision(
		float Time, float Pos[3], float Vel[3], float Dir[3],
		float Delta, float ParticleSize, 
		float ParticleBounce, int nParticle, 
		float *pCt, PyroBool bUpdatePastCollide) = 0;

	virtual ~IPyroCollisionObject() {}

};

class IPyroParticleEmitter
{  

public:

	virtual void Reset() = 0;

	virtual int Render() = 0;

	virtual int Render(int MaxParticles, PyroBool bUniformDistribution) = 0;

	/* Creates new and destroy dead particles */
	virtual void Prepare(float Time, float TimeDelta) = 0;
	virtual void Prepare(float Time, float TimeDelta, float x, float y, float z, PyroBool bEmitting) = 0;

	/* Move particles */
	virtual void Move(float Time, float TimeDelta) = 0;

	/* Returns TRUE in case of 3D emitter, FALSE elsewhere */
	virtual PyroBool IsVolumeEmitter() const = 0;

	/* Returns name of the emitter */
	virtual const char *GetName() const = 0;

	/* Setups transformation matrix for the emitter */
	virtual void SetCameraMatrix(float m[3][4]) = 0;

	/* Setups transformation matrix for the emitter */
	virtual void SetLocalMatrix(const float m[3][4]) = 0;

	virtual void SetAirVector(float AirVectorX, float AirVectorY, float AirVectorZ) = 0;
	virtual void GetAirVector(float &AirVectorX, float &AirVectorY, float &AirVectorZ) = 0;

	virtual void SetTintColor(float r, float g, float b, float a) = 0;
	virtual void GetTintColor(float &r, float &g, float &b, float &a) = 0;

	virtual PyroBool HasParticles() const = 0;

	virtual int GetNumParticles() const = 0;

	virtual void AddCollisionObject(IPyroCollisionObject *pCollisionObject) = 0;

	virtual void SetOffset(float x, float y, float z) = 0;

	virtual void GetOffset(float &x, float &y, float &z) const = 0;

	virtual void SetScale(float Scale) = 0;

	virtual float GetScale() const = 0;
    
	virtual void SetParticleCreationCallback(void (*)(CPyroCreatedParticle *pParticle, void *pUserData), void *pUserData) = 0;

	virtual void SetUserData(void *pUserData) = 0;
	virtual void *GetUserData() = 0;

	virtual float GetContainerRadius() const = 0;

	virtual PyroBool HasInfiniteLife() const = 0;

	virtual ~IPyroParticleEmitter() {}

};

class IPyroFileIO
{

public:

	virtual long Read(void *pDest, long nBytes) = 0;
	virtual long Write(void *pSrc, long nBytes) = 0;
	virtual long Seek(long Position, int Where = SEEK_SET) = 0;
	virtual long Tell() = 0;

};

class IPyroFile
{

public:

	virtual void CreateTextures() = 0;
	virtual void DestroyTextures() = 0;

	virtual IPyroParticleEmitter *CreateEmitter(const char *pName) = 0;

	virtual int GetFileEmitters() const = 0;

	virtual IPyroParticleEmitter *FindEmitter(const char *pName) = 0;

	virtual const IPyroParticleEmitter *GetFileEmitter(int nEmitter) = 0;

	virtual void InvalidateTextures() = 0;

	virtual ~IPyroFile() {}

};

class IPyroParticleLibrary
{

public:

	virtual IPyroFile *LoadPyroFile(const char *pFileName) = 0;
	virtual IPyroFile *LoadPyroFile(IPyroFileIO *pFile) = 0;

	virtual void Init(PyroGraphics::IDevice *pGraphicsDevice) = 0;
	virtual void Done() = 0;

};

}


#endif /* _PYRO_INTERFACES_H_INCLUDED */
