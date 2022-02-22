--------------------------------------------------------------------
--   SWORD CAVE ENTITIES
--------------------------------------------------------------------

level = {
	[1] = {
		level_item = "wood_sword"
	},
	-- FIRE LEFT
	[2] ={
		group = "NPC",	
		components = {
			transform = {
				position = { x = 250, y = 200},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "npcs",
				width = 32,
				height = 32,
				layer = 4,
				fixed = false,
				src_rect_x = 0, 
				src_rect_y = 0 
			}, 
			animation = {
				num_frames = 2,
				frame_rate = 10,
				vertical = true, 
				looped = true, 
				frame_offset = 0
			} 
		}
	},
	-- FIRE RIGHT
	[3] ={
		group = "NPC",	
		components = {
			transform = {
				position = { x = 650, y = 200},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "npcs",
				width = 32,
				height = 32,
				layer = 4,
				fixed = false,
				src_rect_x = 0, 
				src_rect_y = 0 
			}, 
			animation = {
				num_frames = 2,
				frame_rate = 10,
				vertical = true, 
				looped = true, 
				frame_offset = 0
			}
		}
	},
	-- OLD MAN
	[4] ={
		group = "NPC",	
		components = {
			transform = {
				position = { x = 464, y = 200},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "npcs",
				width = 32,
				height = 32,
				layer = 4,
				fixed = false,
				src_rect_x = 32, 
				src_rect_y = 0 
			}
		}
	},
	-- WOOD SWORD
	[5] ={
		group = "trigger",	
		components = {
			transform = {
				position = { x = 464, y = 332},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "Sword",
				width = 32,
				height = 32,
				layer = 1,
				fixed = false,
				src_rect_x = 0, 
				src_rect_y = 0 
			},
			box_collider = {
				width = 16,
				height = 16,
				offset_x = 32,
				offset_y = 32
			},
			trigger_box = {
				trigger_type = "collect_item", -- Collect Item -- Need to convert to enum 
				transport_offset = { x = 0, y = 0},
				camera_offset = {x = 0, y = 0},
			},
			item = {
				item_type = "default",
				special = "wood_sword"
			}
		}
	},
	-- Caption
	[6] ={
		group = "caption",	
		components = {
			caption = {
				captions = "It's Dangerous to go Alone! Take this",
				x_pos = 235,
				y_pos = 135,
				num_frames = 37,
			},
		}
	},

}