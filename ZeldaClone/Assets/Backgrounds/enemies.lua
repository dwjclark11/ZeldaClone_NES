----------------------------------------------------------
-- C:\Users\dwjcl\Desktop\ZeldaClone\ZeldaClone\Assets\Backgrounds\enemies.lua
----------------------------------------------------------

enemies = {
  [1] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 576, y = 288},
        scale = { x = 4, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0}
    },
    sprite = {
      asset_id = "sprites_enemies",
      width = 16,
      height = 16,
      z_index = 0,
      is_fixed = false,
      src_rect_x = 0,
      src_rect_y = 0,
      offset = { x = 0, y = 0}
    },
    animation = {
      num_frames = 4,
      frame_speed_rate = 20,
      vertical = false,
      looped = false,
      frame_offset = 0
    }
  },
  [2] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 736, y = 352},
        scale = { x = 4, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0}
    },
    sprite = {
      asset_id = "sprites_enemies",
      width = 16,
      height = 16,
      z_index = 0,
      is_fixed = false,
      src_rect_x = 48,
      src_rect_y = 32,
      offset = { x = 0, y = 0}
    },
    animation = {
      num_frames = 4,
      frame_speed_rate = 20,
      vertical = false,
      looped = false,
      frame_offset = 0
    }
  },
  [3] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 576, y = 448},
        scale = { x = 4, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0}
    },
    sprite = {
      asset_id = "sprites_enemies",
      width = 16,
      height = 16,
      z_index = 0,
      is_fixed = false,
      src_rect_x = 0,
      src_rect_y = 16,
      offset = { x = 0, y = 0}
    },
    animation = {
      num_frames = 4,
      frame_speed_rate = 20,
      vertical = false,
      looped = false,
      frame_offset = 0
    }
  },
  [4] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 832, y = 192},
        scale = { x = 4, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0}
    },
    sprite = {
      asset_id = "sprites_enemies",
      width = 16,
      height = 16,
      z_index = 0,
      is_fixed = false,
      src_rect_x = 0,
      src_rect_y = 16,
      offset = { x = 0, y = 0}
    },
    animation = {
      num_frames = 4,
      frame_speed_rate = 20,
      vertical = false,
      looped = false,
      frame_offset = 0
    }
  },
  [5] = {
    group = "enemies",
    components = {
      transform = {
        position = { x = 864, y = 288},
        scale = { x = 4, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0}
    },
    sprite = {
      asset_id = "sprites_enemies",
      width = 16,
      height = 16,
      z_index = 0,
      is_fixed = false,
      src_rect_x = 112,
      src_rect_y = 16,
      offset = { x = 0, y = 0}
    },
    animation = {
      num_frames = 4,
      frame_speed_rate = 20,
      vertical = false,
      looped = false,
      frame_offset = 0
    }
  }
}