---------------------------------------------------------
-- Shop 3 Triggers
---------------------------------------------------------

triggers = {
	-- First Sword Cave Entrance
	[1] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 448, y = 600},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 32,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "secret_area",
				transport_offset 	= { x = 14784, y = 3488 },
				camera_offset 		= { x = 14336, y = 3072 },
				level_music			= "Overworld",
				asset_file			= "game_state_assets",
				enemy_file			= "overworld_enemies",
				collider_file		= "overworld_colliders_1", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "map",
				entity_file			= "no_file",
				trigger_file		= "overworld_triggers",
				image_width			= 4096,
				image_height		= 1344
			},
		}
	},
}