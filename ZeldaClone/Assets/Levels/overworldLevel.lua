----------------------------------------------------
-- All the Entities that are in the Over World
----------------------------------------------------

level_data = {
	collectables = {
				[1] = {
			
	
	
		}
	},
	enemies = {
				[1] = {
					group = "enemies",
					components = {
						transform = {
							position = { x = 0, y = 0 },
							scale = { x = 4, y = 4 },
							rotation = 0.0
						},
						sprite = {
							assetID = "enemy_sprites",
							width = 16,
							height = 16, 
							layer = 1,
							fixed = false,
							src_rect_x = 0,
							src_rect_y = 0
						},
						rigid_body = {
							velocity = { x = 50, y = 0}
						},
						health = {
							health_percentage = 10,
						},
						projectile_emitter = {
						
						
						},
					}
				}			
			},		
		}
	