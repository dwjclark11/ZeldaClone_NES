--------------------------------------------------------------------
--   BOMB SECRET 1 ENTITIES
--------------------------------------------------------------------

level = {
	-- FIRE LEFT
	[1] ={
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
	[2] ={
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
	-- MOBLIN
	[3] ={
		group = "NPC",	-- This is not an enemy
		components = {
			transform = {
				position = { x = 476, y = 216},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "moblin",
				width = 16,
				height = 16,
				layer = 4,
				fixed = false,
				src_rect_x = 0, 
				src_rect_y = 0 
			}
		}
	},
	-- RUPEE
	[4] ={
		group = "trigger",	
		components = {
			transform = {
				position = { x = 480, y = 332},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "hearts",
				width = 16,
				height = 16,
				layer = 1,
				fixed = false,
				src_rect_x = 48, 
				src_rect_y = 0 
			},
			box_collider = {
				width = 16,
				height = 16,
				offset = {x = 0, y = 0}
			},
			trigger_box = {
				trigger_type = "shop_item", 
				transport_offset = { x = 0, y = 0},
				camera_offset = { x = 0, y = 0},
			},
			item = {
				item_type = "bombs",
				special = "none",
				price = 20
			},
			animation = {
				num_frames = 2,
				frame_rate = 10,
				vertical = false, 
				looped = true, 
				frame_offset = 48
			}
		}
	},
	-- Caption -- MOBLIN
	[5] ={
		group = "caption",	
		components = {
			caption = {
				captions = "It's a Secret to     Everyone!",
				x_pos = 250,
				y_pos = 135,
				num_frames = 22,
			},
		}
	},
}