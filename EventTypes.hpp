#pragma once

enum class EventTypes {
	MOVE_NORTH,
	MOVE_NORTH_EAST,
	MOVE_EAST,
	MOVE_SOUTH_EAST,
	MOVE_SOUTH,
	MOVE_SOUTH_WEST,
	MOVE_WEST,
	MOVE_NORTH_WEST,
	BLOCKED_MOVEMENT,
	DESCEND_DUNGEON,
	ASCEND_DUNGEON,
	PLAY_SOUND,
	FADE_OUT_SOUND,
	FADE_IN_SOUND,
	STOP_MUSIC,
	BUTTON_CLICK,
	PLAY_CHUNK,
	BUMP_ATTACK,
	MELEE_HIT,
	CRITICAL_MELEE_HIT,
	RANGED_HIT,
	CRITICAL_RANGED_HIT,
	SPELL_HIT,
	CRITICAL_SPELL_HIT,
	DEATH,
	BUMP_SCRIPT,
	TICK,
	LOAD_GAME,
	SAVE_GAME,
	LEVEL_UP,
	MESSAGE,
	TRY_PICK_UP_ITEM,
	NO_ITEM_PRESENT,
	PICK_UP_ITEM,
	OVERWEIGHT,
	DROP_ITEM, 
	DROP_ITEM_STACK,
	DROP_ITEM_MESSAGE,
	DROP_ITEM_STACK_MESSAGE,
	EQUIP_ITEM,
	UNEQUIP_ITEM,
	SEND_ITEM_TO_ACTION_STATE,
	SEND_ITEM_TO_QUANTITY_STATE,
	SEND_ITEM_TO_TARGETING_STATE,
	SEND_AOE_ITEM_TO_TARGETING_STATE, 
	SEND_USER_TO_TARGETING_STATE,
	SEND_EQUIPPED_ITEM_TO_ACTION_STATE,
	USE_ITEM,
	INVALID_TARGET,
	NO_USE,
	CONSUME,
	LOOK,
	DROP_EQUIPPED_ITEM,
	CREATE_ENTITY,
	OVERWORLD_MOVEMENT,
	ON_USE_SCRIPT,
	ON_EQUIP_SCRIPT,
	ON_UNEQUIP_SCRIPT,
	TEMP_SCRIPT_USE,
	TEMP_SCRIPT_CONSUME,
	HEAL,
	CAST,
	DECREASE_CHARGE,
};