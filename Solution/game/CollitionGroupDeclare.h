/********************************************************************
	创建时间:	9:12:2010   14:07
	作者:		常齐
	作用:		碰撞组的定义
*********************************************************************/
#pragma once
/// Default collision groups
enum ePhysXCollisionGroup
{
	/// Default collision group
	COLLISION_GROUP_DEFAULT,
	/// Collision group for terrain
	COLLISION_GROUP_TERRAIN,
	/// Collision group for ragdolls
	COLLISION_GROUP_RAGDOLL,
	/// Collision group for player characters
	COLLISION_GROUP_PLAYER_CHARACTER,
	/// Collision group for static objects
	COLLISION_GROUP_STATIC,
	/// Collision group for props
	COLLISION_GROUP_PROP,
	/// Collision group for vehicles
	COLLISION_GROUP_VEHICLE,
	/// Collision group for projectiles
	COLLISION_GROUP_PROJECTILE,
	/// Collision group for triggers
	COLLISION_GROUP_TRIGGER,
	/// Collision group for NPCs
	COLLISION_GROUP_NPC,
	/// Objects in this collision group cannot be pushed, except by a BOSS character
	COLLISION_GROUP_NOPUSH,
	/// Objects in this collision group cannot be pushed, no collision with players
	COLLISION_GROUP_PICKUP,
	/// Same as Player character above, but for AI
	COLLISION_GROUP_AI_CHARACTER,
	/// Collision group specialized for a boss character.
	COLLISION_GROUP_BOSS_CHARACTER,
	/// Objects in this collision group cannot be pushed by a BOSS
	COLLISION_GROUP_NOPUSH_BOSS,
	/// Stores the number of items in this enum
	COLLISION_GROUP_COUNT
};

/// Default collision group masks.
enum ePhysXCollisionGroupMask
{
	/// Mask for default collision group
	COLLISION_MASK_DEFAULT              = (1<<COLLISION_GROUP_DEFAULT),
	/// Mask for terrain collision group
	COLLISION_MASK_TERRAIN              = (1<<COLLISION_GROUP_TERRAIN),
	/// Mask for ragdoll collision group
	COLLISION_MASK_RAGDOLL              = (1<<COLLISION_GROUP_RAGDOLL),
	/// Mask for player character collision group
	COLLISION_MASK_PLAYER_CHARACTER     = (1<<COLLISION_GROUP_PLAYER_CHARACTER),
	/// Mask for static collision group
	COLLISION_MASK_STATIC               = (1<<COLLISION_GROUP_STATIC),
	/// Mask for prop collision group
	COLLISION_MASK_PROP                 = (1<<COLLISION_GROUP_PROP),
	/// Mask for vehicle collision group
	COLLISION_MASK_VEHICLE              = (1<<COLLISION_GROUP_VEHICLE),
	/// Mask for projectile collision group
	COLLISION_MASK_PROJECTILE           = (1<<COLLISION_GROUP_PROJECTILE),
	/// Mask for trigger collision group
	COLLISION_MASK_TRIGGER              = (1<<COLLISION_GROUP_TRIGGER),
	/// Mask for NPC collision group
	COLLISION_MASK_NPC                  = (1<<COLLISION_GROUP_NPC),
	/// Mask for no-push collision group
	COLLISION_MASK_NOPUSH               = (1<<COLLISION_GROUP_NOPUSH),
	/// Mask for pickup collision group
	COLLISION_MASK_PICKUP               = (1<<COLLISION_GROUP_PICKUP),
	/// Mask for AI character collision group
	COLLISION_MASK_AI_CHARACTER         = (1<<COLLISION_GROUP_AI_CHARACTER),
	/// Mask for boss collision group
	COLLISION_MASK_BOSS_CHARACTER       = (1<<COLLISION_GROUP_BOSS_CHARACTER),
	/// Mask for collision group that contains objects that cannot be pushed by bosses
	COLLISION_MASK_NOPUSH_BOSS          = (1<<COLLISION_GROUP_NOPUSH_BOSS),
	/// Mask for all collision groups
	COLLISION_MASK_ALL                  = 0xFFFFFFFFU,
	/// Mask combining collision groups that are immune from explosions
	COLLISION_MASK_EXPLOSION_IMMUNE     = (COLLISION_MASK_TRIGGER | COLLISION_MASK_PICKUP |
	COLLISION_MASK_STATIC | COLLISION_MASK_PLAYER_CHARACTER |
	COLLISION_MASK_TERRAIN | COLLISION_MASK_PROJECTILE),
	/// Mask combining collision groups for ammunition
	COLLISION_MASK_BULLET               =
	(COLLISION_MASK_ALL
	&~COLLISION_MASK_TRIGGER
	&~COLLISION_MASK_PICKUP)
};
