ID = 
{
    ["HEAD"] = 0,
    ["CHEST"]  = 1,
    ["LEFT_HAND"] = 2,
    ["RIGHT_HAND"] = 3,
    ["NECK"] = 4,
    ["LEGS"] = 5,
    ["HANDS"] = 6,
    ["BOOTS"] = 7,
    ["RING"] = 8,
    ["QUIVER"] = 9,
    ["BACK"] = 10,
}

UseableType =
{
    ["CONSUMABLE"] = 0,
    ["TARGETED"] = 1,
    ["TARGETED_AOE"] = 2,
}

width = 14; -- used for the animation frame clipping
height = 16; -- used for the animation frame clipping

arrow = 
{
    item = 
    {
        name = "arrow", description = "A wooden arrow.", weight = 1;
    };
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 15, clip_y = 2, 
        width = width, height = height, depth = 1,
        red = 187, green = 170, blue = 153,
    };
    equipable = 
    {
        slot = ID["QUIVER"]
    };
    stackable = 
    {
        true,
    };
    id = 
    {
        uid = "arrow";
    };
}

bow = 
{
    item =
    {
        name ="bow", description = "A simple oak flatbow.", weight = 5;
    };
    sprite =
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 13, clip_y = 7, 
        width = width, height = height, depth = 1,
        red = 187, green = 170, blue = 153,
    };
    equipable = 
    {
        slot = ID["LEFT_HAND"];
    };
    weapon = 
    {
        num_dice = 2,
        sides = 6,
        ranged = true,
        ammo = "arrow";
    };
    id =
    {
        uid = "flatbow";
    };
}

health_potion = 
{
    item = 
    {
        name = "health potion", description = "A vial of thick, redish liquid.", weight = 1;
    };
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 1, clip_y = 2, 
        width = width, height = height, depth = 1,
        red = 205, green = 92, blue = 92,
    };
    stackable = 
    {
        true,
    };
    id = 
    {
        uid = "health_potion";
    };
    useable = 
    {
        type = UseableType["CONSUMABLE"];
        charges = 1;
    };
    script = 
    {
        scriptFile = "HealthPotion.lua"
    };
}

fireball_scroll = 
{
    item = 
    {
        name = "scroll of fireball", description = "A piece of parchment which could destroy a whole town.", weight = 1;
    };
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 14, clip_y = 7, 
        width = width, height = height, depth = 1,
        red = 205, green = 92, blue = 92,
    };
    stackable = 
    {
        true,
    };
    id = 
    {
        uid = "fireball_scroll";
    };
    useable = 
    {
        type = UseableType["TARGETED_AOE"];
        charges = 1;
    };
    script = 
    {
        scriptFile = "Fireball.lua"
    };
}

fire_sword = 
{
    item = 
    {
        name = "sword of fire", description = "A blazin' sword.", weight = 5;
    };
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 15, clip_y = 2, 
        width = width, height = height, depth = 1,
        red = 205, green = 92, blue = 92,
    };
    equipable = 
    {
        slot = ID["RIGHT_HAND"];
    };
    script = 
    {
        scriptFile = "FireSword.lua"
    };
    weapon = 
    {
        num_dice = 2,
        sides = 8,
        ranged = false,
        ammo = "";
    };
    id = 
    {
        uid = "fire_sword";
    };
    useable = 
    {
        type = UseableType["TARGETED"];
    };
}

