attitude = 
{
    ["neutral"] = 0,
    ["hostile"] = 1,
    ["friendly"] = 2,
}

bat = {
    actor = 
    {
        name = "bat", 
        description = "You wouldn't believe one of these could grind civilization to halt with a simple virus!"
    };
    ai = 
    {
        state = attitude["hostile"], 
        smart = false,
        blind = true, 
        scent = false,
        xp = 10;
    };
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 2, clip_y = 6, 
        width = 14, height = 16, depth = 5, -- depth = 10 ensures the player will be drawn above everything else
        red = 100, green = 149, blue = 237,
    }; 
    fighter = 
    {
        hp = 10, defence = 1, str = 5, dex = 5, con = 5, wis = 5, int = 5, cha = 5,
    };
    id = 
    {
        uid = "bat";
    };
    blocker = 
    {
        blocks_view = false;
    };
}

goblin = {
    actor = 
    {
        name = "goblin", 
        description = "A gangly, ugle goblin!"
    };
    ai = 
    {
        state = attitude["hostile"], 
        smart = true,
        blind = false, 
        scent = false,
        xp = 10;
    };
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 7, clip_y = 6, 
        width = 14, height = 16, depth = 5, -- depth = 10 ensures the player will be drawn above everything else
        red = 134, green = 175, blue = 128,
    }; 
    fighter = 
    {
        hp = 20, defence = 3, str = 9, dex = 8, con = 10, wis = 12, int = 8, cha = 5,
    };
    id = 
    {
        uid = "goblin";
    };
    blocker = 
    {
        blocks_view = false;
    };
    container = 
    {
        weight_capacity = 100;
        --starting_equipment = 
        --{ 
        --    "health_potion", "fireball_scroll";
        --};
    };
    body = 
    {
        true;
    };
}
