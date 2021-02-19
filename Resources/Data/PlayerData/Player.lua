
-- This contains all the player data
player_table = 
{
    position = 
    { 
        x = 19, y = 19, z = 0;
    };
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 0, clip_y = 4, 
        width = 14, height = 16, depth = 1, -- depth = 1 ensures the player will be drawn above everything else
        red = 255, green = 167, blue = 93;
    };
    player = 
    {
        vision = 8;
    };
    blocker = 
    {
        blocks_vision = false;
    };
    actor = 
    {
        name = "Player", description = "It's you!";
    };
    container = 
    {
        weight_capacity = 100;
    };
    body = 
    {
        true;
    };
    id = 
    {
        uid = "player";
    };
}


