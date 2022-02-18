--------------------------------------------------------------------
--   BOMB SECRET 2 ENTITIES -- Money Making Game 1
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
	-- OLD MAN
	[3] ={
		group = "NPC",	-- This is not an enemy
		components = {
			transform = {
				position = { x = 444, y = 216},
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
	-- RUPEE --> CENTER
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
				src_rect_x = 64, 
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
		}
	},
	-- RUPEE --> LEFT
	[5] ={
		group = "trigger",	
		components = {
			transform = {
				position = { x = 330, y = 332},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "hearts",
				width = 16,
				height = 16,
				layer = 1,
				fixed = false,
				src_rect_x = 64, 
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
		}
	},
	-- RUPEE --> RIGHT
	[6] ={
		group = "trigger",	
		components = {
			transform = {
				position = { x = 660, y = 332},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "hearts",
				width = 16,
				height = 16,
				layer = 1,
				fixed = false,
				src_rect_x = 64, 
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
		}
	},
	-- Caption -- - 10
	[7] ={
			group = "caption",	
			components = {
				caption = {
					captions = "-",
					scrollable = false,
					numbers = true,
					x_pos = 660,
					y_pos = 400,
					num_frames = 3,
					ones = 0,
					tens = 1, 
					hundreds = 0
				},
			}
		},
	-- Caption -- OLD MAN
	[8] ={
		group = "caption",	
		components = {
			caption = {
				captions = "Let's Play a Money Making Game",
				x_pos = 150,
				y_pos = 135,
				num_frames = 22,
			},
		}
	},
	-- RUPEE animation
	[9] ={
		--group = "NPC",	
		components = {
			transform = {
				position = { x = 160, y = 360},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "hearts",
				width = 16,
				height = 16,
				layer = 4,
				fixed = false,
				src_rect_x = 48, 
				src_rect_y = 0 
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
	-- Caption -- X
	[10] ={
		group = "caption",	
		components = {
			caption = {
				captions = "X",
				scrollable = false,
				numbers = false,
				x_pos = 212,
				y_pos = 400,
				num_frames = 1,
			},
		}
	},
}