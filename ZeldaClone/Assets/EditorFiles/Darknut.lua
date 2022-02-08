-------------------------------------------------------------
--  Red Darknut Attributes
-------------------------------------------------------------

moblin = {
    components = {
        sprite = {
            asset_id    = "darknut",
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
            health_percentage = 9,
            max_hearts = 3
        },
        rigid_body = {
            velocity = { x = 75, y = 0}
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
                enemy_type = "darknut",
        }
    }
}