-- config data for the SDL window.
title = "In Tartarum";
width = 80;
height = 50;

-- default renderer colour
colour_r = 28;
colour_g = 28;
colour_b = 28;

-- config data for the world map.
map_width = 100;
map_height = 60;
overworld_width = 200;
overworld_height = 150;

-- config data for info panel.
info_panel_height = 10;

--config data for the camera.
camera_x = 1;
camera_y = 1;
camera_zoom = 2;
camera_x_offset = 1;
camera_y_offset = 1;
camera_width = width - 2 * camera_x_offset;
camera_height = height - info_panel_height - 2 * camera_y_offset - 1; 

--config data for the ascii font tiles
tile_path = "Cooz-curses-14x16.png";
tile_width = 14;
tile_height = 16;


--config data for text font.
font_path = "Cooz-curses-14x16.png";
font_width = 14;
font_height = 16;


-- config data for the message log.
message_log_width = 20;
message_log_height = 20;
message_log_number_msgs = 100;
message_log_num_lines = info_panel_height;