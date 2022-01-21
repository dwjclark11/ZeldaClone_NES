----------------------------------------------------------
-- C:\Development\ZeldaClone\ZeldaClone\Assets\Tilemaps\EnemyTest.lua
----------------------------------------------------------

enemies = {
    [1] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 608, y = 384},
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
                asset_id = "octorok",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 0,
                offset = { x = 0, y = 0}
            },
            health = {
                health_percentage = 9,
                max_hearts = 3
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = ""
            }
        }
    },
    [2] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 576, y = 640},
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
                asset_id = "octorok",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 0,
                offset = { x = 0, y = 0}
            },
            health = {
                health_percentage = 9,
                max_hearts = 3
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = ""
            }
        }
    },
    [3] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 1024, y = 768},
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
                asset_id = "octorok",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 0,
                offset = { x = 0, y = 0}
            },
            health = {
                health_percentage = 9,
                max_hearts = 3
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = ""
            }
        }
    },
    [4] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 1184, y = 512},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 32,
                height = 32,
                offset_x = 0,
                offset_y = 0
            },
            sprite = {
                asset_id = "moblin",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 0,
                offset = { x = 0, y = 0}
            },
            health = {
                health_percentage = 9,
                max_hearts = 3
            },
            rigidbody = {
                velocity = { x = 50, y = 0}
            },
            projectile_emitter = {
                velocity = { x = 0, y = 0},
                repeat_frequency = 0,
                projectile_duration = 10000,
                hit_percent_damage = 1,
                is_friendly = false
            },
            animation = {
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = ""
            }
        }
    },
    [5] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 864, y = 352},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 32,
                height = 32,
                offset_x = 0,
                offset_y = 0
            },
            sprite = {
                asset_id = "moblin",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 0,
                offset = { x = 0, y = 0}
            },
            health = {
                health_percentage = 9,
                max_hearts = 3
            },
            rigidbody = {
                velocity = { x = 50, y = 0}
            },
            projectile_emitter = {
                velocity = { x = 0, y = 0},
                repeat_frequency = 0,
                projectile_duration = 10000,
                hit_percent_damage = 1,
                is_friendly = false
            },
            animation = {
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = ""
            }
        }
    },
    [6] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 672, y = 800},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 32,
                height = 32,
                offset_x = 0,
                offset_y = 0
            },
            sprite = {
                asset_id = "moblin",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 0,
                offset = { x = 0, y = 0}
            },
            health = {
                health_percentage = 9,
                max_hearts = 3
            },
            rigidbody = {
                velocity = { x = 50, y = 0}
            },
            projectile_emitter = {
                velocity = { x = 0, y = 0},
                repeat_frequency = 0,
                projectile_duration = 10000,
                hit_percent_damage = 1,
                is_friendly = false
            },
            animation = {
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = ""
            }
        }
    }
}