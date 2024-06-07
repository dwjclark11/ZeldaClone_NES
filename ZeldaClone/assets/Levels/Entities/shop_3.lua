--------------------------------------------------------------------
--   SHOP 1 ENTITIES
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
	-- MERCHANT
	[3] ={
		group = "NPC",	
		components = {
			transform = {
				position = { x = 448, y = 200},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "npcs",
				width = 32,
				height = 32,
				layer = 4,
				fixed = false,
				src_rect_x = 64, 
				src_rect_y = 0 
			}
		}
	},
	-- LARGE SHIELD
	[4] ={
		group = "trigger",	
		components = {
			transform = {
				position = { x = 300, y = 332},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "items",
				width = 16,
				height = 16,
				layer = 1,
				fixed = false,
				src_rect_x = 80, 
				src_rect_y = 48 
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
				item_type = "default",
				special = "red_candle",
				price = 160
			},
		}
	},
	-- BOMBS
	[5] ={
		group = "trigger",	
		components = {
			transform = {
				position = { x = 480, y = 332},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "items",
				width = 16,
				height = 16,
				layer = 1,
				fixed = false,
				src_rect_x = 64, 
				src_rect_y = 112 
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
				price = 100
			},
		}
	},
	-- ARROWS
	[6] = {
		group = "trigger",	
		components = {
			transform = {
				position = { x = 660, y = 332},
				scale = { x = 4, y = 4},
				rotation = 0.0
			},
			sprite = {
				asset_id = "items",
				width = 16,
				height = 16,
				layer = 1,
				fixed = false,
				src_rect_x = 0, 
				src_rect_y = 64 
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
				item_type = "default",
				special = "arrows",
				price = 60
			},
		}
	},
	-- RUPEE animation
	[7] ={
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
	-- Caption -- MERCHANT
	[8] ={
		group = "caption",	
		components = {
			caption = {
				captions = "Buy Somethin' will ya!",
				x_pos = 200,
				y_pos = 135,
				num_frames = 21,
			},
		}
	},
	-- Caption -- SHIELD PRICE
	[9] ={
		group = "caption",	
		components = {
			caption = {
				captions = "no_caption",
				scrollable = false,
				numbers = true,
				x_pos = 284,
				y_pos = 400,
				num_frames = 3,
				ones = 0,
				tens = 3, 
				hundreds = 1
			},
		}
	},
	-- Caption -- BOMB PRICE
	[10] ={
		group = "caption",	
		components = {
			caption = {
				captions = "no_caption",
				scrollable = false,
				numbers = true,
				x_pos = 480,
				y_pos = 400,
				num_frames = 3,
				ones = 0,
				tens = 2, 
				hundreds = 0
			},
		}
	},
	-- Caption -- ARROW PRICE
	[11] ={
		group = "caption",	
		components = {
			caption = {
				captions = "no_caption",
				scrollable = false,
				numbers = true,
				x_pos = 660,
				y_pos = 400,
				num_frames = 3,
				ones = 0,
				tens = 8, 
				hundreds = 0
			},
		}
	},
	-- Caption -- X
	[12] ={
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