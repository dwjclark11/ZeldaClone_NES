----------------------------------------------------------
-- New Player Create 
----------------------------------------------------------
-- This creates a player entity, sword and shield
player = 
{
  [1] = 
  {
    tag = "player",
    components = 
    {
      transform = {
        position = { x = 7600, y = 5152},
        scale = { x = 4, y = 4},
        rotation = {0}
      },
      box_collider = {
        width = 8,
        height = 10,
        offset_x = 45,
        offset_y = 45
      },
      sprite = {
        asset_id = "Link",
        width = 32,
        height = 32,
        z_index = 3,
        is_fixed = false,
        src_rect_x = 0,
        src_rect_y = 0,
        offset = { x = 0, y = 0}
      },
      health = {
        health_percentage = 6,
        max_hearts = 3
      },
      rigidbody = {
        velocity = { x = 0, y = 0}
      },
      animation = {
        num_frames = 2,
        frame_rate = 10,
        vertical = true,
        looped = true,
        frame_offset = 0
      },
      projectile_emitter = {
        velocity = { x = 400, y = 400 },
        repeat_frequency = 0,
        projectile_duration = 3000,
        hit_percentage_damage = 10,
        is_friendly = true
      },
        keyboard_control = {
        up_velocity = { x = 0, y = -400 },
        right_velocity = { x = 400, y = 0 },
        down_velocity = { x = 0, y = 400 },
        left_velocity = { x = -400, y = 0 }
      },
    }
  },
  [2] = {
    tag = "the_shield",
    components = {
      transform = {
        position = { x = 7600, y = 5152},
        scale = { x = 1, y = 1},
        rotation = {0}
      },
      box_collider = {
        width = 4,
        height = 4,
        offset_x = 64,
        offset_y = -96
      },
      rigidbody = {
        velocity = { x = 0, y = 0}
      },
      projectile_emitter = {
        velocity = { x = 300, y = 300 },
        repeat_frequency = 0,
        projectile_duration = 3000,
        hit_percentage_damage = 10,
        is_friendly = true
      },
      keyboard_control = {
            up_velocity = { x = 0, y = 0 },
            right_velocity = { x = 0, y = 0 },
            down_velocity = { x = 0, y = 0 },
            left_velocity = { x = 0, y = 0 }
      }
    }
  },
  [3] = {
    tag = "the_sword",
    components = {
      transform = {
        position = { x = 7600, y = 5152},
        scale = { x = 1, y = 1},
        rotation = {0}
      },
      box_collider = {
        width = 4,
        height = 4,
        offset_x = 36,
        offset_y = -96
      },
      rigidbody = {
        velocity = { x = 0, y = 0}
      },
      projectile_emitter = {
        velocity = { x = 300, y = 300 },
        repeat_frequency = 0,
        projectile_duration = 3000,
        hit_percentage_damage = 10,
        is_friendly = true
      },
      keyboard_control = {
            up_velocity = { x = 0, y = 0 },
            right_velocity = { x = 0, y = 0 },
            down_velocity = { x = 0, y = 0 },
            left_velocity = { x = 0, y = 0 }
      }
    }
  }
}