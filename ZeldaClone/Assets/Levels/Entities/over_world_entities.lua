--------------------------------------------------------------------
--   OVERWORLD ENTITIES
--------------------------------------------------------------------

level = {
	-- Fairy near Level 1 
	[1] ={
		group = "fairy",	
		components = {
			transform = {
				position = { x = 9698, y = 2267},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "npcs",
				width = 16,
				height = 16,
				layer = 4,
				fixed = false,
				src_rect_x = 0, 
				src_rect_y = 80 
			}, 
			animation = {
				num_frames = 2,
				frame_rate = 10,
				vertical = false, 
				looped = true, 
				frame_offset = 0
			} 
		}
	},
	-- Fairy near lost woods 
	[2] ={
		group = "fairy",	
		components = {
			transform = {
				position = { x = 3552, y = 2964},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "npcs",
				width = 16,
				height = 16,
				layer = 4,
				fixed = false,
				src_rect_x = 0, 
				src_rect_y = 80 
			}, 
			animation = {
				num_frames = 2,
				frame_rate = 10,
				vertical = false, 
				looped = true, 
				frame_offset = 0
			} 
		}
	},
}
