#pragma once

#include "../main.hpp"
#include "color.hpp"
#include "angle.hpp"

typedef float matrix3x4[ 3 ][ 4 ];

namespace netvars {
	extern uintptr_t m_iTeamNum, m_iHealth, m_lifeState;
}

#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2

#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4)	// Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6)	// Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7)	// Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

class c_csplayer {
public:
	int health() {
		return *foi::make_ptr<int*>( this, netvars::m_iHealth );
	}

	angle& get_absangles() {
		return foi::getvfunc< angle&( __thiscall*) (void *) >( this, 11 )( this );
	}

	Vector bbmins() {
		return *foi::make_ptr<Vector*>( this, 0x320 );
	}

	Vector bbmaxs() {
		return *foi::make_ptr<Vector*>( this, 0x32C );
	}

	Vector origin() {
		return *foi::make_ptr<Vector*>( this, 0x134 );
	}

	int teamnum() {
		return *foi::make_ptr<int*>( this, netvars::m_iTeamNum );
	}

	bool is_dormant() {
		return ( *foi::make_ptr<bool*>( this, 0xE9 ) );
	}

	bool is_alive() {
		return ( *foi::make_ptr<int*>( this, 0x25B ) == LIFE_ALIVE );
	}

	int flags() {
		return *foi::make_ptr<int*>( this, 0x100 );
	}
};

// neither this.
struct player_info_t {
	char __pad0[ 0x8 ];

	int xuidlow;
	int xuidhigh;

	char name[ 128 ];
	int userid;			// local server user ID, unique while server is running
	char guid[ 33 ];	// global unique player identifer

	char __pad1[ 0x17B ];
};

class local_player {
	float _frametime;

public:

	void set_frametime( float frametime ) {
		_frametime = frametime;
	}

	float get_frametime() {
		return _frametime;
	}
};

struct recvtable;

struct recvprop {
	char					*varname;
	SendPropType			recvtype;
	int						flags;
	int						stringbuffersize;
	bool					is_inarray;
	const void				*extradata;
	recvprop				*arrayprop;
	void					*arraylengthproxy;
	void					*proxyfn;
	void					*datatableproxy;
	recvtable				*datatable;
	int						offset;
	int						elementstride;
	int						elements;
	const char				*parentarrayparentname;
};

struct recvtable {
	recvprop		*props;
	int				nprops;
	void			*decoder;
	char			*nettablename;
	bool			is_initialized;
	bool			is_inmainlist;
};

// linked list of classes
struct clientclass {
	void*			createfn;
	void*			createeventfn;	// Only called for event objects.
	const char		*networkname;	// networked name
	recvtable		*recvtable;		// this class's table of properties.
	clientclass		*next;			// next clientclass
	int				classid;		// managed by the engine.
};

class ibaseclient {
public:
	// Connect appsystem components, get global interfaces, don't run any other init code
	virtual int				Connect( void * (*appSystemFactory), void *pGlobals ) = 0;
	virtual int				Disconnect( void ) = 0;
	// run other init code here
	virtual int				Init( void * ( *appSystemFactory ), void *pGlobals ) = 0;
	virtual void			PostInit() = 0;
	// Called once when the client DLL is being unloaded
	virtual void			Shutdown( void ) = 0;
	// Called at the start of each level change
	virtual void			LevelInitPreEntity( char const* pMapName ) = 0;
	// Called at the start of a new level, after the entities have been received and created
	virtual void			LevelInitPostEntity() = 0;
	// Called at the end of a level
	virtual void			LevelShutdown( void ) = 0;
	// Request a pointer to the list of client datatable classes
	virtual clientclass		*get_allclasses( void ) = 0;
	// Called once per level to re-initialize any hud element drawing stuff
	virtual int				HudVidInit( void ) = 0;
	// Called by the engine when gathering user input
	virtual void			HudProcessInput( bool bActive ) = 0;
	// Called oncer per frame to allow the hud elements to think
	virtual void			HudUpdate( bool bActive ) = 0;
	// Reset the hud elements to their initial states
	virtual void			HudReset( void ) = 0;
	// Display a hud text message
	virtual void			HudText( const char * message ) = 0;
	virtual void			ShouldDrawDropdownConsole( void ) = 0;
	// Mouse Input Interfaces
	// Activate the mouse (hides the cursor and locks it to the center of the screen)
	virtual void			IN_ActivateMouse( void ) = 0;
	// Deactivates the mouse (shows the cursor and unlocks it)
	virtual void			IN_DeactivateMouse( void ) = 0;
	// This is only called during extra sound updates and just accumulates mouse x, y offets and recenters the mouse.
	//  This call is used to try to prevent the mouse from appearing out of the side of a windowed version of the engine if
	//  rendering or other processing is taking too long
	virtual void			IN_Accumulate( void ) = 0;
	// Reset all key and mouse states to their initial, unpressed state
	virtual void			IN_ClearStates( void ) = 0;
	// If key is found by name, returns whether it's being held down in isdown, otherwise function returns false
	virtual bool			IN_IsKeyDown( const char *name, bool& isdown ) = 0;
	// Raw keyboard signal, if the client .dll returns 1, the engine processes the key as usual, otherwise,
	//  if the client .dll returns 0, the key is swallowed.
	virtual int				IN_KeyEvent( int eventcode, int /*ButtonCode_t*/ keynum, const char *pszCurrentBinding ) = 0;
	// This function is called once per tick to create the player CUserCmd (used for prediction/physics simulation of the player)
	// Because the mouse can be sampled at greater than the tick interval, there is a separate input_sample_frametime, which
	//  specifies how much additional mouse / keyboard simulation to perform.
	virtual void			CreateMove(
		int sequence_number,			// sequence_number of this cmd
		float input_sample_frametime,	// Frametime for mouse input sampling
		bool active ) = 0;				// True if the player is active (not paused)
};

class CEntityList {
public:
	c_csplayer* get_cliententity( int entnum ) {
		return foi::getvfunc<c_csplayer *( __thiscall *)( void *, int )>( this, 3 )( this, entnum );
	}
	c_csplayer* get_cliententity_from_handle( unsigned long hEnt ) {
		return foi::getvfunc<c_csplayer *( __thiscall * ) ( void *, unsigned long )>( this, 4 )( this, hEnt );
	}
};

// JUST SO YOU KNOW THIS WON'T BE DETECTED -.-
class vengineclient {
public:
	void get_screensize( int& width, int& height ) {
		return foi::getvfunc< void( __thiscall *)( void *, int&, int& ) >( this, 5 )( this, width, height );
	}

	bool get_playerinfo( int ent_num, player_info_t *pinfo ) {
		return foi::getvfunc< bool( __thiscall *)( void *, int, player_info_t* ) >( this, 8 )( this, ent_num, pinfo );
	}

	int get_localplayer() {
		return foi::getvfunc< int( __thiscall * )( void * ) >( this, 12 )( this );
	}

	void get_viewangles( angle& vAngles ) {
		return foi::getvfunc< void( __thiscall * )( void *, angle& ) >( this, 18 )( this, vAngles );
	}

	void set_viewangles( angle& vAngles ) {
		return foi::getvfunc< void( __thiscall * )( void *, angle& ) >( this, 19 )( this, vAngles );
	}

	int get_maxclients() {
		return foi::getvfunc< bool( __thiscall * )( void * ) >( this, 20 )( this );
	}

	bool in_game() {
		return foi::getvfunc< bool( __thiscall * )( void * ) >( this, 26 )( this );
	}

	bool connected() {
		return foi::getvfunc< bool( __thiscall * )( void * ) >( this, 27 )( this );
	}

	const matrix3x4 &worldtoscreen() {
		return foi::getvfunc< const matrix3x4 &( __thiscall * )( void * ) >( this, 37 )( this );
	}

	void clientcmd_unrestricted( char  const* cmd ) {
		return foi::getvfunc<void( __thiscall * )( void *, const char* )>( this, 114 )( this, cmd );
	}

	bool is_paused() {
		return foi::getvfunc<bool( __thiscall * )( void * ) >( this, 197 )( this );
	}
};

class IVEngineServer {
public:
	int get_entitycount() { // returns all entities in map
		return foi::getvfunc<int( __thiscall* )( void * )>( this, 20 )( this );
	}
};

class IGameEvent
{
public:
	virtual						~IGameEvent() {};
	virtual const char			*GetName() const = 0;						// get event name

	virtual bool				IsReliable() const = 0;						// if event handled reliable
	virtual bool				IsLocal() const = 0;						// if event is never networked
	virtual bool				IsEmpty( const char *keyName = NULL ) = 0;	// check if data field exists

							// Data access
	virtual bool				GetBool( const char *keyName = NULL, bool defaultValue = false ) = 0;
	virtual int					GetInt( const char *keyName = NULL, int defaultValue = 0 ) = 0;
	virtual unsigned long		GetUint64( const char *keyName = NULL, unsigned long defaultValue = 0 ) = 0;
	virtual float				GetFloat( const char *keyName = NULL, float defaultValue = 0.0f ) = 0;
	virtual const char			*GetString( const char *keyName = NULL, const char *defaultValue = "" ) = 0;
	virtual const wchar_t		*GetWString( char const *keyName = NULL, const wchar_t *defaultValue = L"" ) = 0;

	virtual void				SetBool( const char *keyName, bool value ) = 0;
	virtual void				SetInt( const char *keyName, int value ) = 0;
	virtual void				SetUint64( const char *keyName, unsigned long value ) = 0;
	virtual void				SetFloat( const char *keyName, float value ) = 0;
	virtual void				SetString( const char *keyName, const char *value ) = 0;
	virtual void				SetWString( const char *keyName, const wchar_t *value ) = 0;
};

class IGameEventListener2
{
public:
	virtual	~IGameEventListener2( void ) {};

	// FireEvent is called by EventManager if event just occured
	// KeyValue memory will be freed by manager if not needed anymore
	virtual void FireGameEvent( IGameEvent *event ) = 0;

	virtual int	 GetEventDebugID( void ) = 0;
};

class IGameEventManager2 {
	virtual	~IGameEventManager2( void ) {};

	// load game event descriptions from a file eg "resource\gameevents.res"
	virtual int LoadEventsFromFile( const char *filename ) = 0;

	// removes all and anything
	virtual void  Reset() = 0;

	// adds a listener for a particular event
	virtual bool AddListener( IGameEventListener2 *listener, const char *name, bool bServerSide ) = 0;

	// returns true if this listener is listens to given event
	virtual bool FindListener( IGameEventListener2 *listener, const char *name ) = 0;

	// removes a listener 
	virtual void RemoveListener( IGameEventListener2 *listener ) = 0;

	// create an event by name, but doesn't fire it. returns NULL is event is not
	// known or no listener is registered for it. bForce forces the creation even if no listener is active
	virtual IGameEvent *CreateEvent( const char *name, bool bForce = false, int *pCookie = NULL ) = 0;

	// fires a server event created earlier, if bDontBroadcast is set, event is not send to clients
	virtual bool FireEvent( IGameEvent *event, bool bDontBroadcast = false ) = 0;

	// fires an event for the local client only, should be used only by client code
	virtual bool FireEventClientSide( IGameEvent *event ) = 0;

	// create a new copy of this event, must be free later
	virtual IGameEvent *DuplicateEvent( IGameEvent *event ) = 0;

	// if an event was created but not fired for some reason, it has to bee freed, same UnserializeEvent
	virtual void FreeEvent( IGameEvent *event ) = 0;

	// write/read event to/from bitbuffer
	virtual bool SerializeEvent( IGameEvent *event, void *buf ) = 0;
	virtual IGameEvent *UnserializeEvent( void *buf ) = 0; // create new KeyValues, must be deleted
};

namespace vgui {
	typedef unsigned long hfont;
	typedef unsigned int vpanel;

	struct Vertex_t {
		Vector2D	m_Position;
		Vector2D	m_TexCoord;

		Vertex_t() {}
		Vertex_t( const Vector2D &pos, const Vector2D &coord = Vector2D( 0, 0 ) ) {
			m_Position = pos;
			m_TexCoord = coord;
		}
		void Init( const Vector2D &pos, const Vector2D &coord = Vector2D( 0, 0 ) ) {
			m_Position = pos;
			m_TexCoord = coord;
		}
	};

	class ipanel {
	public:
		const char *GetName( vpanel vguiPanel ) {
			return foi::getvfunc<const char* ( __thiscall * )( void *, vpanel )>( this, 36 )( this, vguiPanel );
		}
	};

	class isurface {
	public:
		void setcolor( color col ) {
			return foi::getvfunc< void( __thiscall * )( void *, int, int, int, int ) >( this, 15 )( this, col.getred(), col.getgreen(), col.getblue(), col.getalpha() );
		}

		void filledrect( Vector2D x, Vector2D y ) {
			foi::getvfunc< void( __thiscall * )( void *, int, int, int, int ) >( this, 16 )( this, x.x, x.y, y.x, y.y );
		}

		void outlinedrect( Vector2D x, Vector2D y ) {
			foi::getvfunc< void( __thiscall * )( void *, int, int, int, int ) >( this, 18 )( this, x.x, x.y, y.x, y.y );
		}

		void line( Vector2D x, Vector2D y ) { // don't mind me doing this, i don't think it has anything to say, it's just preference.
			foi::getvfunc< void( __thiscall * )( void *, int, int, int, int ) >( this, 19 )( this, x.x, x.y, y.x, y.y );
		}

		void setfont( hfont font ) {
			foi::getvfunc< void( __thiscall * )( void *, hfont ) >( this, 23 )( this, font );
		}

		void setfontcolor( color col ) {
			return foi::getvfunc< void( __thiscall * )( void *, color ) >( this, 24 )( this, col );
		}

		void settextpos( int x, int y ) {
			foi::getvfunc< void( __thiscall *)( void *, int, int ) >( this, 26 )( this, x, y );
		}

		void printtext( const wchar_t* text, int textLen ) {
			typedef void( __thiscall* Fn )( void *, const wchar_t*, int, int );
			foi::getvfunc< Fn >( this, 28 )( this, text, textLen, 0 );
		}

		void settextureid( int textureID ) {
			typedef void( __thiscall* Fn )( void *, int );
			return foi::getvfunc< Fn >( this, 38 )( this, textureID );
		}

		void settexturergba( int textureID, unsigned char  const* colors, int w, int h ) {
			typedef void( __thiscall* Fn )( void *, int, unsigned char  const*, int, int );
			return foi::getvfunc< Fn >( this, 37 )( this, textureID, colors, w, h );
		}

		int createtexture( bool procedural ) {
			typedef int( __thiscall* Fn )( void *, bool );
			return foi::getvfunc< Fn >( this, 43 )( this, procedural );
		}

		void texturedpoly( int vtxCount, Vertex_t *vtx, bool bClipVertices = true ) {
			typedef void( __thiscall* Fn )( void *, int, Vertex_t*, bool );
			return foi::getvfunc< Fn >( this, 106 )( this, vtxCount, vtx, bClipVertices );
		}

		hfont CreateFont() {
			typedef unsigned int( __thiscall* Fn )( void * );
			return foi::getvfunc< Fn >( this, 71 )( this );
		}

		void gettextsize( unsigned long font, const wchar_t* text, int& wide, int& tall ) {
			typedef void( __thiscall* Fn )( void *, unsigned long, const wchar_t*, int&, int& );
			foi::getvfunc< Fn >( this, 79 )( this, font, text, wide, tall );
		}

		void outlinedcircle( int x, int y, int r, int seg ) {
			typedef void( __thiscall* Fn )( void *, int, int, int, int );
			return foi::getvfunc< Fn >( this, 103 )( this, x, y, r, seg );
		}

		void getcursorpos( int &x, int &y ) {
			typedef void( __thiscall* Fn )( void *, int&, int& );
			return foi::getvfunc< Fn >( this, 66 )( this, x, y );
		}
	};
}

struct OverlayText_t {
	Vector			origin;
	bool			bUseOrigin; 	// Align to 4 bytes to avoid padding, stupid valve..
	char			pad0xD[ 3 ];
	int				lineOffset;
	float			flXPos;
	float			flYPos;
	char			text[ 512 ];
	float			m_flEndTime;
	int				m_nServerCount;
	int				m_nCreationTick;
	int				r;
	int				g;
	int				b;
	int				a;
	OverlayText_t	*nextOverlayText;
};	// size==0x23C

class IVDebugOverlay
{
public:
	virtual void AddEntityTextOverlay( int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char *format, ... ) = 0;
	virtual void AddBoxOverlay( const Vector& origin, const Vector& mins, const Vector& maxs, angle const& orientation, int r, int g, int b, int a, float duration ) = 0;
	virtual void AddTriangleOverlay( const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration ) = 0;
	virtual void AddSphereOverlay( const Vector& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration = 0 ) = 0;

	virtual void AddBoxOverlayNew( const Vector& origin, const Vector& mins, const Vector& maxs, const angle& orientation, int r, int g, int b, int a ) = 0; // new according to dude719

	virtual void AddLineOverlay( const Vector& origin, const Vector& dest, int r, int g, int b,bool noDepthTest, float duration ) = 0;
	virtual void AddTextOverlay( const Vector& origin, float duration, const char *format, ... ) = 0;
	virtual void AddTextOverlay( const Vector& origin, int line_offset, float duration, const char *format, ... ) = 0;
	virtual void AddScreenTextOverlay( float flXPos, float flYPos,float flDuration, int r, int g, int b, int a, const char *text ) = 0;
	virtual void AddSweptBoxOverlay( const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const angle & angles, int r, int g, int b, int a, float flDuration ) = 0;
	virtual void AddGridOverlay( const Vector& origin ) = 0;
	
	virtual void AddCoordFrameOverlay( const matrix3x4& frame, float flScale, int vColorTable[ 3 ][ 3 ] = NULL ) = 0; // new

	virtual int ScreenPosition( const Vector& point, Vector& screen ) = 0;
	virtual int ScreenPosition( float flXPos, float flYPos, Vector& screen ) = 0;

	virtual OverlayText_t *GetFirst( void ) = 0;
	virtual OverlayText_t *GetNext( OverlayText_t *current ) = 0;
	virtual void ClearDeadOverlays( void ) = 0;
	virtual void ClearAllOverlays() = 0;

	virtual void AddTextOverlayRGB( const Vector& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char *format, ... ) = 0;
	virtual void AddTextOverlayRGB( const Vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char *format, ... ) = 0;

	virtual void AddLineOverlayAlpha( const Vector& origin, const Vector& dest, int r, int g, int b, int a, bool noDepthTest, float duration ) = 0;
	virtual void AddBoxOverlay2( const Vector& origin, const Vector& mins, const Vector& max, angle const& orientation, const color& faceColor, const color& edgeColor, float duration ) = 0;

	virtual void PurgeTextOverlays() = 0; // new

	virtual void NewOverlay( const Vector& origin1, const Vector& origin2, int a3, int r, int g, int b, int a, int unusedParam ) = 0; // new

private:
	inline void AddTextOverlay( const Vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char *format, ... ) {} /* catch improper use of bad interface. Needed because '0' duration can be resolved by compiler to NULL format string (i.e., compiles but calls wrong function) */
};

class CUserCmd {
public:
	//BYTE	pad[ 4 ]; // either use this or the virtual destructor ( as padding.. )
	virtual ~CUserCmd() {};
	int		command_number;		// 0x04 For matching server and client commands for debugging
	int		tick_count;			// 0x08 the tick the client created this command
	angle	viewangles;			// 0x0C Player instantaneous view angles.
	Vector	aimdirection;		// 0x18
	float	forwardmove;		// 0x24
	float	sidemove;			// 0x28
	float	upmove;				// 0x2C
	int		buttons;			// 0x30 Attack button states
	BYTE    impulse;			// 0x34
	int		weaponselect;		// 0x38 Current weapon id
	int		weaponsubtype;		// 0x3C
	int		random_seed;		// 0x40 For shared random functions
	short	mousedx;			// 0x44 mouse accum in x from create move
	short	mousedy;			// 0x46 mouse accum in y from create move
	bool	hasbeenpredicted;	// 0x48 Client only, tracks whether we've predicted this command at least once
	char	pad_0x4C[ 0x18 ];	// 0x4C Current sizeof( usercmd ) =  100  = 0x64
};

class IClientMode {
public:
	virtual			~IClientMode() {}

	// Called before the HUD is initialized.
	virtual void	InitViewport() = 0;

	// One time init when .dll is first loaded.
	virtual void	Init() = 0;

	// Called when vgui is shutting down.
	virtual void	VGui_Shutdown() = 0;

	// One time call when dll is shutting down
	virtual void	Shutdown() = 0;

	// Called when switching from one IClientMode to another.
	// This can re-layout the view and such.
	// Note that Enable and Disable are called when the DLL initializes and shuts down.
	virtual void	Enable() = 0;

	// Called when it's about to go into another client mode.
	virtual void	Disable() = 0;

	// Called when initializing or when the view changes.
	// This should move the viewport into the correct position.
	virtual void	Layout() = 0;

	// Gets at the viewport, if there is one...
	virtual vgui::ipanel *GetViewport() = 0;

	// Gets at the viewports vgui panel animation controller, if there is one...
	virtual void *GetViewportAnimationController() = 0;

	// called every time shared client dll/engine data gets changed,
	// and gives the cdll a chance to modify the data.
	virtual void	ProcessInput( bool bActive ) = 0;

	// The mode can choose to draw/not draw entities.
	virtual bool	ShouldDrawDetailObjects() = 0;
	virtual bool	ShouldDrawEntity( c_csplayer *pEnt ) = 0;
	virtual bool	ShouldDrawLocalPlayer( c_csplayer *pPlayer ) = 0;
	virtual bool	ShouldDrawParticles() = 0;

	// The mode can choose to not draw fog
	virtual bool	ShouldDrawFog( void ) = 0;

	virtual void	OverrideView( void *pSetup ) = 0;
	virtual int		KeyInput( int down, int keynum, const char *pszCurrentBinding ) = 0;
	virtual void	StartMessageMode( int iMessageModeType ) = 0;
	virtual vgui::ipanel *GetMessagePanel() = 0;
	virtual void	OverrideMouseInput( float *x, float *y ) = 0;
	virtual bool	CreateMove( float flInputSampleTime, CUserCmd *cmd ) = 0;

	virtual void	LevelInit( const char *newmap ) = 0;
	virtual void	LevelShutdown( void ) = 0;
};