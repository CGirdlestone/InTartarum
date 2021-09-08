#pragma once

#include "World.hpp"
#include "WorldMap.hpp"
#include "MessageLog.hpp"
#include "SoundSystem.hpp"
#include "InventorySystem.hpp"
#include "CombatSystem.hpp"
#include "EntityFactory.hpp"
#include "Lua542/include/lua.hpp"

void registerCombatSystem(lua_State* vm, CombatSystem& combatSystem);
int lua_checkAlive(lua_State* vm);

void registerInventorySystem(lua_State* vm, InventorySystem& inventorySystem);
int lua_decreaseCharge(lua_State* vm);

void registerEntityFactory(lua_State* vm, EntityFactory& entityFactory);
int lua_createMobAt(lua_State* vm);
int lua_createMob(lua_State* vm);
int lua_createItemAt(lua_State* vm);
int lua_createItem(lua_State* vm);

void registerSoundSystem(lua_State* vm, SoundSystem& soundSystem);
int lua_playMusic(lua_State* vm);
int lua_playSFX(lua_State* vm);
int lua_fadeIn(lua_State* vm);
int lua_fadeOut(lua_State* vm);

void registerMessageLog(lua_State* vm, MessageLog& messageLog);
int lua_addMessage(lua_State* vm);

void registerWorldMap(lua_State* vm, WorldMap& worldMap);
int lua_getGrid(lua_State* vm);
int lua_getEntityGrid(lua_State* vm);

void registerEntityGrid(lua_State* vm, EntityGrid& entityGrid);
int lua_isEmpty(lua_State* vm);
int lua_addEntity(lua_State* vm);
int lua_removeEntity(lua_State* vm);
int lua_clear(lua_State* vm);
int lua_getEntitiesAt(lua_State* vm);

void registerEntities(lua_State* vm, std::vector<uint32_t>& entities);
int lua_size(lua_State* vm);
int lua_get(lua_State* vm);

void registerGrid(lua_State* vm, Grid& grid);
int lua_getTile(lua_State* vm);
int lua_inBounds(lua_State* vm);
int lua_getWidth(lua_State* vm);
int lua_getHeight(lua_State* vm);

void registerTile(lua_State* vm, Tile& tile);
int lua_isTileWalkable(lua_State* vm);
int lua_doesTileBlockView(lua_State* vm);
int lua_isTileVisible(lua_State* vm);

void registerWorld(lua_State* vm, World& world);
int lua_createEntity(lua_State* vm);
int lua_killEntity(lua_State* vm);

// need to implement these four functions for all components
int lua_addPosition(lua_State* vm);
int lua_getPosition(lua_State* vm);
int lua_removePosition(lua_State* vm);
int lua_getEntitiesWithPosition(lua_State* vm);

int lua_addFighter(lua_State* vm);
int lua_getFighter(lua_State* vm);
int lua_removeFighter(lua_State* vm);
int lua_getEntitiesWithFighter(lua_State* vm);

int lua_addSprite(lua_State* vm);
int lua_getSprite(lua_State* vm);
int lua_removeSprite(lua_State* vm);
int lua_getEntitiesWithSprite(lua_State* vm);

int lua_addInteractable(lua_State* vm);
int lua_getInteractable(lua_State* vm);
int lua_removeInteractable(lua_State* vm);
int lua_getEntitiesWithInteractable(lua_State* vm);

int lua_addBlocker(lua_State* vm);
int lua_getBlocker(lua_State* vm);
int lua_removeBlocker(lua_State* vm);
int lua_getEntitiesWithBlocker(lua_State* vm);

int lua_addItem(lua_State* vm);
int lua_getItem(lua_State* vm);
int lua_removeItem(lua_State* vm);
int lua_getEntitiesWithItem(lua_State* vm);

int lua_addActor(lua_State* vm);
int lua_getActor(lua_State* vm);
int lua_removeActor(lua_State* vm);
int lua_getEntitiesWithActor(lua_State* vm);
