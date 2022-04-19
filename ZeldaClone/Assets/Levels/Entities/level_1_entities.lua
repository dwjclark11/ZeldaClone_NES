--------------------------------------------------------------------
--   LEVEL 1 - EAGLE - ENTITIES
--------------------------------------------------------------------

level = {
	[1] ={
		group = "trigger",	
		components = {
			transform = {
				position = { x = 512, y = 925},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "items",
				width = 16,
				height = 16,
				layer = 1,
				fixed = false,
				src_rect_x = 96, 
				src_rect_y = 16 
			},
			box_collider = {
				width = 16,
				height = 16,
				offset_x = 0,
				offset_y = 0
			},
			trigger_box = {
				trigger_type = "collect_item", -- Collect Item -- Need to convert to enum 
				transport_offset = { x = 0, y = 0},
				camera_offset = {x = 0, y = 0},
			},
			item = {
				item_type = "default",
				special = "wood_bow"
			}
		}
	},
		-- FIRE LEFT
	[2] ={
		group = "NPC",	
		components = {
			transform = {
				position = { x = 236, y = 2198},
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
				position = { x = 600, y = 2198},
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
				position = { x = 414, y = 2214},
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
	-- Caption
	[5] ={
		group = "caption",	
		components = {
			caption = {
				captions = "Eastmost Penninsula  is the secret.",
				x_pos = 236,
				y_pos = 2150,
				num_frames = 37,
			},
		}
	},
	-- Triforce Piece
	[6] ={
		group = "trigger",	
		components = {
			transform = {
				position = { x = 5597, y = 1690},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "items",
				width = 16,
				height = 16,
				layer = 4,
				fixed = false,
				src_rect_x = 80, 
				src_rect_y = 64 
			}, 
			animation = {
				num_frames = 2,
				frame_rate = 10,
				vertical = false, 
				looped = true, 
				frame_offset = 80
			},
			box_collider = {
				width = 16,
				height = 16,
				offset_x = 0,
				offset_y = 0
			},
			trigger_box = {
				trigger_type = "collect_item", -- Collect Item -- Need to convert to enum 
				transport_offset = { x = 0, y = 0},
				camera_offset = {x = 0, y = 0},
			},
			item = {
				item_type = "default",
				special = "triforce_piece"
			}
		}
	},
}
