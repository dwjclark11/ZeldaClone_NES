----------------------------------------------------------
-- Game box Colliders
----------------------------------------------------------

colliders = {
    [1] = {
        group = "colliders",
        components = {
            transform = {
                position = { x = 640, y = 480},
                scale = { x = 10, y = 10},
                rotation = {0}
            },
            boxCollider = {
                width = 43,
                height = 50,
                offset_x = 0,
                offset_y = 0
            }
        }
    }
}
