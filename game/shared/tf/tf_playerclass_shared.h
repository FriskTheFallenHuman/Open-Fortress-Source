//========= Copyright � 1996-2005, Valve LLC, All rights reserved. ============
//
// Purpose:
//
//=============================================================================
#ifndef TF_PLAYERCLASS_SHARED_H
#define TF_PLAYERCLASS_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_shareddefs.h"

#define TF_NAME_LENGTH		128

// Client specific.
#ifdef CLIENT_DLL

EXTERN_RECV_TABLE( DT_TFPlayerClassShared );

// Server specific.
#else

EXTERN_SEND_TABLE( DT_TFPlayerClassShared );

#endif


//-----------------------------------------------------------------------------
// Cache structure for the TF player class data (includes citizen). 
//-----------------------------------------------------------------------------

#define MAX_PLAYERCLASS_SOUND_LENGTH	128

struct TFPlayerClassData_t
{
	char		m_szClassName[TF_NAME_LENGTH];
	char		m_szModelName[TF_NAME_LENGTH];
	char		m_szArmModelName[TF_NAME_LENGTH];
	char		m_szHWMModelName[TF_NAME_LENGTH];
	char		m_szLocalizableName[TF_NAME_LENGTH];
	float		m_flMaxSpeed;
	int			m_nMaxHealth;
	int			m_nMaxArmor;
	int			m_aWeapons[TF_PLAYER_WEAPON_COUNT];
	int			m_aGrenades[TF_PLAYER_GRENADE_COUNT];
	int			m_aAmmoMax[TF_AMMO_COUNT];
	int			m_aBuildable[TF_PLAYER_BUILDABLE_COUNT];
	
	float		m_flTFCMaxSpeed;
	int			m_nTFCMaxHealth;
	int			m_nTFCMaxArmor;
	char		m_szTFCModelName[TF_NAME_LENGTH];
	char		m_szTFCArmModelName[TF_NAME_LENGTH];
	int			m_aTFCWeapons[TF_PLAYER_WEAPON_COUNT];

	bool		m_bDontDoAirwalk;
	bool		m_bDontDoNewJump;

	bool		m_bParsed;

#ifdef GAME_DLL
	// sounds
	char		m_szDeathSound[MAX_PLAYERCLASS_SOUND_LENGTH];
	char		m_szCritDeathSound[MAX_PLAYERCLASS_SOUND_LENGTH];
	char		m_szMeleeDeathSound[MAX_PLAYERCLASS_SOUND_LENGTH];
	char		m_szExplosionDeathSound[MAX_PLAYERCLASS_SOUND_LENGTH];
#endif

	TFPlayerClassData_t();
	const char *GetModelName() const;
	const char *GetArmModelName() const { return m_szArmModelName; }
	const char *GetTFCModelName() const { return m_szTFCModelName; }
	const char *GetTFCArmModelName() const  { return m_szTFCArmModelName; }
	void Parse( const char *pszClassName );

private:

	// Parser for the class data.
	void ParseData( KeyValues *pKeyValuesData );
};

void InitPlayerClasses( void );
TFPlayerClassData_t *GetPlayerClassData( int iClass );

//-----------------------------------------------------------------------------
// TF Player Class Shared
//-----------------------------------------------------------------------------
class CTFPlayerClassShared
{
public:

	CTFPlayerClassShared();

	DECLARE_EMBEDDED_NETWORKVAR()
	DECLARE_CLASS_NOBASE( CTFPlayerClassShared );

	bool		Init( int iClass );
	bool		IsClass( int iClass ) const						{ return ( m_iClass == iClass ); }
	int			GetClassIndex( void )							{ return m_iClass; }
	
	const char	*GetName( void ) const							{ return GetPlayerClassData( m_iClass )->m_szClassName; }
#ifndef CLIENT_DLL
	void		SetCustomModel( const char *pszModelName );
#endif	
	const char	*GetModelName( void ) const;
	const char	*GetTFCModelName( void ) const;
	const char 	*GetSetCustomModel ( void ) const;		
	bool 		UsesCustomModel ( void );		

	const char	*GetArmModelName( void ) const			{ return GetPlayerClassData( m_iClass )->GetArmModelName(); }		
	float		GetMaxSpeed( void )						{ return GetPlayerClassData( m_iClass )->m_flMaxSpeed; }
	int			GetMaxHealth( void )					{ return GetPlayerClassData( m_iClass )->m_nMaxHealth; }
	int			GetMaxArmor( void )						{ return GetPlayerClassData( m_iClass )->m_nMaxArmor; }
	
	const char	*GetTFCArmModelName( void ) const				{ return GetPlayerClassData( m_iClass )->GetTFCArmModelName(); }		
	float		GetTFCMaxSpeed( void )							{ return GetPlayerClassData( m_iClass )->m_flTFCMaxSpeed; }
	int			GetTFCMaxHealth( void )							{ return GetPlayerClassData( m_iClass )->m_nTFCMaxHealth; }
	int			GetTFCMaxArmor( void )							{ return GetPlayerClassData( m_iClass )->m_nTFCMaxArmor; }

	TFPlayerClassData_t  *GetData( void )						{ return GetPlayerClassData( m_iClass ); }

	// If needed, put this into playerclass scripts
	bool CanBuildObject( int iObjectType );

protected:

	CNetworkVar( int,	m_iClass );
#ifdef CLIENT_DLL
	char	m_iszSetCustomModel[MAX_PATH];
#else
	CNetworkVar(string_t,m_iszSetCustomModel);
#endif
};

#endif // TF_PLAYERCLASS_SHARED_H