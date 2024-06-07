----------------------------------------------------------
-- Overworld_enemies.lua
----------------------------------------------------------

enemies = {
  [1] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 500, y = 640},
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
        z_index = 0,
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
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
      }, 
      projectile_emitter = {
        velocity = { x = 200, y = 200}
        repeat_frequency = 0,
        projectile_duration = 6000,
        hit_percent_damage = 1,
        is_friendly = false
      },
      ai_component = {
            enemy_pos = { x = 0,  y = 0},
            enemy_type = "octorok",
      }
    }
  },
  [2] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 800, y = 640},
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
        z_index = 0,
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
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
      }, 
      projectile_emitter = {
        velocity = { x = 200, y = 200}
        repeat_frequency = 0,
        projectile_duration = 6000,
        hit_percent_damage = 1,
        is_friendly = false
      },
      ai_component = {
            enemy_pos = { x = 0,  y = 0},
            enemy_type = "octorok",
      }
    }
  },
  [3] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 960, y = 480},
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
        z_index = 0,
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
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
      }, 
      projectile_emitter = {
        velocity = { x = 200, y = 200}
        repeat_frequency = 0,
        projectile_duration = 6000,
        hit_percent_damage = 1,
        is_friendly = false
      },
      ai_component = {
            enemy_pos = { x = 0,  y = 0},
            enemy_type = "octorok",
      }
    }
  },
  [4] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 672, y = 416},
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
        z_index = 0,
        is_fixed = false,
        src_rect_x = 64,
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
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
      }, 
      projectile_emitter = {
        velocity = { x = 200, y = 200}
        repeat_frequency = 0,
        projectile_duration = 6000,
        hit_percent_damage = 1,
        is_friendly = false
      },
      ai_component = {
            enemy_pos = { x = 0,  y = 0},
            enemy_type = "octorok",
      }
    }
  },
  [5] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 7000, y = 5000},
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
        z_index = 0,
        is_fixed = false,
        src_rect_x = 64,
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
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
      },
      projectile_emitter = {
        velocity = { x = 200, y = 200}
        repeat_frequency = 0,
        projectile_duration = 6000,
        hit_percent_damage = 1,
        is_friendly = false
      },
      ai_component = {
        enemy_pos = { x = 0,  y = 0},
        enemy_type = "octorok",
      }
    }
  },
  [6] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 7700, y = 5100},
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
        z_index = 0,
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
      animation = {
        num_frames = 1,
        frame_rate = 0,
        vertical = true,
        looped = true,
        frame_offset = 0
      }, 
      ai_component = {
            enemy_pos = { x = 0,  y = 0},
            enemy_type = "leever",
      }
    }
  },
  [7] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 7700, y = 5000},
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
        asset_id = "moblin",
        width = 16,
        height = 16,
        z_index = 0,
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
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
      },
      projectile_emitter = {
        velocity = { x = 200, y = 200}
        repeat_frequency = 0,
        projectile_duration = 6000,
        hit_percent_damage = 1,
        is_friendly = false
      },
      ai_component = {
            enemy_pos = { x = 0,  y = 0},
            enemy_type = "moblin",
      },
    }
  }
}