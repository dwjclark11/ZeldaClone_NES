-------------------------------------------------------------
--  Moblin Attributes
-------------------------------------------------------------

moblin = {
    components = {
        sprite = {
            asset_id    = "moblin",
            width       = 16,
            height      = 16,
            layer       = 1,
            fixed       = false,
            src_rect_x  = 0,
            src_rect_y  = 0,
            src_offset  = { x = 0, y = 0 }
        },
        box_collider = {
            width       = 12,
            height      = 12,
            offset      = { x = 12, y = 12 }
        },
        health = {
            health_percentage = 2,
            max_hearts = 1
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
        projectile_emitter = {
    
        },
        ai_component = {
                enemy_pos = { x = 0,  y = 0},
                enemy_type = "moblin",
        }
    }
}
