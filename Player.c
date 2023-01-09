#include <stdlib.h>
#include <stdio.h>
#include <components/spritecomponent.h>
#include <components/animatedspritecomponent.h>
#include "Player.h"
#include "Inventory.h"
#include "Weapon.h"
#include "TypeWeapon.h"
#include "TypeMonstere.h"
#include "Monstere.h"
#include "Object.h"
#include "TypeKey.h"
#include "TypeCanTake.h"
#include "TypeDigide.h"
#include "TypeExit.h"
#include "TypeExit2.h"
#include "WeaponHit.h"
#include "EndMenu.h"


typedef struct
{
	H3Handle cam;
	H3Handle gameObject;
	H3Handle scn;
	H3Handle hideView;
	H3Handle hitObj;
	float player_x;
	float player_y;
	float speed;
	float run;
	float walk;
	int spotInventory;
	bool init;
	H3Handle objTouch;
	H3Handle kickObj;
	int playerOnCol;
	int nbrObjTouch;
	float timeKick;
	float w; float h;
	int lastKeyPress;
	bool isBoy;
	int switchToWeapon;
	bool couldHit;
	bool iHitAI;

} Player_Properties;


void* Player_CreateProperties(H3Handle cam, H3Handle scn, H3Handle gameObject)
{
	Player_Properties* properties = malloc(sizeof(Player_Properties));
	H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

	properties->objTouch = NULL;
	properties->speed = 200;
	properties->run = 250;
	properties->walk = 200;
	properties->cam = cam;
	properties->init = true;
	properties->spotInventory = 2;
	properties->scn = scn;
	properties->lastKeyPress = 0;
	properties->isBoy = true;
	properties->couldHit = false;
	properties->switchToWeapon = 0;
	properties->gameObject = gameObject;
	properties->hitObj = H3_Object_Create(scn, "hitObj", NULL);
	properties->iHitAI = false;

	return properties;
}


void Player_Terminate(void* properties)
{
	free(properties);
}


void Player_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) 
{
	Player_Properties* props = (Player_Properties*)properties;
	H3_Transform_GetPosition(H3_Object_GetTransform(object), &props->player_x, &props->player_y);

	if (props->init) {
		props->kickObj = H3_Object_Create2(props->scn, "kickObj", NULL, 3);
		H3_Object_EnablePhysics(object, H3_BOX_COLLIDER(CDT_Dynamic, 24, 22, A_Top, false));
		props->isBoy ? H3_Object_AddComponent(object, SPRITECOMPONENT_CREATE("assets/BoyFront.png", A_Center | A_Middle)) : H3_Object_AddComponent(object, SPRITECOMPONENT_CREATE("assets/GirlFront.png", A_Center | A_Middle));;
		H3_Object_Translate(object, 960, 540);
		H3_Object_AddComponent(props->kickObj, ANIMATEDSPRITECOMPONENT_CREATE("assets/kick.png", A_Center | A_Middle, 1, 0.1, false));
		H3_Object_AddComponent(object, WEAPON_CREATE(props->cam, props->scn));
		H3_Object_AddComponent(object, INVENTORYCOMPONENT_CREATE(object, props->cam, props->scn));
		
		H3_Object_EnablePhysics(props->hitObj, H3_CIRCLE_COLLIDER(CDT_Dynamic, 20, true));
		H3_Object_AddComponent(props->hitObj, HIT_CREATE());
		H3_Object_SetEnabled(props->kickObj, false);

		props->hideView = H3_Object_Create2(props->scn, "hideView", object, 6);
		H3_Object_AddComponent(props->hideView, SPRITECOMPONENT_CREATE("assets/hideView.png", A_Middle + A_Center));

		H3Handle alive = H3_Object_Create2(props->scn, "alive", NULL, 3);
		H3_Object_AddComponent(object, ENDMENUCOMPONENT_CREATE(props->scn, props->cam));

		props->init = false;
	}




	//------------------------------
	props->iHitAI = false;
	
	if(InventoryComponent_GetstockSelectedEx(object) != NULL)
	{

		if (H3_Object_HasComponent(InventoryComponent_GetstockSelectedEx(object), TYPEMONSTERE_TYPEID) == true && H3_Input_IsMouseBtnPressed(MB_Left))
		{
			int a = MonstereComponent_GetuseMonstereEx(ObjectComponent_GetmonstereEx(props->gameObject));
			MonstereComponent_SetuseMonstereEx(ObjectComponent_GetmonstereEx(props->gameObject), a + 1);
			H3_Object_SetEnabled(InventoryComponent_GetstockSelectedEx(object), false);
			int b = InventoryComponent_GetselectedEx(object);
			if (b == 1)
			{
				InventoryComponent_Setstock1Ex(object, NULL);
			}
			else if (b == 2)
			{
				InventoryComponent_Setstock2Ex(object, NULL);
			}
			else if (b == 3)
			{
				InventoryComponent_Setstock3Ex(object, NULL);
			}
		}

		if (props->objTouch != NULL && props->playerOnCol >= 1)
		{
			if (H3_Object_HasComponent(InventoryComponent_GetstockSelectedEx(object), TYPEKEY_TYPEID) == true && H3_Input_IsMouseBtnPressed(MB_Left))
			{
				if (H3_Object_HasComponent(props->objTouch, TYPEKEY_TYPEID) == true)

				{
					H3_Object_SetEnabled(ObjectComponent_GetdoorEx(props->gameObject), false);
					H3_Object_SetEnabled(InventoryComponent_GetstockSelectedEx(object), false);
					int b = InventoryComponent_GetselectedEx(object);
					if (b == 1)
					{
						InventoryComponent_Setstock1Ex(object, NULL);
					}
					else if (b == 2)
					{
						InventoryComponent_Setstock2Ex(object, NULL);
					}
					else if (b == 3)
					{
						InventoryComponent_Setstock3Ex(object, NULL);
					}
				}
			}

			if (H3_Object_HasComponent(InventoryComponent_GetstockSelectedEx(object), TYPEEXIT_TYPEID) == true && H3_Input_IsMouseBtnPressed(MB_Left))
			{
				if (H3_Object_HasComponent(props->objTouch, TYPEEXIT_TYPEID) == true)

				{
					// SCENE WIN
					H3_Object_SetEnabled(ObjectComponent_GetdoorEx(props->gameObject), false);
					H3_Object_SetEnabled(InventoryComponent_GetstockSelectedEx(object), false);
					int b = InventoryComponent_GetselectedEx(object);
					if (b == 1)
					{
						InventoryComponent_Setstock1Ex(object, NULL);
					}
					else if (b == 2)
					{
						InventoryComponent_Setstock2Ex(object, NULL);
					}
					else if (b == 3)
					{
						InventoryComponent_Setstock3Ex(object, NULL);
					}
				}
			}

			if (H3_Object_HasComponent(InventoryComponent_GetstockSelectedEx(object), TYPEEXIT2_TYPEID) == true && H3_Input_IsMouseBtnPressed(MB_Left))
			{
				if (H3_Object_HasComponent(props->objTouch, TYPEEXIT2_TYPEID) == true)

				{
					// SCENE WIN
					H3_Object_SetEnabled(ObjectComponent_GetdoorEx(props->gameObject), false);
					H3_Object_SetEnabled(InventoryComponent_GetstockSelectedEx(object), false);
					int b = InventoryComponent_GetselectedEx(object);
					if (b == 1)
					{
						InventoryComponent_Setstock1Ex(object, NULL);
					}
					else if (b == 2)
					{
						InventoryComponent_Setstock2Ex(object, NULL);
					}
					else if (b == 3)
					{
						InventoryComponent_Setstock3Ex(object, NULL);
					}
				}
			}

		}

		if (H3_Object_HasComponent(InventoryComponent_GetstockSelectedEx(object), TYPEWEAPON_TYPEID) == true )
		{
			H3_Object_SetEnabled(Weapon_GetweaponObjEx(object), true);
			props->couldHit = true;
		}
		else
		{
			H3_Object_SetEnabled(Weapon_GetweaponObjEx(object), false);
			props->couldHit = false;
		}
		
	}
	else
	{
		H3_Object_SetEnabled(Weapon_GetweaponObjEx(object), false);
		props->couldHit = false;
	}

	if (props->objTouch != NULL)
	{
		if (H3_Object_HasComponent(props->objTouch, TYPEDIGIDE_TYPEID) == true && (H3_Input_IsKeyPressed(K_Space) || H3_Input_IsMouseBtnPressed(MB_Left)))
		{
			if (true == true)
			{
				H3_Object_SetEnabled(ObjectComponent_GetdoorDigideEx(props->gameObject), false);
			}
		}
	}

	if (InventoryComponent_GetstockSelectedEx(object) == NULL )
	{

		H3_Object_SetEnabled(Weapon_GetweaponObjEx(object), false);
		props->couldHit = false;
	}
	//----------------------------
	H3_Object_SetTranslation(props->cam, props->player_x, props->player_y);
	H3_Object_SetVelocity(object, 0, 0);

	if (H3_Input_IsKeyDown(K_Shift))
	{
		props->speed = props->run;
	}
	else
	{
		props->speed = props->walk;
	}

	if (H3_Input_IsKeyDown(K_D)) {
		props->isBoy ? SpriteComponent_SetTextureEx(object, H3_Texture_Load("assets/BoyRight.png", &props->w, &props->h)) : SpriteComponent_SetTextureEx(object, H3_Texture_Load("assets/GirlRight.png", &props->w, &props->h));
		H3_Object_AddVelocity(object, props->speed, 0);
		H3_Object_SetTranslation(props->kickObj, props->player_x + 30, props->player_y);
		H3_Object_SetTranslation(props->hitObj, props->player_x + 30, props->player_y);
		props->lastKeyPress = 0;


	}
	if (H3_Input_IsKeyDown(K_Q)) {
		props->isBoy ? SpriteComponent_SetTextureEx(object, H3_Texture_Load("assets/BoyLeft.png", &props->w, &props->h)) : SpriteComponent_SetTextureEx(object, H3_Texture_Load("assets/GirlLeft.png", &props->w, &props->h));
		H3_Object_AddVelocity(object, -props->speed, 0);
		H3_Object_SetTranslation(props->kickObj, props->player_x - 30, props->player_y);
		H3_Object_SetTranslation(props->hitObj, props->player_x - 30, props->player_y);
		props->lastKeyPress = 1;

	}
	if (H3_Input_IsKeyDown(K_S)) {
		props->isBoy ? SpriteComponent_SetTextureEx(object, H3_Texture_Load("assets/BoyFront.png", &props->w, &props->h)) : SpriteComponent_SetTextureEx(object, H3_Texture_Load("assets/GirlFront.png", &props->w, &props->h));
		H3_Object_AddVelocity(object, 0, props->speed);
		H3_Object_SetTranslation(props->kickObj, props->player_x, props->player_y + 35);
		H3_Object_SetTranslation(props->hitObj, props->player_x, props->player_y + 35);
		props->lastKeyPress = 2;

	}
	if (H3_Input_IsKeyDown(K_Z)) {
		props->isBoy ? SpriteComponent_SetTextureEx(object, H3_Texture_Load("assets/BoyBack.png", &props->w, &props->h)) : SpriteComponent_SetTextureEx(object, H3_Texture_Load("assets/GirlBack.png", &props->w, &props->h));
		H3_Object_AddVelocity(object, 0, -props->speed);
		H3_Object_SetTranslation(props->kickObj, props->player_x, props->player_y - 15);
		H3_Object_SetTranslation(props->hitObj, props->player_x, props->player_y - 15);
		props->lastKeyPress = 3;


	}

	if (H3_Input_IsMouseBtnPressed(MB_Left) && props->couldHit==true)
	{
		if (Hit_GetiAmOnCollisionWithAIEx(props->hitObj) == true)
		{
			props->iHitAI = true;
		}

		H3_Object_SetEnabled(props->kickObj, true);
		
		if (props->lastKeyPress == 0)
		{
			AnimatedSpriteComponent_SetTextureEx(props->kickObj, H3_Texture_Load("assets/kick.png", &props->w, &props->h));
			AnimatedSpriteComponent_SetFlipXEx(props->kickObj, false);
			AnimatedSpriteComponent_SetFlipYEx(props->kickObj, false);


		}
		else if (props->lastKeyPress == 1)
		{
			AnimatedSpriteComponent_SetTextureEx(props->kickObj, H3_Texture_Load("assets/kick.png", &props->w, &props->h));
			AnimatedSpriteComponent_SetFlipXEx(props->kickObj, true);
			AnimatedSpriteComponent_SetFlipYEx(props->kickObj, false);

		}
		else if (props->lastKeyPress == 2)
		{
			AnimatedSpriteComponent_SetTextureEx(props->kickObj, H3_Texture_Load("assets/kick2.png", &props->w, &props->h));
			AnimatedSpriteComponent_SetFlipXEx(props->kickObj, false);
			AnimatedSpriteComponent_SetFlipYEx(props->kickObj, true);

		}
		else if (props->lastKeyPress == 3)
		{
			AnimatedSpriteComponent_SetTextureEx(props->kickObj, H3_Texture_Load("assets/kick2.png", &props->w, &props->h));
			AnimatedSpriteComponent_SetFlipXEx(props->kickObj, false);
			AnimatedSpriteComponent_SetFlipYEx(props->kickObj, false);

		}
		props->timeKick = H3_GetTime();
	}

	if (H3_GetTime() - props->timeKick > 0.1)
	{
		H3_Object_SetEnabled(props->kickObj, false);


	}

}

void playerOnCollision(H3Handle obj, SH3Collision col)
{
	SH3Component* component = H3_Object_GetComponent(obj, PLAYER_TYPEID);
	Player_Properties* pe = (Player_Properties*)(component->properties);
	if (col.other == NULL) {}


	pe->objTouch = col.other;
	pe->playerOnCol = 1;

	pe->nbrObjTouch += 1;

}

void playerOnCollisionLeave(H3Handle obj, SH3Collision col)
{
	SH3Component* component = H3_Object_GetComponent(obj, PLAYER_TYPEID);
	Player_Properties* pe = (Player_Properties*)(component->properties);


	if (pe->nbrObjTouch < 2)
	{
		pe->objTouch = NULL;
		pe->playerOnCol = 0;
	}

	pe->nbrObjTouch -= 1;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(Player, PLAYER_TYPEID, float, player_x);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, PLAYER_TYPEID, float, player_y);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, PLAYER_TYPEID, float, walk);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, PLAYER_TYPEID, float, run);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, PLAYER_TYPEID, int, lastKeyPress);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, PLAYER_TYPEID, bool, isBoy);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, PLAYER_TYPEID, int, spotInventory);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, PLAYER_TYPEID, int, playerOnCol);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(Player, PLAYER_TYPEID, H3Handle, objTouch);
H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RO_EX(Player, PLAYER_TYPEID, bool, iHitAI);