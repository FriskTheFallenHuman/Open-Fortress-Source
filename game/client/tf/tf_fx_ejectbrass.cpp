//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Game-specific impact effect hooks
//
//=============================================================================//
#include "cbase.h"
#include "c_te_effect_dispatch.h"
#include "tempent.h"
#include "c_te_legacytempents.h"
#include "tf_shareddefs.h"
#include "tf_weapon_parse.h"

extern CTFWeaponInfo *GetTFWeaponInfo( int iWeapon );

//-----------------------------------------------------------------------------
// Purpose: TF Eject Brass
//-----------------------------------------------------------------------------
void TF_EjectBrassCallback( const CEffectData &data )
{
	CTFWeaponInfo *pWeaponInfo = GetTFWeaponInfo( data.m_nHitBox );
	if ( !pWeaponInfo )
		return;
	if ( !pWeaponInfo->m_szBrassModel || !pWeaponInfo->m_szBrassModel[0] )
		return;

	Vector vForward, vRight, vUp;
	AngleVectors( data.m_vAngles, &vForward, &vRight, &vUp );

	QAngle vecShellAngles;
	VectorAngles( -vUp, vecShellAngles );
	
	Vector vecVelocity = random->RandomFloat( 130, 180 ) * vForward +
						 random->RandomFloat( -30, 30 ) * vRight +
						 random->RandomFloat( -30, 30 ) * vUp;

	float flLifeTime = 10.0f;

	model_t *pModel = (model_t *)engine->LoadModel( pWeaponInfo->m_szBrassModel );
	if ( !pModel )
		return;
	
	int flags = FTENT_FADEOUT | FTENT_GRAVITY | FTENT_COLLIDEALL | FTENT_HITSOUND | FTENT_ROTATE;

	if ( data.m_nHitBox == TF_WEAPON_MINIGUN || data.m_nHitBox == TF_WEAPON_GATLINGGUN )
	{
		// More velocity for Jake
		vecVelocity = random->RandomFloat( 130, 250 ) * vForward +
			random->RandomFloat( -100, 100 ) * vRight +
			random->RandomFloat( -30, 80 ) * vUp;
	}

	Assert( pModel );	

	C_LocalTempEntity *pTemp = tempents->SpawnTempModel( pModel, data.m_vOrigin, vecShellAngles, vecVelocity, flLifeTime, FTENT_NEVERDIE );
	if ( pTemp == NULL )
		return;

	pTemp->m_vecTempEntAngVelocity[0] = random->RandomFloat(-512,511);
	pTemp->m_vecTempEntAngVelocity[1] = random->RandomFloat(-255,255);
	pTemp->m_vecTempEntAngVelocity[2] = random->RandomFloat(-255,255);
//	string_t shotgun = MAKE_STRING( "models/weapons/shells/shell_shotgun.mdl" );
	model_t *pShotgunShell = (model_t *)engine->LoadModel( "models/weapons/shells/shell_shotgun.mdl" );
	if ( pModel == pShotgunShell )
	{
		//STOP FUCKING DOING THIS SHIT JESUS CHRIST I WILL LITERALLY RIP YOUR SPINE OUT OF YOUR ASS IF I SEE SHIT LIKE THIS AGAIN
		//DevMsg("eh");
		pTemp->hitSound = BOUNCE_SHOTSHELL;
	}
	else
	{
		pTemp->hitSound = BOUNCE_SHELL;
	}
	pTemp->SetGravity( 0.4 );

	pTemp->m_flSpriteScale = 10;

	pTemp->flags = flags;

	// don't collide with owner
	pTemp->clientIndex = data.entindex();
	if ( pTemp->clientIndex < 0 )
	{
		pTemp->clientIndex = 0;
	}

	// ::ShouldCollide decides what this collides with
	pTemp->flags |= FTENT_COLLISIONGROUP;
	pTemp->SetCollisionGroup( COLLISION_GROUP_DEBRIS );
}

DECLARE_CLIENT_EFFECT( "TF_EjectBrass", TF_EjectBrassCallback );