bat = {
    actor = {name = "bat", description = "You wouldn't believe one of these could grind civilization to halt with a simple virus!"};
    ai = {state = "neutral", blind = true, scent = false};
    sprite = 
    {
        tilesheet = "Cooz-curses-14x16.png", 
        clip_x = 2, clip_y = 6, 
        width = 14, height = 16, depth = 10, -- depth = 10 ensures the player will be drawn above everything else
        red = 100, green = 149, blue = 237,
    }; 
    fighter = {hp = 10, defence = 3, attributes = { 10, 10, 10, 10, 10, 10 }};
    -- status effects
        -- bleeding - damage per turn
        -- stunned - miss turns
        -- exhausted - actions cost more energy 
        -- poisoned - poison damage per turn
        -- burned - fire damage per turn
}