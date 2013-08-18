#ifndef __G_SV_SHARED_H__
#define __G_SV_SHARED_H__

#include "q_math.h"
#include "q_shared.h"
#include "entity.h"
#include "cvar.h"

void Init_CallVote(void);
DLL_PUBLIC __cdecl void Cmd_CallVote_f( gentity_t *ent );
void G_ChatRedirect(char* msg, int client, int mode);
void G_AddChatRedirect(void (*rd_dest)( char *, int, int));
qboolean Cmd_FollowClient_f(gentity_t *ent, int clientnum);
DLL_PUBLIC __cdecl void StopFollowingOnDeath( gentity_t *ent );
__cdecl void G_Say( gentity_t *ent, gentity_t *target, int mode, const char *chatText );
DLL_PUBLIC __cdecl void ExitLevel( void );
DLL_PUBLIC void G_RegisterCvarsCallback( );
DLL_PUBLIC void QDECL G_LogPrintf( const char *fmt, ... );
void __cdecl StopFollowing( gentity_t* ent );
void __regparm3 G_SayTo(gentity_t *ent, gentity_t *other, int mode, int color, const char* teamname, const char* name, const char *message);

//This defines Cvars directly related to executable file
#define getcvaradr(adr) ((cvar_t*)(*(int*)(adr)))

#define g_gametype getcvaradr(0x13ed89bc)
#define g_gravity getcvaradr(0x84bcff4)
#define g_TeamName_Allies getcvaradr(0x84bd090)
#define g_TeamName_Axis getcvaradr(0x84bd094)
#define g_maxclients getcvaradr(0x84bcfe8)

extern cvar_t* g_speed;

void __cdecl SV_GameSendServerCommand(int clientnum, int svscmd_type, const char *text);
//tSV_GameSendServerCommand SV_GameSendServerCommand = (tSV_GameSendServerCommand)(0x817ce42);
void G_ShowMotd(int clnum);
DLL_PUBLIC void QDECL G_LogPrintf( const char *fmt, ... );
void G_PrintRedirect(char* msg, int len);
void G_PrintAddRedirect(void (*rd_dest)( char *, int));
DLL_PUBLIC __cdecl void ClientSpawn(gentity_t* ent, vec3_t* px, vec3_t* py);
DLL_PUBLIC __cdecl void ClientUserinfoChanged( int clientNum );


void Pmove_ExtendedResetState( void );
void Pmove_ExtendedTurnOn( void );
DLL_PUBLIC __cdecl __optimize3 int Pmove_GetSpeed( playerState_t *ps );
DLL_PUBLIC __cdecl __optimize3 int Pmove_GetGravity( playerState_t *ps );
DLL_PUBLIC __cdecl __optimize3 float Jump_GetHeight( playerState_t *ps);
DLL_PUBLIC __cdecl __optimize3 void Jump_ClampVelocity(playerState_t* ps, vec3_t vec);
DLL_PUBLIC __cdecl __optimize3 qboolean Jump_IsPlayerAboveMax(playerState_t* ps);
DLL_PUBLIC __cdecl __optimize3 qboolean Jump_GetStepHeight(playerState_t* ps, const vec3_t vec1, float* val2);
DLL_PUBLIC __cdecl __optimize3 float Jump_CalcHeight( playerState_t* ps );
DLL_PUBLIC __cdecl __optimize3 void StuckInClient( gentity_t* gen );

void __cdecl G_RunFrame( int time );
int __cdecl G_LocalizedStringIndex( const char* );

void __cdecl ClientCommand( int );
const char* __cdecl ClientConnect(int clnum, short clscriptid);  //Something simular to VM_Call
void __cdecl ClientBegin( int clientNum );

#endif