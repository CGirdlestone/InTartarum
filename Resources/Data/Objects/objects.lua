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
        slot = ID["RIGHT_HAND"]
    };
    script = 
    {
        --OnInit = "";
        --OnUpdate = "";
        --OnBump = "";
        --OnDeath = "";
        OnEquip = "buff_strength",
        OnUnequip = "debuff_strength",
        --OnUse = "";
        --OnHit = "";
    };
    weapon = 
    {
        num_dice = 2,
        sides = 8,
    };
    id = 
    {
        uid = "fire_sword";
    };
}

chest = 
{
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 11, clip_y = 13, 
        width = width, height = height, depth = 1,
        red = 187, green = 170, blue = 153,
    };
    script = 
    {
        --OnInit = "";
        --OnUpdate = "";
        OnBump = "OPEN_CHEST";
        --OnDeath = "";
        --OnEquip = "buff_strength",
        --OnUnequip = "debuff_strength",
        --OnUse = "";
        --OnHit = "";
    };
    blocker = 
    {
        blocks_view = true;
    };
    interactable = 
    {
        repeatable = true;
    };
    animation = 
    {
        lifetime = 0.1;
        dynamic = false;
        frames = 
        {
            { tilesheet = "Cooz-curses-14x16.png", clip_x = 12 * width, clip_y = 13 * height, width = width, height = height };
            { tilesheet = "Cooz-curses-14x16.png", clip_x = 11 * width, clip_y = 13 * height, width = width, height = height };
        }
    };
    id = 
    {
        uid = "chest";
    };
}

door = 
{
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 11, clip_y = 2, 
        width = width, height = height, depth = 1,
        red = 187, green = 170, blue = 153,
    };
    script = 
    {
        --OnInit = "";
        --OnUpdate = "";
        OnBump = "OPEN_DOOR";
        --OnDeath = "";
        --OnEquip = "",
        --OnUnequip = "",
        --OnUse = "";
        --OnHit = "";
    };
    blocker = 
    {
        blocks_view = true;
    };
    interactable = 
    {
        repeatable = true;
    };
    animation = 
    {
        lifetime = 0.1;
        dynamic = false;
        frames = 
        {
            { tilesheet = "Cooz-curses-14x16.png", clip_x = 13 * width, clip_y = 2 * height, width = width, height = height };
            { tilesheet = "Cooz-curses-14x16.png", clip_x = 11 * width, clip_y = 2 * height, width = width, height = height };
        }
    };
    id = 
    {
        uid = "door";
    };
}

camp_fire = 
{
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 5, clip_y = 1, 
        width = width, height = height, depth = 1,
        red = 205, green = 92, blue = 92,
    };
    blocker = 
    {
        blocks_view = false;
    };
    animation = 
    {
        lifetime = 0.1;
        dynamic = true;
        frames = 
        {
            { tilesheet = "Cooz-curses-14x16.png", clip_x = 4 * width, clip_y = 2 * height, width = width, height = height };
            { tilesheet = "Cooz-curses-14x16.png", clip_x = 5 * width, clip_y = 1 * height, width = width, height = height };
        }
    };
    id = 
    {
        uid = "camp_fire";
    };
}
