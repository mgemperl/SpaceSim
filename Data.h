#ifndef _DATA_H  
#define _DATA_H 
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>
#include <vector>
#include <unordered_map>

// Definitions

#define LP_SPRITE LPD3DXSPRITE
#define LP_TEXTURE LPDIRECT3DTEXTURE9

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }

#ifndef RANDOM
#define RANDOM (double(rand()) / double(RAND_MAX))
#define RANDSIGN ((rand() % 2) ? -1.0 : 1.0)
#define RANDBOOL ((rand() % 2) ? false : true)
#endif

//-----------------------------------------------
// Constants
//-----------------------------------------------

// window
const char CLASS_NAME[] = "SpaceSim";
const char GAME_TITLE[] = "SpaceSim Experimental Build";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  1024U;
const UINT GAME_HEIGHT = 800U;
 
// game
#ifndef PI
#define PI 3.14159265
const double TWO_PI = PI * 2.0;
const double HALF_PI = PI / 2.0;
const double THIRD_PI = PI / 3.0;
const double QUARTER_PI = PI / 4.0;
const double EIGHTH_PI = PI / 8.0;
const double SIXTEENTH_PI = PI / 16.0;
#endif

#ifndef THETA_E
#define THETA_E 0.0001
#endif

const float FRAME_RATE  = 80.0f;               // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 1.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float PHYS_UPDATE_RATE = 1.0f / 25.0f;		// Seconds between each physics update



// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key

// Structs and Enums
namespace SpaceSimNS
{
	enum ShipClass;
	enum ClassType;
	enum Faction;
	enum Action;
	enum StateType;
	enum WeaponSize;
	struct WeapDef;
	struct SlotDef;
	struct ShipStats;
	struct ShipDef;
	enum MessageType;
	struct Telegram;
	struct defs;

	enum ShipClass // ordered by size
	{
		NONSHIP = 0,
		MISSILE = 1,
		FIGHTER = 2,
		CORVETTE = 3,
		FRIGATE = 4,
		SMALLFREIGHTER = 5,
		DESTROYER = 6,
		CRUISER = 7,
		MEDIUMFREIGHTER = 8,
		FASTCARRIER = 9,
		BATTLECRUISER = 10,
		LARGEFREIGHTER = 11,
		ESCORTCARRIER = 12,
		BATTLESHIP = 13,
		SUPERFREIGHTER = 14,
		DREADNAUGHT = 15,
		SUPERCARRIER = 16
	};

	enum ClassType
	{
		GUIDANCEAI,
		PLAYER,
		SHIPAI,
		VESSEL,
		GUIDEDPROJECTILE,
		PROJECTILE,
		MOVEABLE,
		BACKGROUND,
		ENTITY,
		WEAPONSLOT
	};

	enum Faction
	{
		SWISS,
		PLAYERFACTION,
		ENEMYFACTION
	};

	const int NUM_ACTIONS = 14;
	enum Action // Directly control vessel
	{
		FIRE,		// Fire selected weapon
		LAUNCH,		// Launch selected guided projectile
		TURNCW,		// Turn clockwise
		TURNCCW,	// Turn counter-clockwise
		ACCEL,		// Accelerate forward
		BACK,		// Turn toward inverse of velocity vector
		LEFT,
		RIGHT,
		LAND,		// Land on selected planet or station
		BOARD,		// Board selected disabled ship
		CONTACT,	// Contact selected ship, station, or planet
		TARGET,     // Target next ship in sector
		BESTTARGET, // Target ideal ship in sector
		UNTARGET    // Cancel targeting
	};

	enum StateType // AI state
	{
		// Ship Maneuvers
		IDLE,
		FORMUP,
		APPROACH,
		STOP,
		PASS,
		STRAFEAPPROACH,
		STRAFEPASS,
		BREAK,
		FLEE,
		CIRCLE,
		ATTEMPTJUMP,
		ATTEMPTLANDING,

		// Ship Orders
		STANDBY,
		SEARCHANDDESTROY,
		ESCORT,
		FORMATION,
		PATROL,
		RETREAT,
		TRAVEL,
		MANUALCONTROL,

		// Turret targeting settings
		ATTACKSHIPTARGET,
		ATTACKBESTTARGET,

		// Turret tracking settings
		TRACKTARGET,
		REMAINSTATIONARY,

		// Missile Settings
		SEEKTARGET,

		// Game State
		ENCOUNTER,
		CAMPAIGN

	};

	// Ship component definition
	struct CompDef
	{

	};

	enum WeaponSize
	{
		POINT = 0,
		SMALL = 1,
		MEDIUM = 2,
		LARGE = 3,
		MASSIVE = 4
	};

	struct Definitions
	{
		// Where the keys are the definitions' names
		std::unordered_map<const char*, ShipDef> shipDefs;
		std::unordered_map<const char*, CompDef> compDefs;
		std::unordered_map<const char*, WeapDef> weapDefs;
	};

	// Ship weapon definition
	struct WeapDef
	{
		const char* name;
		const char* textureName;

		double reloadTime;		// in seconds
		WeaponSize size;

		// Projectile stats
		bool isGuided;
		double speed;
		int hullDam;
		int shieldDam;
		int lifetime;


		WeapDef(){}
		WeapDef(const char* inName,
			int inReloadTime,
			WeaponSize inSize,
			bool inIsGuided,
			double inSpeed,
			int inHullDam,
			int inShieldDam,
			int inLifetime)
		{
			name = inName;
			reloadTime = inReloadTime;
			size = inSize;
			isGuided = inIsGuided;
			speed = inSpeed;
			hullDam = inHullDam;
			shieldDam = inShieldDam;
			lifetime = inLifetime;
		}
	};

	struct SlotDef
	{
		// Polar coordinates of slot with respect to ship center
		double angleOff; // in radians
		double magOff;

		WeaponSize size;
		bool isTurret;
		double facing;
		double firingArc;
		double bufferArc;

		SlotDef(){}
		SlotDef(double inAngleOff,
			double inMagOff,
			WeaponSize inWeapSize,
			bool inIsTurret,
			double inFacing,
			double inFiringArc,
			double inBufferArc)
		{
			angleOff = inAngleOff;
			magOff = inMagOff;
			size = inWeapSize;
			isTurret = inIsTurret;
			facing = inFacing;
			firingArc = inFiringArc;
			bufferArc = inBufferArc;
		}
	};

	struct ShipStats
	{
		int maxHull;
		int hull;
		int maxShield;
		int shield;
		double forAccelRate;
		double sideAccelRate;
		double backAccelRate;
		double turnRate;
		double maxSpeed;
		int mass;
		int space;

		ShipStats(){}
		ShipStats(int inMaxHull,
			int inHull,
			int inMaxShield,
			int inShield,
			double inForAccelRate,
			double inSideAccelRate,
			double inBackAccelRate,
			double inTurnRate,
			double inMaxSpeed,
			int inMass,
			int inSpace)
		{
			maxHull = inMaxHull;
			hull = inHull;
			maxShield = inMaxShield;
			shield = inShield;
			forAccelRate = inForAccelRate;
			sideAccelRate = inSideAccelRate;
			backAccelRate = inBackAccelRate;
			turnRate = inTurnRate;
			maxSpeed = inMaxSpeed;
			mass = inMass;
			space = inSpace;
		}
	};

	// Ship definition
	struct ShipDef
	{
		const char* name;
		const char* texture;
		ShipClass shipClass;
		ShipStats stats;
		Faction allegience;
		std::vector<SlotDef> weapSlots;

		ShipDef(){}
		ShipDef(const char* inName,
			const char* inTexture,
			ShipClass inClass,
			ShipStats inStats,
			Faction inAllegience)
		{
			name = inName;
			texture = inTexture;
			shipClass = inClass;
			stats = inStats;
			allegience = inAllegience;
		}
	};

	

	enum MessageType
	{
		PROJECTILEIMPACT,
		TARGETED,
		UNTARGETED,
		MISSILELOCK,
		MISSILEUNLOCK,
		DESTROY
	};

	struct Telegram
	{
		typedef unsigned long TICK;

		UINT senderID;
		UINT receiverID;
		MessageType type;
		UINT delay;
		TICK dispatchTick;
		void* info;

		Telegram(
			UINT inSenderID,
			UINT inReceiverID,
			MessageType inType,
			TICK inDelay,
			void* inInfo)
		{
			senderID = inSenderID;
			receiverID = inReceiverID;
			type = inType;
			delay = inDelay;
			info = inInfo;
		}

		~Telegram()
		{
			if (info != NULL)
			{
				delete info;
			}
		}
	};

	inline bool operator==(const Telegram& lhs, const Telegram& rhs)
	{
		return (
			(lhs.senderID == rhs.senderID) &&
			(lhs.receiverID == rhs.receiverID) &&
			(lhs.type == rhs.type) &&
			(lhs.delay == rhs.delay) &&
			(lhs.dispatchTick == rhs.dispatchTick) &&
			(lhs.info == rhs.info)
			);
	}

	inline bool operator!=(const Telegram& lhs, const Telegram& rhs)
	{
		return (!(lhs == rhs));
	}

	inline bool operator<(const Telegram& lhs, const Telegram& rhs)
	{
		bool comp = false;

		if (lhs != rhs)
		{
			comp = (lhs.dispatchTick < rhs.dispatchTick);
		}

		return comp;
	}

	template <class T>
	inline T Dereference(const void* pointer)
	{
		return *(T*)(pointer);
	}

}

#endif
