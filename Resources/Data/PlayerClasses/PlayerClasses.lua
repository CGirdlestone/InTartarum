character_classes = {
    { 
        name = "Archer"; 
        description = "An arrow in the chest will kill a man as good as a sword. No need to put yourself in harm's way for nought."; 
        stats = { 8, 12, 10, 12, 9, 9 };
        sprite = {tilesheet = "Player0.png", clip_x = 2, clip_y = 3};
        hit_die = 6; -- number of sides
    };
    { 
        name = "Cleric"; 
        description = "Many claim they would give their lives for their gods, which is handy as you'd much rather kill for yours."; 
        stats = { 10, 10, 10, 10, 10, 10 };
        sprite = {tilesheet = "Player0.png", clip_x = 0, clip_y = 4};
        hit_die = 8; -- number of sides
    };
    { 
        name = "Rogue"; 
        description = "The ground is filled with honourable men who would insist on playing by the rules. Luckily, you aren't one of them."; 
        stats = { 8, 12, 10, 12, 10, 8 };
        sprite = {tilesheet = "Player0.png", clip_x = 2, clip_y = 4};
        hit_die = 4; -- number of sides
    };
    { 
        name = "Warrior"; 
        description = "You only feel truly alive when you're inches from death, and what better way to be so close to death than being inches from an enemy's sword."; 
        stats = { 12, 10, 12, 10, 8, 8 };
        sprite = {tilesheet = "Player0.png", clip_x = 3, clip_y = 3};
        hit_die = 10; -- number of sides
    };
    { 
        name = "Wizard"; 
        description = "Your path to unlocking the secrets of magic is littered with many more charred bodies than you could ever have imagined."; 
        stats = { 8, 10, 8, 12, 12, 10 };
        sprite = {tilesheet = "Player0.png", clip_x = 7, clip_y = 3};
        hit_die = 4; -- number of sides
    };
}