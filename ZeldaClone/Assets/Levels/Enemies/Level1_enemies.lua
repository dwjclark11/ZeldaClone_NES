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
                health_percentage = 1,
                max_hearts = 4
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
    [2] = 
    {
        group = "enemies",
        components = {
            transform = {
                position = { x = 1152, y = 800},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            sprite = {
                asset_id = "simple_enemies",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 32,
                offset = { x = 0, y = 0}
            },
            rigidbody = {
                velocity = { x = 150, y = 150}
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "blade_trap",
                enemy_boss_type = "not_a_boss",
                is_boss = false
            },
            enemy_component = {
                max_move_distance = 320,
                move_dir = "right",
                start_pos = { x = 1152, y = 800 }
            }
        }
    },
    [3] = 
    {
        group = "enemies",
        components = {
            transform = {
                position = { x = 1152, y = 1184},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            sprite = {
                asset_id = "simple_enemies",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 32,
                offset = { x = 0, y = 0}
            },
            rigidbody = {
                velocity = { x = 150, y = 150}
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "blade_trap",
                enemy_boss_type = "not_a_boss",
                is_boss = false
            },
            enemy_component = {
                max_move_distance = 320,
                move_dir = "right",
                start_pos = { x = 1152, y = 1184 }
            }
        }
    },
    [4] = 
    {
        group = "enemies",
        components = {
            transform = {
                position = { x = 1856, y = 1184},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            sprite = {
                asset_id = "simple_enemies",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 32,
                offset = { x = 0, y = 0}
            },
            rigidbody = {
                velocity = { x = 150, y = 150}
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "blade_trap",
                enemy_boss_type = "not_a_boss",
                is_boss = false
            },
            enemy_component = {
                max_move_distance = 192,
                move_dir = "up",
                start_pos = { x = 1856, y = 1184 }
            }
        }
    },
    [5] = 
    {
        group = "enemies",
        components = {
            transform = {
                position = { x = 1856, y = 800},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 16,
                height = 16,
                offset_x = 0,
                offset_y = 0
            },
            sprite = {
                asset_id = "simple_enemies",
                width = 16,
                height = 16,
                z_index = 2,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 32,
                offset = { x = 0, y = 0}
            },
            rigidbody = {
                velocity = { x = 150, y = 150}
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "blade_trap",
                enemy_boss_type = "not_a_boss",
                is_boss = false
            },
            enemy_component = {
                max_move_distance = 192,
                move_dir = "down",
                start_pos = { x = 1856, y = 800 }
            }
        }
    },
}
