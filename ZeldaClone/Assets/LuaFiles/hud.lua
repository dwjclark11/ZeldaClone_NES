--------------------------------------------------------------------
--   HUD INITIALIZER
--------------------------------------------------------------------

hud = {
	-- rupee
	[1] ={
		tag = "rupee",
		group = "hud",	
		components = {
			transform = {
				position = { x = 300, y = 50},
				scale = { x = 2, y = 2},
				rotation = 45.0
			},
			sprite = {
				asset_id = "items",
				width = 16,
				height = 16,
				layer = 5,
				fixed = true,
				src_rect_x = 32, 
				src_rect_y = 48 
			}
		}
	},
	[2] = {
		tag = "hudHolder1",
		group = "hud",
		components = {
			transform = {
				position = { x = 375, y = 15 }, 
				scale = {x = 6, y = 6},
				rotation = 0.0,
			},
			sprite = {
				asset_id = "hud_holder",
				width = 32,
				height = 32,
				layer = 5,
				fixed = true,
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	}, 
	[3] = {
		tag = "hudHolder2",
		group = "hud",
		components = {
			transform = {
				position = {x = 495, y = 15 }, 
				scale = { x = 6, y = 6},
				rotation = 0.0
			},
			sprite = {
				asset_id = "hud_holder",
				width = 32, 
				height = 32, 
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	}, 
	[4] = {
		tag = "key",
		group = "hud",
		components = {
			transform = {
				position = { x = 300, y = 100 },
				scale = { x = 2, y = 2 },
				rotation = 45.0
			},
			sprite = {
				asset_id = "items",
				width = 16,
				height = 16,
				layer = 5,
				fixed = true,
				src_rect_x = 64,
				src_rect_y = 32
			}
		}
	},
	[5] = {
		tag = "bomb",
		group = "hud",
		components = {
			transform = {
				position = { x = 300, y = 125 },
				scale = { x = 2, y = 3 },
				rotation = 45.0
			},
			sprite = {
				asset_id = "items",
				width = 16,
				height = 16,
				layer = 5,
				fixed = true, 
				src_rect_x = 64,
				src_rect_y = 112
			}
		}
	}, 
	[6] = {
		tag = "hudItem",
		group = "hud",
		components = {
			transform = {
				position = { x = 426, y = 60},
				scale = { x = 6, y = 6 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "items",
				width = 16,
				height = 16,
				layer = 5,
				fixed = true, 
				src_rect_x = 48,
				src_rect_y = 16
			}
		}
	},
	[7] = {
		tag = "hudSword",
		group = "hud",
		components = {
			transform = {
				position = { x = 550, y = 60},
				scale = { x = 6, y = 6 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "items",
				width = 16,
				height = 16,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	}, 
	[8] = {
		tag = "world_location",
		group = "hud",
		components = {
			transform = {
				position = { x = 10, y = 40},
				scale = { x = 2, y = 2},
				rotation = 0.0
			},
			sprite = {
				asset_id = "world_location",
				width = 128,
				height = 64,
				layer = 5,
				fixed = true,
				src_rect_x = 0,
				src_rect_y = 0
			} 
		}
	},
	[9] = {
		tag = "locator",
		group = "hud",
		components = {
			transform = {
				position = { x = 10, y = 40},
				scale = { x = 3, y = 3},
				rotation = 0.0
			},
			sprite = {
				asset_id = "locator",
				width = 4,
				height = 4,
				layer = 6,
				fixed = true,
				src_rect_x = 0,
				src_rect_y = 0
			} 
		}
	},
	[10] = {
		tag = "heart1",
		group = "hud",
		components = {
			transform = {
				position = { x = 660, y = 95 },
				scale = { x = 4, y = 4 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "hud_hearts",
				width = 16,
				height = 16,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	},
	[11] = {
		tag = "heart2",
		group = "hud",
		components = {
			transform = {
				position = { x = 692, y = 95 },
				scale = { x = 4, y = 4 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "hud_hearts",
				width = 16,
				height = 16,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	},
	[12] = {
		tag = "heart3",
		group = "hud",
		components = {
			transform = {
				position = { x = 724, y = 95 },
				scale = { x = 4, y = 4 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "hud_hearts",
				width = 16,
				height = 16,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	},
	[13] = {
		tag = "rupee_hundreds",
		group = "hud",
		components = {
			transform = {
				position = { x = 332, y = 55 },
				scale = { x = 3, y = 3 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "numbers",
				width = 8,
				height = 8,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	},
		[14] = {
		tag = "rupee_tens",
		group = "hud",
		components = {
			transform = {
				position = { x = 356, y = 55 },
				scale = { x = 3, y = 3 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "numbers",
				width = 8,
				height = 8,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	},
	[15] = {
		tag = "rupee_ones",
		group = "hud",
		components = {
			transform = {
				position = { x = 380, y = 55 },
				scale = { x = 3, y = 3 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "numbers",
				width = 8,
				height = 8,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	},
	[16] = {
		tag = "keys_tens",
		group = "hud",
		components = {
			transform = {
				position = { x = 340, y = 95 },
				scale = { x = 3, y = 3 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "numbers",
				width = 8,
				height = 8,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	},
	[17] = {
		tag = "keys_ones",
		group = "hud",
		components = {
			transform = {
				position = { x = 364, y = 95 },
				scale = { x = 3, y = 3 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "numbers",
				width = 8,
				height = 8,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	},
	[18] = {
		tag = "bombs_tens",
		group = "hud",
		components = {
			transform = {
				position = { x = 340, y = 135 },
				scale = { x = 3, y = 3 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "numbers",
				width = 8,
				height = 8,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	},
	[19] = {
		tag = "bombs_ones",
		group = "hud",
		components = {
			transform = {
				position = { x = 364, y = 135 },
				scale = { x = 3, y = 3 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "numbers",
				width = 8,
				height = 8,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	},
	[20] = {
		tag = "lifeText",
		group = "hud",
		components = {
			transform = {
				position = { x = 700, y = 60 },
				scale = { x = 2, y = 2 },
				rotation = 0.0
			},
			sprite = {
				asset_id = "hud_words",
				width = 112,
				height = 16,
				layer = 5,
				fixed = true, 
				src_rect_x = 0,
				src_rect_y = 0
			}
		}
	},
 }
