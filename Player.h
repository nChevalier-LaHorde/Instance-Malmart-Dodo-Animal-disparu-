#ifndef _H3_PLAYER_H_
#define _H3_PLAYER_H_
#include <h3.h>

#define PLAYER_TYPEID 0x10000001

H3_CAPI_BEGIN_BLOCK
void Player_Terminate(void* properties);
void Player_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void* Player_CreateProperties(H3Handle cam, H3Handle scn, H3Handle gameObject);
void playerOnCollision(H3Handle object, SH3Collision collision);
void playerOnCollisionLeave(H3Handle object, SH3Collision collision);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(Player, float, player_x);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, float, player_y);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, float, walk);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, int, lastKeyPress);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, float, run);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, bool, isBoy);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, int, spotInventory);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, int, playerOnCol);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, H3Handle, objTouch);
H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(Player, bool, iHitAI);
H3_CAPI_END_BLOCK

#define PLAYER_CREATE(CAM, SCN, GAMEOBJECT)														   \
	(SH3Component) {                                                                       \
		.Init               = NULL,                                                        \
		.Terminate          = Player_Terminate,                                 \
		.Update             = Player_Update,                                    \
		.isInitialized      = false,													   \
		.componentType      = PLAYER_TYPEID,                                    \
		.properties         = Player_CreateProperties(CAM, SCN, GAMEOBJECT),						   \
		.OnTriggerEnter     = playerOnCollision,									\
		.OnTriggerLeave     = playerOnCollisionLeave,								\
	}

#endif /* _H3_PLAYER_H_ */#pragma once
