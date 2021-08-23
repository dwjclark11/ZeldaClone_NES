----------------------------------------------------------
-- Game box Colliders
----------------------------------------------------------

colliders = {
  [1] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 960, y = 480},
        scale = { x = 3, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 38,
        offset_x = 0,
        offset_y = 0
      }
    }
  },
  [2] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 576, y = 544},
        scale = { x = 3, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 38,
        offset_x = 0,
        offset_y = 0
      }
    }
  },
  [3] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 736, y = 288},
        scale = { x = 3, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 38,
        offset_x = 0,
        offset_y = 0
      }
    }
  },
  [4] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 416, y = 288},
        scale = { x = 3, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 38,
        offset_x = 0,
        offset_y = 0
      }
    }
  },
  [5] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 1408, y = 640},
        scale = { x = 3, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 38,
        offset_x = 0,
        offset_y = 0
      }
    }
  },
  [6] = {
    group = "colliders",
    components = {
      transform = {
        position = { x = 1344, y = 384},
        scale = { x = 3, y = 4},
        rotation = {0}
      },
      boxCollider = {
        width = 36,
        height = 38,
        offset_x = 0,
        offset_y = 0
      }
    }
  }
},
triggers = {
  [7] = {
    group = "trigger",
    components = {
      transform = {
        position = { x = 864, y = 672},
        scale = { x = 3, y = 4},
        rotation = {0}
      },
      triggerComponent = {
        type = 2,
        transport_offset_x = 0,
        transport_offset_y = 0,
        level = 0
      }
    }
  },
  [8] = {
    group = "trigger",
    components = {
      transform = {
        position = { x = 608, y = 800},
        scale = { x = 3, y = 4},
        rotation = {0}
      },
      triggerComponent = {
        type = 2,
        transport_offset_x = 0,
        transport_offset_y = 0,
        level = 0
      }
    }
  },
  [9] = {
    group = "trigger",
    components = {
      transform = {
        position = { x = 1088, y = 288},
        scale = { x = 3, y = 4},
        rotation = {0}
      },
      triggerComponent = {
        type = 5,
        transport_offset_x = 0,
        transport_offset_y = 0,
        level = 0
      }
    }
  },
  [10] = {
    group = "trigger",
    components = {
      transform = {
        position = { x = 960, y = 192},
        scale = { x = 3, y = 4},
        rotation = {0}
      },
      triggerComponent = {
        type = 5,
        transport_offset_x = 0,
        transport_offset_y = 0,
        level = 0
      }
    }
  },
  [11] = {
    group = "trigger",
    components = {
      transform = {
        position = { x = 608, y = 448},
        scale = { x = 3, y = 4},
        rotation = {0}
      },
      triggerComponent = {
        type = 5,
        transport_offset_x = 0,
        transport_offset_y = 0,
        level = 0
      }
    }
  }
}