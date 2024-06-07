----------------------------------------------------------
-- Menu Load UI Lua 
----------------------------------------------------------
menu = 
{
	[1] = -- Heart Selector
	{
		tag = "selector", 
		components = 
		{
			transform = 
			{
				position = {x = 260, y = 200 },
				scale = {x = 4, y = 4 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "hearts",
				width = 16,
				height = 16, 
				z_index = 0,
				fixed = false
			}, 
		}
	},
	[2] = -- Menu Box
	{
		components = 
		{
			transform = 
			{
				position = {x = 80, y = 50 },
				scale = {x = 6, y = 6 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "menu_box",
				width = 160,
				height = 160, 
				z_index = 0,
				fixed = false
			}, 
		}
	},
	[3] = -- Life 
	{
		components = 
		{
			transform = 
			{
				position = {x = 645, y = 150 },
				scale = {x = 2, y = 2 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "main_menu_gui",
				width = 64,
				height = 16, 
				z_index = 0,
				fixed = true,
				src_rect_x = 0,
				src_rect_y = 48
			}, 
		}
	},
	[4] = -- Name 
	{
		components = 
		{
			transform = 
			{
				position = {x = 355, y = 150 },
				scale = {x = 2, y = 2 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "main_menu_gui",
				width = 64,
				height = 16, 
				z_index = 0,
				fixed = true,
				src_rect_x = 0,
				src_rect_y = 32
			}, 
		}
	},
	[5] = -- Select
	{
		components = 
		{
			transform = 
			{
				position = {x = 320, y = 80 },
				scale = {x = 2.5, y = 2.5 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "main_menu_gui",
				width = 160,
				height = 16, 
				z_index = 0,
				fixed = true,
				src_rect_x = 0,
				src_rect_y = 0
			}, 
		}
	},
	[6] = -- Elimination Mode
	{
		components = 
		{
			transform = 
			{
				position = {x = 320, y = 600 },
				scale = {x = 1.5, y = 1.5 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "main_menu_gui",
				width = 256,
				height = 16, 
				z_index = 0,
				fixed = true,
				src_rect_x = 0,
				src_rect_y = 64
			}, 
		}
	},
	[7] = -- Register Name
	{
		components = 
		{
			transform = 
			{
				position = {x = 320, y = 500 },
				scale = {x = 1.5, y = 1.5 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "main_menu_gui",
				width = 288,
				height = 16, 
				z_index = 0,
				fixed = false,
				src_rect_x = 0,
				src_rect_y = 16
			}, 
		}
	},
	[8] = -- TileMapEditor -- This is only selectable in DEBUG MODE!
	{
		components = 
		{
			transform = 
			{
				position = {x = 320, y = 800 },
				scale = {x = 1.5, y = 1.5 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "main_menu_gui",
				width = 224,
				height = 16, 
				z_index = 0,
				fixed = false,
				src_rect_x = 0,
				src_rect_y = 80
			}, 
			menu = 
			{
				slot_pos = 0
			}
		}
	},
	[9] = -- First Player Slot
	{
		tag = "first_menu_slot",
		components = 
		{
			transform = 
			{
				position = {x = 290, y = 200 },
				scale = {x = 4, y = 4 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "Link",
				width = 32,
				height = 32, 
				z_index = 0,
				fixed = true,
				src_rect_x = 0,
				src_rect_y = 0
			}, 
			menu = 
			{
				slot_pos = 1
			}
		}
	},
	[10] = -- Second Player slot
	{
		tag = "second_menu_slot",
		components = 
		{
			transform = 
			{
				position = {x = 290, y = 300 },
				scale = {x = 4, y = 4 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "Link",
				width = 32,
				height = 32, 
				z_index = 0,
				fixed = true,
				src_rect_x = 0,
				src_rect_y = 0
			}, 
			menu = 
			{
				slot_pos = 2
			}
		}
	},
	[11] = -- third player slot
	{
		tag = "third_menu_slot",
		components = 
		{
			transform = 
			{
				position = {x = 290, y = 400 },
				scale = {x = 4, y = 4 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "Link",
				width = 32,
				height = 32, 
				z_index = 0,
				fixed = true,
				src_rect_x = 0,
				src_rect_y = 0
			}, 
			menu = 
			{
				slot_pos = 3
			}
		}
	},
	[12] = -- Settings
	{
		components = 
		{
			transform = 
			{
				position = {x = 320, y = 700 },
				scale = {x = 1.5, y = 1.5 },
				rotation = 0
			},
			sprite = 
			{
				asset_id = "main_menu_gui",
				width = 224,
				height = 16, 
				z_index = 0,
				fixed = false,
				src_rect_x = 0,
				src_rect_y = 96
			}, 
			menu = 
			{
				slot_pos = 0
			}
		}
	},
}