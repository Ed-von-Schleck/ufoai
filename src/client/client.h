/**
 * @file client.h
 * @brief Primary header for client.
 */

/*
All original material Copyright (C) 2002-2009 UFO: Alien Invasion.

Original file from Quake 2 v3.21: quake2-2.31/client/client.h
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "cl_renderer.h"
#include "cl_video.h"
#include "sound/s_main.h"
#include "input/cl_input.h"
#include "input/cl_keys.h"
#include "battlescape/cl_camera.h"
#include "battlescape/cl_localentity.h"
#include "battlescape/cl_battlescape.h"

/* Map debugging constants */
/** @brief cvar debug_map options:
 * debug_map is a bit mask, like the developer cvar.  There is no ALL bit.
 * 1 (1<<0) = Turn on pathfinding floor tiles.  Red can not be entered, green can be
 *     reached with current TUs, yellow can be reached but actor does not have needed
 *     TUs, and black should never be able to be reached because there is not floor to
 *     support an actor there (not tuned for flyers yet)
 * 2  (1<<1) = Turn on cursor debug text display.  The information at the top is the
 *     ceiling value where the cursor is and the (x, y, z) coordinates for the cursor
 *     *at the current level*.  The z value will always match the level - 1.  The bottom
 *     information is the floor value at teh cursor at the current level and the location
 *     of the actual displayed cursor.  The distinction is made because our GUI currently
 *     always places the cursor on the ground beneath the mouse pointer, even if the
 *     ground is a few levels beneath the current level.  Eventually, we will need to be
 *     able to disable that for actors that can fly and remain in the air between turns,
 *     if any.
 * 4 (1<<2) = Turn on obstruction arrows for the floor and the ceiling.  These arrows point
 *     to the ceiling surface and the floor surface for teh current cell.
 * 8 (1<<3) = Turn on obstruction arrows.  These currently look broken from what I've
 *     seen in screenshots.  These arrows are supposed to point to the brushes that limit
 *     movement in a given direction. The arrows either touch the floor/ceiling of the
 *     current cell in the tested direction if it is possible to move in that direction,
 *     or touch the obstruction (surface) that prevents movement in a given direction.
 *     I want to redo these anyhow, as they are meant for map editors to be able to see what
 *     surfaces are actually preventing movement at the routing level.  Feedback is encouraged.
*/
#define MAPDEBUG_PATHING	(1<<0) /* Turns on pathing tracing. */
#define MAPDEBUG_TEXT		(1<<1) /* Creates arrows pointing at floors and ceilings at mouse cursor */
#define MAPDEBUG_CELLS		(1<<2) /* Creates arrows pointing at floors and ceilings at mouse cursor */
#define MAPDEBUG_WALLS		(1<<3) /* Creates arrows pointing at obstructions in the 8 primary directions */

/* Macros for faster access to the inventory-container. */
#define CONTAINER(e, containerID) ((e)->i.c[(containerID)])
#define ARMOUR(e) ((e)->i.c[csi.idArmour])
#define RIGHT(e) ((e)->i.c[csi.idRight])
#define LEFT(e)  ((e)->i.c[csi.idLeft])
#define FLOOR(e) ((e)->i.c[csi.idFloor])
#define HEADGEAR(e) ((e)->i.c[csi.idHeadgear])
#define EXTENSION(e) ((e)->i.c[csi.idExtension])

typedef enum {
	ca_uninitialized,
	ca_disconnected,			/**< not talking to a server */
	ca_sequence,				/**< rendering a sequence */
	ca_connecting,				/**< sending request packets to the server */
	ca_connected,				/**< netchan_t established, waiting for svc_serverdata */
	ca_active					/**< game views should be displayed */
} connstate_t;

/**
 * @brief Not cleared on a map change (static data)
 * @note Only some data is set to new values on a map change
 * @note the @c client_static_t structure is persistant through an arbitrary
 * number of server connections
 * @sa client_state_t
 */
typedef struct client_static_s {
	connstate_t state;
	keydest_t keyDest;

	int realtime;				/**< always increasing, no clamping, etc */
	float frametime;			/**< seconds since last frame */
	float framerate;

	/** showing loading plaque between levels if time gets > 30 seconds ahead, break it */
	float disableScreen;

	int gametype;		/**< singleplayer or multiplayer */

	/* connection information */
	char servername[MAX_VAR];		/**< name of server from original connect */
	char serverport[16];			/**< port the server is running at */
	int connectTime;				/**< for connection retransmits */
	int waitingForStart;			/**< waiting for EV_START or timeout */

	struct datagram_socket *netDatagramSocket;
	struct net_stream *netStream;

	int challenge;				/**< from the server to use for connecting */

	/** needs to be here, because server can be shutdown, before we see the ending screen */
	int team;			/**< the team you are in @sa TEAM_CIVILIAN, TEAM_ALIEN */

	float loadingPercent;
	char loadingMessages[96];

	int playingCinematic;	/**< Set to cinematic playing flags or 0 when not playing */

	int currentSelectedMap;	/**< current selected multiplayer or skirmish map */

	char downloadName[MAX_OSPATH];
	size_t downloadPosition;
	int downloadPercent;

	dlqueue_t downloadQueue;	/**< queue of paths we need */
	dlhandle_t HTTPHandles[4];	/**< actual download handles */
	char downloadServer[512];	/**< base url prefix to download from */
	char downloadReferer[32];	/**< libcurl requires a static string :( */
	CURL *curl;

	/** these models must only be loaded once */
	struct model_s *modelPool[MAX_OBJDEFS];

	/** this pool is reloaded on every sound system restart */
	s_sample_t *soundPool[MAX_SOUNDIDS];

	/* unique character id */
	int nextUniqueCharacterNumber;

	inventoryInterface_t i;
} client_static_t;

extern client_static_t cls;

extern struct memPool_s *cl_genericPool;
extern struct memPool_s *cl_ircSysPool;
extern struct memPool_s *cl_soundSysPool;

/*============================================================================= */

/* i18n support via gettext */
#include <libintl.h>

/* the used textdomain for gettext */
#define TEXT_DOMAIN "ufoai"
#include <locale.h>
#define _(String) gettext(String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

/* cvars */
extern cvar_t *cl_leshowinvis;
extern cvar_t *cl_fps;
extern cvar_t *cl_worldlevel;
extern cvar_t *cl_selected;
extern cvar_t *cl_team;
extern cvar_t *cl_teamnum;

extern cvar_t *s_language;

/* cl_main.c */
void CL_SetClientState(int state);
void CL_Disconnect(void);
void CL_Init(void);

void CL_ClearState(void);

#endif /* CLIENT_CLIENT_H */
