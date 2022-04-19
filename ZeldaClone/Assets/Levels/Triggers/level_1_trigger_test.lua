----------------------------------------------------------
-- level_1_trigger_test
----------------------------------------------------------
triggers = 
{
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
            scene_change = {
                level_music = "stop",
                asset_file = "no_file",
                enemy_file = "no_file",
                collider_file = "no_file",
                tilemap_name = "no_file",
                tilemap_image = "no_file",
                entity_file = "no_file",
                image_width = 0,
                image_height = 0,
                trigger_file = "no_file",
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
	[2] = {
        group = "triggers",
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
            scene_change = {
                level_music = "stop",
                asset_file = "no_file",
                enemy_file = "no_file",
                collider_file = "no_file",
                tilemap_name = "no_file",
                tilemap_image = "no_file",
                entity_file = "no_file",
                image_width = 0,
                image_height = 0,
                trigger_file = "no_file",
            },
        }
    },
    [3] = {
        group = "triggers",
        components = {
            transform = {
                position = { x = 187, y = 677},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 8,
                offset_x = 0,
                offset_y = 0
            },
            trigger_box = {
                trigger_type = "transport",
                transport_offset = { x = 1410, y = 960},
                camera_offset = { x = 1024, y = 384},
                collider = false
            },
            scene_change = {
                level_music = "stop",
                asset_file = "no_file",
                enemy_file = "no_file",
                collider_file = "no_file",
                tilemap_name = "no_file",
                tilemap_image = "no_file",
                entity_file = "no_file",
                image_width = 0,
                image_height = 0,
                trigger_file = "no_file",
            },
        }
    },
	-- Eagle Level Entrance/Exit
	[4] = {
		group = "trigger",
		components = {
			transform = {
				position = { x = 2532, y = 4632},
				scale    = { x = 4, y = 4 },
				rotation = 0
			},
			box_collider = {
				width 	 = 16,
				height 	 = 16,
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
				enemy_file			= "overworld_enemies",
				collider_file		= "overworld_colliders_1", 
				tilemap_name 		= "no_file",
				tilemap_image   	= "map",
				entity_file			= "no_file",
				trigger_file		= "overworld_triggers",
				image_width			= 4096,
				image_height		= 1344
            },
		}
	},
    -- First Locked Door 
    [5] = {
        group = "triggers",
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
    },
    -- Trap door - 6 bats
    [6] = {
        group = "triggers",
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
    },
    -- Trap door - To old man
    [7] = {
        tag = "td_eagle_old_man",
        group = "triggers",
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
    },
    -- Trap door - Boss Door to triforce
    [8] = {
        tag = "level1Door",
        group = "triggers",
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
    },
    -- Boss Locked Door 
    [9] = {
        group = "triggers",
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
    },
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
    },
}
