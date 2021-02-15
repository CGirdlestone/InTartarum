npc = 
{
    actor = {name = "Steve", description = "A simple merchant."};
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 2, clip_y = 0, 
        width = 14, height = 16, depth = 1, -- depth = 10 ensures the player will be drawn above everything else
        red = 187, green = 170, blue = 153,
    }; 
    blocker = 
    {
        blocks_view = false;
    };
}