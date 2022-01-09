----------------------------------------------------------
-- C:\Users\dwjcl\Desktop\ZeldaClone\ZeldaClone\Assets\Levels\enemies.lua
----------------------------------------------------------

local sm = loadfile("C:/Development/ZeldaClone/ZeldaClone/Assets/Levels/StateMachine.lua")
sm()
local success, error = loadfile("C:/Development/ZeldaClone/ZeldaClone/Assets/Levels/testState.lua")

if not success then
    print("Not Successful ", error)
else
    print("Successful")
end
success()


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
      }, 
      projectile_emitter = {

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
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0
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
      }, 
      projectile_emitter = {

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
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0
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
      }, 
      projectile_emitter = {

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
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0
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
      }, 
      projectile_emitter = {

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
        width = 16,
        height = 16,
        offset_x = 0,
        offset_y = 0
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
    --  projectile_emitter = {

  --    },
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
            repeat_frequency = 0,
            projectile_duration = 6000,
            hit_percent_damage = 2,
            is_friendly = false
      },
      ai_component = {
            enemy_pos = { x = 0,  y = 0},
            enemy_type = "moblin",
      },
      on_update_script = -- This is the scripting components of the Lua files
	  {
        instance = TestState:Create(),
        states = {instance},
        gStateMachine = StateMachine:Create(states),
        --gStateMachine:Change(instance),
		[0] = 
        
		function(entity, delta_time, elapsed_time)
           
           -- gStateMachine:Update(delta_time)
			-- Change the position of the tank to follow a sine wave movement
			--local new_x = elapsed_time * 0.09
			--local new_y = 200 + (math.sin(elapsed_time * 0.001) * 50)
			--set_position(entity, new_x, new_y) -- Set the new position
		end
	 }
    }
  }
}