----------------------------------------------------------
-- C:\Users\dwjcl\Desktop\ZeldaClone\ZeldaClone\Assets\Levels\enemies.lua
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
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0
      },
      sprite = {
        asset_id = "enemies",
        width = 16,
        height = 16,
        z_index = 0,
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
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
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
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0
      },
      sprite = {
        asset_id = "enemies",
        width = 16,
        height = 16,
        z_index = 0,
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
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
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
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0
      },
      sprite = {
        asset_id = "enemies",
        width = 16,
        height = 16,
        z_index = 0,
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
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
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
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0
      },
      sprite = {
        asset_id = "enemies",
        width = 16,
        height = 16,
        z_index = 0,
        is_fixed = false,
        src_rect_x = 64,
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
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
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
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0
      },
      sprite = {
        asset_id = "enemies",
        width = 16,
        height = 16,
        z_index = 0,
        is_fixed = false,
        src_rect_x = 64,
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
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
      }
    }
  }
}