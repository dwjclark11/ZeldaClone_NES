--------------------------------------------------------------
-- Item Attributes for projectiles
--------------------------------------------------------------

items = {
	[1] = {
		name 				= "bow",
		group 				= "projectile",
		asset_id 			= "items",
		width 				= 16,
		height 				= 16, 
		src_rect_x 			= 0,
		src_rect_y 			= 64,
		num_frames 			= 4,
		scale				= { x = 4, y = 4 },
		trans_offset_up 	= { x = 30, y = 0 }, 
		trans_offset_down 	= { x = 30, y = 0 }, 
		trans_offset_right 	= { x = -30, y = 25 }, 
		trans_offset_left 	= { x = 70, y = 25 }, 
		box_size_up		 	= { x = 10, y = 40 }, 
		box_size_down	 	= { x = 10, y = 40 }, 
		box_size_left		= { x = 40, y = 10 }, 
		box_size_right		= { x = 60, y = 10 }, 
		up_offset 			= { x = 26, y = 0 }, 
		down_offset 		= { x = 26, y = 20 }, 
		right_offset 		= { x = 0, y = 30 }, 
		left_offset 		= { x = 0, y = 30 }, 
		duration			= 3000,
		animation			= false,
		vertical			= false
	},
	[2] = {
		name 				= "boomerang",
		group 				= "boomerang",
		asset_id 			= "items",
		width 				= 16,
		height 				= 16, 
		src_rect_x 			= 0,
		src_rect_y 			= 112,
		num_frames 			= 4,
		scale				= { x = 4, y = 4 },
		trans_offset_up 	= { x = 32, y = 64 }, 
		trans_offset_down 	= { x = 32, y = 0 }, 
		trans_offset_right 	= { x = 0, y = 32 }, 
		trans_offset_left 	= { x = 50, y = 32 }, 
		box_size_up		 	= { x = 32, y = 32 }, 
		box_size_down	 	= { x = 32, y = 32 }, 
		box_size_left		= { x = 32, y = 32 }, 
		box_size_right		= { x = 32, y = 32 }, 
		up_offset 			= { x = 14, y = 12 }, 
		down_offset 		= { x = 18, y = 18 }, 
		right_offset 		= { x = 20, y = 18 }, 
		left_offset 		= { x = 18, y = 18 }, 
		duration			= 3000,
		animation			= true,
		vertical			= false
	}, 
	[3] = {
		name 				= "candle",
		group 				= "fire",
		asset_id 			= "items",
		width 				= 16,
		height 				= 16, 
		src_rect_x 			= 0,
		src_rect_y 			= 96,
		num_frames 			= 2,
		scale				= { x = 4, y = 4 },
		trans_offset_up 	= { x = 32  , y = 96 }, 
		trans_offset_down 	= { x = 32, y = 0 }, 
		trans_offset_right 	= { x = 0, y = 32 }, 
		trans_offset_left 	= { x = 64, y = 32 }, 
		box_size_up		 	= { x = 64, y = 64 }, 
		box_size_down	 	= { x = 64, y = 64 }, 
		box_size_left		= { x = 64, y = 64 }, 
		box_size_right		= { x = 64, y = 64 }, 
		up_offset 			= { x = 0, y = 0 }, 
		down_offset 		= { x = 0, y = 0 }, 
		right_offset 		= { x = 0, y = 0 }, 
		left_offset 		= { x = 0, y = 0 }, 
		duration			= 500,
		animation			= true,
		vertical			= false
	}, 
	[4] = {
		name 				= "magic_rod",
		group 				= "projectile",
		asset_id 			= "items",
		width 				= 16,
		height 				= 16, 
		src_rect_x 			= 64,
		src_rect_y 			= 96,
		num_frames 			= 4,
		scale				= { x = 4, y = 4 },
		trans_offset_up 	= { x = 32, y = 64 }, 
		trans_offset_down 	= { x = 32, y = 32 }, 
		trans_offset_right 	= { x = 32, y = 32 }, 
		trans_offset_left 	= { x = 32, y = 32 }, 
		box_size_up		 	= { x = 64, y = 64 }, 
		box_size_down	 	= { x = 64, y = 64 }, 
		box_size_left		= { x = 64, y = 64 }, 
		box_size_right		= { x = 64, y = 64 }, 
		up_offset 			= { x = 0, y = 0 }, 
		down_offset 		= { x = 0, y = 0 }, 
		right_offset 		= { x = 0, y = 0 }, 
		left_offset 		= { x = 0, y = 0 }, 
		duration			= 2000,
		animation			= false,
		vertical			= false
	}, 
	[5] = {
		name 				= "bomb",
		group 				= "bomber",
		asset_id 			= "items",
		width 				= 16,
		height 				= 16, 
		src_rect_x 			= 64,
		src_rect_y 			= 112,
		num_frames 			= 1,
		scale				= { x = 4, y = 4 },
		trans_offset_up 	= { x = 32, y = 40 }, 
		trans_offset_down 	= { x = 32, y = 0 }, 
		trans_offset_right 	= { x = 0, y = 16 }, 
		trans_offset_left 	= { x = 32, y = 16 }, 
		box_size_up		 	= { x = 16, y = 16 }, 
		box_size_down	 	= { x = 16, y = 16 }, 
		box_size_left		= { x = 16, y = 16 }, 
		box_size_right		= { x = 16, y = 16 }, 
		up_offset 			= { x = 0, y = 0 }, 
		down_offset 		= { x = 0, y = 0 }, 
		right_offset 		= { x = 0, y = 0 }, 
		left_offset 		= { x = 0, y = 0 }, 
		duration			= 3000,
		animation			= false,
		vertical			= false
	}, 
	[6] = {
		name 				= "beam",
		group 				= "projectile",
		asset_id 			= "Sword",
		width 				= 32,
		height 				= 32, 
		src_rect_x 			= 0,
		src_rect_y 			= 0,
		num_frames 			= 4,
		scale				= { x = 4, y = 4 },
		trans_offset_up 	= { x = 0, y = 45 }, 
		trans_offset_down 	= { x = 0, y = 25 }, 
		trans_offset_right 	= { x = 15, y = 20 }, 
		trans_offset_left 	= { x = 65, y = 20 }, 
		box_size_up		 	= { x = 10, y = 40 }, 
		box_size_down	 	= { x = 10, y = 40 }, 
		box_size_left		= { x = 40, y = 10 }, 
		box_size_right		= { x = 40, y = 10 }, 
		up_offset 			= { x = 58, y = 30 }, 
		down_offset 		= { x = 58, y = 45 }, 
		right_offset 		= { x = 45, y = 58 }, 
		left_offset 		= { x = 30, y = 58 }, 
		duration			= 1000,
		animation			= true,
		vertical			= true
	}, 
}