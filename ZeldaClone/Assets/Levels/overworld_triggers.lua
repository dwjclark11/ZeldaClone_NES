---------------------------------------------------------
-- Overworld Triggers
---------------------------------------------------------

triggers = {
	-- First Sword Cave Entrance
	[1] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 7424, y = 4768 },
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 8,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "secret_area",
				transport_offset 	= { x = 450, y = 500 },
				camera_offset 		= { x = 0, y = -288 },
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", -- This needs to be changed to sword_cave_colliders
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "sword_cave",
				image_width			= 256,
				image_height		= 176,
				trigger_file		= "sword_cave_triggers"
			},
		}
	},
	-- First Shop Entrance
	[2] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 6592, y = 4096 },
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 8,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "secret_area",
				transport_offset 	= { x = 450, y = 500 },
				camera_offset 		= { x = 0, y = -288 },
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "shop_1",
				image_width			= 256,
				image_height		= 176,
				trigger_file		= "shop_1_triggers"
			},
		}
	},
	-- Eagle Level Entrance
	[3] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 7616, y = 2275},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 8,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "secret_area",
				transport_offset 	= { x = 2300, y = 4000 },
				camera_offset 		= { x = 2048, y = 3072 },
				level_music			= "dungeon",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "no_file", 
				tilemap_name 		= "level1_Eagle_Test",
				tilemap_image   	= "Dungeon_Tiles",
				entity_file			= "no_file",
				trigger_file		= "no_file",
				image_width			= 0,
				image_height		= 0
			},
		}
	},	
}
