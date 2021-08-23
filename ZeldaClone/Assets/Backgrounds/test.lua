----------------------------------------------------------
-- Game box Colliders
----------------------------------------------------------

colliders = {
  [1] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 704, y = 512},
        scale = { x = 4, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 40,
        offset_x = 0,
        offset_y = 0
      }
    }
  },
  [2] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 1152, y = 416},
        scale = { x = 4, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 40,
        offset_x = 0,
        offset_y = 0
      }
    }
  },
  [3] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 832, y = 224},
        scale = { x = 4, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 40,
        offset_x = 0,
        offset_y = 0
      }
    }
  }
},
triggers = {
  [4] = {
    group = "trigger",
    components = {
      transform = {
        position = { x = 736, y = 768},
        scale = { x = 4, y = 4},
        rotation = {0}
      },
      triggerComponent = {
        type = 1,
        transport_offset_x = 0,
        transport_offset_y = 0,
        level = 0
      }
    }
  }
}