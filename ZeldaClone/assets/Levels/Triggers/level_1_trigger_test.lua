----------------------------------------------------------
-- level_1_trigger_test
----------------------------------------------------------
triggers = 
{
    -- Block inn front of the stairs to the bow
    [1] = 
    {
        group = "secret",
        components = 
        {
            transform = {
                position = { x = 1402, y = 992},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "push_rocks",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true
            },
            secret = {
                location_id = "none",
                new_trigger = "no_trigger",
                new_sprite_id = "",
                sprite_width = 0,
                sprite_height = 0,
                sprite_src_x = 0,
                sprite_src_y = 0
            },
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 64,
                src_rect_y = 32,
                offset = {x = 0,y = 0}
            }
        }
    },
    -- Transport - Stairs to the Secret Bow Room 
	[2] = {
        group = "trigger",
        components = {
            transform = {
                position = { x = 1533, y = 993},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "transport",
                transport_offset = { x = 187, y = 728},
                camera_offset = { x = 0, y = 384},
                collider = false
            }, 
        }
    },
    -- Transport - Takes you back to the stairs to the secret room
    [3] = {
        group = "trigger",
        components = {
            transform = {
                position = { x = 187, y = 732},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = -24
            },
            trigger_box = {
                trigger_type = "transport",
                transport_offset = { x = 1410, y = 960},
                camera_offset = { x = 1024, y = 384},
                collider = false
            },
        }
    },
	-- Eagle Level Entrance/Exit
	[4] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 2532, y = 4648},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 32,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "scene_change",
				transport_offset 	= { x = 7616, y = 2344 },
				camera_offset 		= { x = 7168, y = 1728 },
			},
            scene_change = {
				level_music			= "Overworld",
				asset_file			= "game_state_assets",
				enemy_file			= "overworld_enemies_2",
				collider_file		= "overworld_colliders_1", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "map",
				entity_file			= "over_world_entities",
				trigger_file		= "overworld_triggers",
				image_width			= 4096,
				image_height		= 1344
            },
		}
	},--]]
    -- [[ First Locked Door 
    [5] = {
        group = "trigger",
        components = {
            transform = {
                position = { x = 2532, y = 4096},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "locked_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 96,
                src_rect_y = 16,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[ Trap door - 6 bats
    [6] = {
        group = "trigger",
        components = {
            transform = {
                position = { x = 1916, y = 3008},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "trap_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 48,
                src_rect_y = 32,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[ Trap door - To old man
    [7] = {
        tag = "td_eagle_old_man",
        group = "trigger",
        components = {
            transform = {
                position = { x = 1084, y = 2336},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "trap_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 48,
                src_rect_y = 32,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[ Trap door - Boss Door to triforce
    [8] = {
        tag = "level1Door",
        group = "trigger",
        components = {
            transform = {
                position = { x = 4988, y = 1664},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "trap_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 48,
                src_rect_y = 32,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[ Boss Locked Door 
    [9] = {
        group = "trigger",
        components = {
            transform = {
                position = { x = 4572, y = 2080},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "locked_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 96,
                src_rect_y = 16,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[
    [10] = 
    {
        group = "secret",
        components = 
        {
            transform = {
                position = { x = 1468, y = 2336},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "push_rocks",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true,
                entity_removed_tag = "td_eagle_old_man",
            },
            secret = {
                location_id = "none",
                new_trigger = "no_trigger",
                new_sprite_id = "",
                sprite_width = 0,
                sprite_height = 0,
                sprite_src_x = 0,
                sprite_src_y = 0
            },
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 64,
                src_rect_y = 32,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[ Bomb Secret - First Side Bomb Wall 
	[11] = {
        tag = "bomb_eagle_1",
		group = "secret",
		components = {
			transform = {
				position = { x = 2524, y = 2752},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "bomb_secret",
				transport_offset 	= { x = 0, y = 0 },
				camera_offset 		= { x = 0, y = 0 },
				collider			= true,
                entity_removed_tag = "bomb_eagle_2",
			},
			secret = {
				location_id			= "none",
				new_trigger			= "no_trigger",
				new_sprite_id		= "Dungeon_Tiles",
				sprite_width		= 16,
				sprite_height		= 16,
				sprite_src_x		= 0,
				sprite_src_y		= 80
			},
		}
	},	--]]
    -- [[ Bomb Secret - Second Side Bomb Wall 
	[12] = {
        tag = "bomb_eagle_2",
		group = "secret",
		components = {
			transform = {
				position = { x = 2524, y = 2592},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
				offset 	 = { x = 0, y = 0 }
			},
			trigger_box  = { 
				trigger_type		= "bomb_secret",
				transport_offset 	= { x = 0, y = 0 },
				camera_offset 		= { x = 0, y = 0 },
				collider			= true,
                entity_removed_tag = "bomb_eagle_1",
			},
			secret = {
				location_id			= "none",
				new_trigger			= "no_trigger",
				new_sprite_id		= "Dungeon_Tiles",
				sprite_width		= 16,
				sprite_height		= 16,
				sprite_src_x		= 16,
				sprite_src_y		= 80
			},
		}
	},--]]
    -- [[ Locked Door - 3
    [13] = {
        tag = "door_eagle_3",
        group = "trigger",
        components = {
            transform = {
                position = { x = 1500, y = 2752},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "locked_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true,
                entity_removed_tag = "door_eagle_4",
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 96,
                src_rect_y = 16,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[ Locked Door - 4
    [14] = {
        tag = "door_eagle_4",
        group = "trigger",
        components = {
            transform = {
                position = { x = 1500, y = 2592},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "locked_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true,
                entity_removed_tag = "door_eagle_3",
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 112,
                src_rect_y = 16,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[ Locked Door - 5
    [15] = {
        tag = "door_eagle_5",
        group = "trigger",
        components = {
            transform = {
                position = { x = 2940, y = 2336},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "locked_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true,
                entity_removed_tag = "door_eagle_6",
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 16,
                src_rect_y = 32,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[ Locked Door - 6
    [16] = {
        tag = "door_eagle_6",
        group = "trigger",
        components = {
            transform = {
                position = { x = 3132, y = 2336},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "locked_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true,
                entity_removed_tag = "door_eagle_5",
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 32,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[Locked Door - 7
    [17] = {
        tag = "door_eagle_7",
        group = "trigger",
        components = {
            transform = {
                position = { x = 2524, y = 1408},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "locked_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true,
                entity_removed_tag = "door_eagle_8",
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 96,
                src_rect_y = 16,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[Locked Door - 8
    [18] = {
        tag = "door_eagle_8",
        group = "trigger",
        components = {
            transform = {
                position = { x = 2524, y = 1248},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "locked_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true,
                entity_removed_tag = "door_eagle_7",
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 112,
                src_rect_y = 16,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[Locked Door - 9
    [19] = {
        tag = "door_eagle_9",
        group = "trigger",
        components = {
            transform = {
                position = { x = 2108, y = 992},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "locked_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true,
                entity_removed_tag = "door_eagle_10",
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 32,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
    -- [[Locked Door - 10
    [20] = {
        tag = "door_eagle_10",
        group = "trigger",
        components = {
            transform = {
                position = { x = 1916, y = 992},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "locked_door",
                transport_offset = { x = 0, y = 0},
                camera_offset = { x = 0, y = 0},
                collider = true,
                entity_removed_tag = "door_eagle_9",
            }, 
            sprite = {
                asset_id = "Dungeon_Tiles",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 16,
                src_rect_y = 32,
                offset = {x = 0,y = 0}
            }
        }
    },--]]
}
