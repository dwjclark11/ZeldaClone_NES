----------------------------------------------------------
-- Game box Colliders
----------------------------------------------------------

colliders = {
  [1] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 832, y = 448},
        scale = { x = 5, y = 5},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 34,
        offset_x = 0,
        offset_y = 0
      }
    }
  },
  [2] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 416, y = 480},
        scale = { x = 5, y = 5},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 34,
        offset_x = 0,
        offset_y = 0
      }
    }
  },
  [3] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 672, y = 736},
        scale = { x = 5, y = 5},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 34,
        offset_x = 0,
        offset_y = 0
      }
    }
  }
},
triggers = {
  [1] = {
    group = "trigger",
    components = {
      transform = {
        position = { x = 1088, y = 448},
        scale = { x = 5, y = 5},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 34,
        offset_x = 0,
        offset_y = 0}
    },
    triggerComponent = {
      type = 2,
      transport_offset_x = 0,
      transport_offset_y = 0,
      level = 0
    }
  },
  [2] = {
    group = "trigger",
    components = {
      transform = {
        position = { x = 960, y = 224},
        scale = { x = 5, y = 5},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 34,
        offset_x = 0,
        offset_y = 0}
    },
    triggerComponent = {
      type = 2,
      transport_offset_x = 0,
      transport_offset_y = 0,
      level = 0
    }
  },
  [3] = {
    group = "trigger",
    components = {
      transform = {
        position = { x = 704, y = 224},
        scale = { x = 5, y = 5},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 34,
        offset_x = 0,
        offset_y = 0}
    },
    triggerComponent = {
      type = 2,
      transport_offset_x = 0,
      transport_offset_y = 0,
      level = 0
    }
  }
}