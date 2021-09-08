width = 14; -- used for the animation frame clipping
height = 16; -- used for the animation frame clipping

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
        scriptFile = "Door.lua"
        --OnInit = "";
        --OnUpdate = "";
        --OnBump = "OPEN_DOOR";
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