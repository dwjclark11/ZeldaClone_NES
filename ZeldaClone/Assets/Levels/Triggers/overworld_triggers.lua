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
				trigger_type		= "scene_change",
				transport_offset 	= { x = 450, y = 500 },
				camera_offset 		= { x = 0, y = -288 }, --{x = 1, y = 1},  
			},
			scene_change = {
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", -- This needs to be changed to sword_cave_colliders
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "sword_cave",
				trigger_file		= "sword_cave_triggers",
				image_width			= 256,
				image_height		= 176,
				
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
				trigger_type		= "scene_change",
				transport_offset 	= { x = 450, y = 500 },
				camera_offset 		= { x = 0, y = -288 },
			},
			scene_change = {
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "shop_1",
				trigger_file		= "shop_1_triggers",
				image_width			= 256,
				image_height		= 176,
			},
		}
	},
	-- Bomb Secret --> Rupees + 30
	[3] = {
		group = "secret",
		components = {
			transform = {
				position = { x = 7616, y = 4096},
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
				collider			= true
			},
			scene_change = {
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "bomb_secret_1",
				trigger_file		= "bomb_1_triggers",
				image_width			= 256,
				image_height		= 176,
			},
			secret = {
				location_id			= "bomb_secret_rupees_1",
				new_trigger			= "scene_change",
				new_sprite_id		= "Overworld_Tiles",
				sprite_width		= 16,
				sprite_height		= 16,
				sprite_src_x		= 64,
				sprite_src_y		= 16
			},
		}
	},	
	-- Eagle Level Entrance
	[4] = {
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
				trigger_type		= "scene_change",
				transport_offset 	= { x = 2505, y = 4545 },
				camera_offset 		= { x = 2048, y = 3776 }, --{x = 3, y = 5},
			},
			scene_change = {
				level_music			= "dungeon",
				asset_file			= "no_file",
				enemy_file			= "Level1_enemies",
				collider_file		= "Test_Colliders", 
				tilemap_name 		= "Level_1_tiles_2",
				tilemap_image   	= "level_1",
				map_image   		= "Dungeon_Tiles",
				entity_file			= "level_1_entities",
				trigger_file		= "level_1_trigger_test",
				image_width			= 1536,
				image_height		= 1184
			},
		}
	},	
	-- Burn Bush Secret -- Shop blue/red potion
	[5] = {
		group = "secret",
		components = {
			transform = {
				position = { x = 8447, y = 5088},
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
				collider			= true
			},
			scene_change = {
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "sword_cave",
				trigger_file		= "sword_cave_triggers",
				image_width			= 256,
				image_height		= 176,
			},
			secret = {
				location_id			= "burn_bush_potion_shop_1",
				new_trigger			= "scene_change",
				new_sprite_id		= "Dungeon_Tiles",
				sprite_width		= 16,
				sprite_height		= 16,
				sprite_src_x		= 64,
				sprite_src_y		= 0
			},
		}
	},	
		-- Push Rock -- Highway Access
	[6] = {
		group = "secret",
		components = {
			transform = {
				position = { x = 9728, y = 5024},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "push_rocks",
                collider = true
			},
			secret = {
				location_id			= "push_rock_to_highway_1",
				new_trigger			= "no_trigger",
				new_sprite_id		= "",
				sprite_width		= 0,
				sprite_height		= 0,
				sprite_src_x		= 0,
				sprite_src_y		= 0
			},
			sprite = {
				asset_id = "Overworld_Tiles",
				width = 16, 
				height = 16,
				z_index = 1,
				is_fixed = false,
				src_rect_x = 16,
				src_rect_y = 0,
				offset = {x = 0, y = 0}
			}
		}
	},	
	[7] = {
        group = "trigger",
        components = {
            transform = {
                position = { x = 9789, y = 5025},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
				offset 	 = { x = 0, y = 0 }
            },
            trigger_box = {
                trigger_type = "scene_change",
				transport_offset 	= { x = 450, y = 500 },
				camera_offset 		= { x = 0, y = -288 },
                collider = false
            }, 
			scene_change = {
                level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "sword_cave",
				trigger_file		= "sword_cave_triggers",
				image_width			= 256,
				image_height		= 176,
			},
			sprite = {
				asset_id = "Overworld_Tiles",
				width = 16, 
				height = 16,
				z_index = 1,
				is_fixed = false,
				src_rect_x = 0,
				src_rect_y = 0,
				offset = {x = 0, y = 0}
			}
        }
    },
	-- Bomb Secret --> Rupees Heart/Red Potion
	[8] = {
		group = "secret",
		components = {
			transform = {
				position = { x = 11838, y = 4768},
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
				collider			= true
			},
			scene_change = {
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "bomb_secret_1",
				trigger_file		= "bomb_1_triggers",
				image_width			= 256,
				image_height		= 176,
			},
			secret = {
				location_id			= "bomb_secret_hearts_1",
				new_trigger			= "scene_change",
				new_sprite_id		= "Overworld_Tiles",
				sprite_width		= 16,
				sprite_height		= 16,
				sprite_src_x		= 64,
				sprite_src_y		= 16
			},
		}
	},
		-- Bomb Secret --> Money Making game 1
	[9] = {
		group = "secret",
		components = {
			transform = {
				position = { x = 12672, y = 4768},
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
				collider			= true
			},
			scene_change = {
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "bomb_secret_2",
				trigger_file		= "bomb_2_triggers",
				image_width			= 256,
				image_height		= 176,
			}, 
			secret = {
				location_id			= "bomb_secret_money_1",
				new_trigger			= "scene_change",
				new_sprite_id		= "Overworld_Tiles",
				sprite_width		= 16,
				sprite_height		= 16,
				sprite_src_x		= 64,
				sprite_src_y		= 16
			},
		}
	},
	-- Bomb Secret --> Rupees 2 : -20
	[10] = {
		group = "secret",
		components = {
			transform = {
				position = { x = 13632, y = 4768},
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
				collider			= true
			},
			scene_change = {
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "bomb_secret_1",
				trigger_file		= "bomb_1_triggers",
				image_width			= 256,
				image_height		= 176,
			},
			secret = {
				location_id			= "bomb_secret_rupees_2",
				new_trigger			= "scene_change",
				new_sprite_id		= "Overworld_Tiles",
				sprite_width		= 16,
				sprite_height		= 16,
				sprite_src_x		= 64,
				sprite_src_y		= 16
			},
		}
	},
	-- Second Shop Entrance: Magic Shield, bombs, arrows
	[11] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 15552, y = 4096 },
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
			},
			scene_change = {
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "shop_2",
				trigger_file		= "shop_2_triggers",
				image_width			= 256,
				image_height		= 176
			},
		}
	},
	-- Third Shop Entrance : Magic Shield, Key, Blue Candle
	[12] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 14784, y = 3424 },
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
			},
			scene_change = {
				level_music			= "stop",
				asset_file			= "no_file",
				enemy_file			= "no_file",
				collider_file		= "secret_dungeon_colliders", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "secret_dungeon",
				entity_file			= "shop_3",
				trigger_file		= "shop_3_triggers",
				image_width			= 256,
				image_height		= 176
			},
		}
	},
	-- Raft Secret to Level 
	[13] = {
		group = "secret",
		components = {
			transform = {
				position = { x = 5632, y = 3552},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "raft",
				transport_offset 	= { x = 5632, y = 3264 },
			},
			secret = {
				location_id			= "none",
				new_trigger			= "no_trigger",
				new_sprite_id		= "none",
				sprite_width		= 0,
				sprite_height		= 0,
				sprite_src_x		= 0,
				sprite_src_y		= 0,
				start_pos			= { x = 0, y = 0 },
				move_up				= true,
				move_down			= false
			},
		}
	},
	-- Raft Secret from Level 4
	[14] = {
		group = "secret",
		components = {
			transform = {
				position = { x = 5632, y = 3328},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "raft",
				transport_offset 	= { x = 5632, y = 3616 },
			},
			secret = {
				location_id			= "none",
				new_trigger			= "no_trigger",
				new_sprite_id		= "none",
				sprite_width		= 0,
				sprite_height		= 0,
				sprite_src_x		= 0,
				sprite_src_y		= 0,
				start_pos			= { x = 0, y = 0 },
				move_up				= false,
				move_down			= true
			},
		}
	},
	-- Ladder Trigger
	[15] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 15936, y = 3680 },
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "ladder",
			},
		}
	},
	-- Ladder Trigger
	[16] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 16064, y = 3680 },
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "ladder",
			},
		}
	},
	-- Ladder Trigger
	[17] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 15936, y = 3008 },
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "ladder",
			},
		}
	},
	-- Ladder Trigger
	[18] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 16064, y = 3008 },
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "ladder",
			},
		}
	},
	-- Fairy Circle Trigger --> Near Level 1
	[19] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 9626, y = 2460},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 64,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "fairy_circle",
			},
		}
	},
	-- Fairy Circle Trigger --> Near Lost Woods
	[20] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 3408, y = 3136},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 64,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "fairy_circle",
			},
		}
	},
}
