----------------------------------------------------------
-- practice
----------------------------------------------------------

triggers = {
    [1] = {
        group = "triggers",
        components = {
            transform = {
                position = { x = 512, y = 256},
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
                collider = false
            }
        }
    }
}
