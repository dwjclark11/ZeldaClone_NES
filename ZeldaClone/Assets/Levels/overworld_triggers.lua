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
	-- Test Secret 
	[4] = {
		group = "secret",
		components = {
			transform = {
				position = { x = 7600, y = 4768},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "bomb_secret",
				transport_offset 	= { x = 450, y = 500 },
				camera_offset 		= { x = 0, y = -288 },
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "sword_cave",
				trigger_file		= "sword_cave_triggers",
				image_width			= 256,
				image_height		= 176
			},
			secret = {
				location_id			= "Sword_cave_Area",
				new_trigger			= "secret_area",
				new_sprite_id		= "Dungeon_Tiles",
				sprite_width		= 16,
				sprite_height		= 16,
				sprite_src_x		= 64,
				sprite_src_y		= 0
			},
		}
	},	
		-- Test Secret 
	[5] = {
		group = "secret",
		components = {
			transform = {
				position = { x = 8576, y = 5012},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "burn_bush",
				transport_offset 	= { x = 450, y = 500 },
				camera_offset 		= { x = 0, y = -288 },
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "sword_cave",
				trigger_file		= "sword_cave_triggers",
				image_width			= 256,
				image_height		= 176
			},
			secret = {
				location_id			= "Burn_Bush_Area",
				new_trigger			= "secret_area",
				new_sprite_id		= "Dungeon_Tiles",
				sprite_width		= 16,
				sprite_height		= 16,
				sprite_src_x		= 64,
				sprite_src_y		= 0
			},
		}
	},	
}
