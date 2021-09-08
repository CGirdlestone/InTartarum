---- Global Variables ----
--------------------------
-- World - the entity-component database 
-- WorldMap - contains the level grid and the entity grid
-- MessageLog - contains all the messages that will be printed to the log
-- SoundSystem - controls the playing of music and sound effects

---- Global Entities ----
-------------------------
-- actor - the game entity that is carrying out an action (e.g. the spell caster) - never nil
-- target - the target - can be nil 
-- item - the item in use - can be nil



-- This runs immediately once the entity has finished being created.
function OnInit(actor)


end


-- This runs every frame
function OnUpdate(dt, actor)


end


-- This runs every game tick
function OnTick(actor)


end


-- This runs when the actor collides with "other"
function OnBump(actor, other)


end


-- This runs when the actor dies
function OnDeath(actor)


end

-- This runs when the actor equips the item
function OnEquip(actor, item)


end

-- This runs when the actor unequips the item
function OnUnequip(actor, item)


end


-- This runs when the actor uses the item (e.g. health potion), or it may involve a target (e.g. a fireball)
function OnUse(actor, target, item)

	
end


-- This runs when the actor makes a successful attack against the target
function OnHit(actor, target)


end