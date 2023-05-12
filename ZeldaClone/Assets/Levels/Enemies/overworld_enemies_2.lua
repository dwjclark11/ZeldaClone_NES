----------------------------------------------------------
-- overworld_enemies_2
----------------------------------------------------------

enemies = {
    [1] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 7582, y = 4183},
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
                health_percentage = 1,
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
                position = { x = 7419, y = 4290},
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
                health_percentage = 1,
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
                position = { x = 7665, y = 4367},
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
                health_percentage = 1,
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
                position = { x = 7863, y = 4451},
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
                health_percentage = 1,
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
                position = { x = 8046, y = 4276},
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
                health_percentage = 1,
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
                position = { x = 8510, y = 4070},
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
                health_percentage = 1,
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
                position = { x = 8378, y = 4093},
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
                health_percentage = 1,
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
                position = { x = 8616, y = 4250},
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
                health_percentage = 1,
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
                position = { x = 9087, y = 4234},
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
                health_percentage = 1,
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
                position = { x = 8395, y = 3495},
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
                health_percentage = 1,
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
                position = { x = 8509, y = 3610},
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
                health_percentage = 1,
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
                position = { x = 9069, y = 3519},
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
                health_percentage = 1,
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
                position = { x = 9432, y = 2877},
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
                health_percentage = 1,
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
                position = { x = 9528, y = 3011},
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
                health_percentage = 1,
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
    [15] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 9878, y = 3032},
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
                health_percentage = 1,
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
    [16] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 9700, y = 2814},
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
                health_percentage = 1,
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
    [17] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 10035, y = 3022},
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
                health_percentage = 1,
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
    [18] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8944, y = 4171},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 12,
                height = 12,
                offset_x = 12,
                offset_y = 12
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
                health_percentage = 2,
                max_hearts = 1
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
                enemy_type = "moblin"
            }
        }
    },
    [19] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8616, y = 4169},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 12,
                height = 12,
                offset_x = 12,
                offset_y = 12
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
                health_percentage = 2,
                max_hearts = 1
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
                enemy_type = "moblin"
            }
        }
    },
    [20] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 9085, y = 4545},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 12,
                height = 12,
                offset_x = 12,
                offset_y = 12
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
                health_percentage = 2,
                max_hearts = 1
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
                enemy_type = "moblin"
            }
        }
    },
    [21] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8381, y = 4570},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 12,
                height = 12,
                offset_x = 12,
                offset_y = 12
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
                health_percentage = 2,
                max_hearts = 1
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
                enemy_type = "moblin"
            }
        }
    },
    [22] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 9111, y = 5207},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 12,
                height = 12,
                offset_x = 12,
                offset_y = 12
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
                health_percentage = 2,
                max_hearts = 1
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
                enemy_type = "moblin"
            }
        }
    },
    [23] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 9101, y = 4958},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 12,
                height = 12,
                offset_x = 12,
                offset_y = 12
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
                health_percentage = 2,
                max_hearts = 1
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
                enemy_type = "moblin"
            }
        }
    },
    [24] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8387, y = 4811},
                scale = { x = 4, y = 4},
                rotation = {0}
            },
            box_collider = {
                width = 12,
                height = 12,
                offset_x = 12,
                offset_y = 12
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
                health_percentage = 2,
                max_hearts = 1
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
                enemy_type = "moblin"
            }
        }
    },
    [25] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8477, y = 4887},
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
                health_percentage = 1,
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
    [26] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8699, y = 4989},
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
                health_percentage = 1,
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
    [27] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8462, y = 5178},
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
                health_percentage = 1,
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
    [28] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8243, y = 3877},
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
                health_percentage = 1,
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
    [29] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 7905, y = 3876},
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
                health_percentage = 1,
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
    [30] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 7808, y = 3744},
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
                health_percentage = 1,
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
    [31] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 7468, y = 3853},
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
                health_percentage = 1,
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
    [32] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 7444, y = 3547},
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
                health_percentage = 1,
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
    [33] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8587, y = 3102},
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
                asset_id = "leever",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 0,
                offset = { x = 0, y = 0}
            },
            health = {
                health_percentage = 4,
                max_hearts = 2
            },
            rigidbody = {
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
                enemy_pos = { x = 0, y = 0},
                enemy_type = "leever"
            }
        }
    },
    [34] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8700, y = 3100},
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
                asset_id = "leever",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 0,
                offset = { x = 0, y = 0}
            },
            health = {
                health_percentage = 4,
                max_hearts = 2
            },
            rigidbody = {
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
                enemy_pos = { x = 0, y = 0},
                enemy_type = "leever"
            }
        }
    },
    [35] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8805, y = 3098},
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
                asset_id = "leever",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 0,
                offset = { x = 0, y = 0}
            },
            health = {
                health_percentage = 4,
                max_hearts = 2
            },
            rigidbody = {
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
                enemy_pos = { x = 0, y = 0},
                enemy_type = "leever"
            }
        }
    },
    [36] = {
        group = "enemies",
        components = {
            transform = {
                position = { x = 8888, y = 3028},
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
                asset_id = "leever",
                width = 16,
                height = 16,
                z_index = 1,
                is_fixed = false,
                src_rect_x = 0,
                src_rect_y = 0,
                offset = { x = 0, y = 0}
            },
            health = {
                health_percentage = 4,
                max_hearts = 2
            },
            rigidbody = {
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
                enemy_pos = { x = 0, y = 0},
                enemy_type = "leever"
            }
        }
    }
}