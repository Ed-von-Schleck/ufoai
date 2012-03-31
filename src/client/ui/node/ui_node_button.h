/**
 * @file
 */

/*
Copyright (C) 2002-2011 UFO: Alien Invasion.

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

#ifndef CLIENT_UI_UI_NODE_BUTTON_H
#define CLIENT_UI_UI_NODE_BUTTON_H

#include "../ui_nodes.h"

class uiButtonNode : public uiLocatedNode {
	void draw(struct uiNode_s *node) OVERRIDE;
	void loading(struct uiNode_s *node) OVERRIDE;
	void loaded(struct uiNode_s *node) OVERRIDE;
	void leftClick(struct uiNode_s *node, int x, int y) OVERRIDE;
};

typedef struct buttonExtraData_s {
	struct uiSprite_s *icon;	/**< Link to an icon */
	qboolean flipIcon;			/**< Flip the icon rendering (horizontal) */
} buttonExtraData_t;

void UI_RegisterButtonNode(struct uiBehaviour_s *behaviour);

#endif
