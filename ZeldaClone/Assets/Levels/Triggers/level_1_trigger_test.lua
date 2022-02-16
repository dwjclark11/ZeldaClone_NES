----------------------------------------------------------
-- level_1_trigger_test
----------------------------------------------------------

triggers = {
    [1] = {
        group = "secret",
        components = {
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
                collider = false
            }
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
                collider = false
            }
        }
    }
}
