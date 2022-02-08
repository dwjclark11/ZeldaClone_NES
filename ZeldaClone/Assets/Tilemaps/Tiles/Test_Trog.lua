----------------------------------------------------------
-- Test_Trog
----------------------------------------------------------

triggers = {
    [1] = {
        group = "secret",
        components = {
            transform = {
                position = { x = 896, y = 640},
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
                trigger_type = "secret_area",
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
                new_trigger = "secret_area",
                new_sprite_id = "",
                sprite_width = 16,
                sprite_height = 16,
                sprite_src_x = 0,
                sprite_src_y = 0
            }
        }
    },
    [2] = {
        group = "secret",
        components = {
            transform = {
                position = { x = 832, y = 384},
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
                trigger_type = "secret_area",
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
                new_trigger = "secret_area",
                new_sprite_id = "",
                sprite_width = 16,
                sprite_height = 16,
                sprite_src_x = 0,
                sprite_src_y = 0
            }
        }
    }
}
