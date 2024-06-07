-------------------------------------------------------------
--  Leever Attributes
-------------------------------------------------------------
leever = {
    components = {
        sprite = {
            asset_id    = "leever",
            width       = 16,
            height      = 16,
            layer       = 1,
            fixed       = false,
            src_rect_x  = 0,
            src_rect_y  = 0,
            src_offset  = { x = 0, y = 0 }
        },
        box_collider = {
            width       = 16,
            height      = 16,
            offset      = { x = 0, y = 0 }
        },
        health = {
            health_percentage = 4,
            max_hearts = 2
        },
        rigid_body = {
            velocity = { x = 50, y = 0}
        },
        animation = {
            num_frames = 2,
            frame_rate = 10,
            vertical = true,
            looped = true,
            frame_offset = 0
        }, 
        ai_component = {
                enemy_pos = { x = 0,  y = 0},
                enemy_type = "leever",
        }
    }
}
