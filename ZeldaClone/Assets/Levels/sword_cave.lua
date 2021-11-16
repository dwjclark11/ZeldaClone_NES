--------------------------------------------------------------------
--   SWORD CAVE ENTITIES
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
				assetID = "npcs",
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
				assetID = "npcs",
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
	[3] ={
		group = "NPC",	
		components = {
			transform = {
				position = { x = 464, y = 200},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				assetID = "npcs",
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
	[4] ={
		group = "trigger",	
		components = {
			transform = {
				position = { x = 464, y = 332},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				assetID = "Sword",
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
				offset = {x = 32, y = 32}
			},
			trigger = {
				trigger_type = 5, -- Collect Item -- Need to convert to enum 
				offset = {0, 0},
				camera_offset = {0, 0},
			},
			item = {
				item_type = "default",
				special = "wood_sword"
			}
		}
	},
}