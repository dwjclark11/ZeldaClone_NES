-------------------------------------------------------------
--  Red Octorok Attributes
-------------------------------------------------------------

octorok_red = {
    components = {
        sprite = {
            asset_id    = "octorok",
            width       = 16,
            height      = 16,
            layer       = 1,
            fixed       = false,
            src_rect_x  = 0,
            src_rect_y  = 0,
            src_offset  = { x = 0, y = 0 }
        },
        box_collider = {
            width       = 10,
            height      = 10,
            offset      = { x = 10, y = 10 }
        },
        health = {
            health_percentage = 1,
            max_hearts = 1
        },
        rigid_body = {
            velocity = { x = 10, y = 0}
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
                enemy_type = "octorok",
        }
    }
}