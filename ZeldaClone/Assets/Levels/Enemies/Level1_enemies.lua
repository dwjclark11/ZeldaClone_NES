enemies = 
{
    [1] = 
    {
        group = "enemies",
        components = {
            transform = {
                position = { x = 4700, y = 1600},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 24,
                height = 24,
                offset_x = 16,
                offset_y = 16
            },
            sprite = {
                asset_id = "aquamentus",
                width = 32,
                height = 32,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 0,
                offset = { x = 0, y = 0}
            },
            health = {
                health_percentage = 2,
                max_hearts = 1
            },
            rigidbody = {
                velocity = { x = 10, y = 0}
            },
            projectile_emitter = {
                velocity = { x = 0, y = 0},
                repeat_frequency = 0,
                projectile_duration = 10000,
                hit_percent_damage = 1,
                is_friendly = false
            },
            animation = {
                num_frames = 4,
                frame_rate = 5,
                vertical = false,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "no_type",
                enemy_boss_type = "aquamentus",
                is_boss = true
            }
        }
    },
}
