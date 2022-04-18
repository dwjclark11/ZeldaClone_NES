----------------------------------------------------------
-- overworld_enemies_1.lua
----------------------------------------------------------

enemies = {
    [1] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 7589, y = 4232},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [2] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 7756, y = 4370},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [3] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8065, y = 4278},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [4] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 6241, y = 4282},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [5] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 6728, y = 4214},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [6] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 6904, y = 5020},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [7] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8671, y = 4963},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [8] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8666, y = 4278},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [9] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8665, y = 3740},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [10] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 9031, y = 3771},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [11] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 7902, y = 3778},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [12] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 7552, y = 3588},
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [13] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 7184, y = 3831},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    },
    [14] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 7133, y = 3585},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 10,
                height = 10,
                offset_x = 10,
                offset_y = 10
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
                num_frames = 2,
                frame_rate = 10,
                vertical = true,
                looped = true,
                frame_offset = 0
            },
            ai_component = {
                enemy_pos = { x = 0, y = 0},
                enemy_type = "octorok"
            }
        }
    }
}