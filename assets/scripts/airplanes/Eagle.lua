local Eagle = { }

Eagle.Sprite = {
    texture = 'Eagle',
    rect = {0, 0, 50, 50}
}

Eagle.Airplane = {
    health = 200,
    idleSpriteRect = {0, 0, 50, 50},
    leftSpriteRect = {50, 0, 50, 50},
    rightSpriteRect = {100, 0, 50, 50},

    onDestroy = function (this)
        this:playAnimation('Explode')
    end,
    
    aiPattern = { --{angle, distance} 
        {45, 80},
        {-45, 160},
        {45, 80}
    }

    weapons = {
        {
            projectile = 'Bullet',
            ammo = 10
        },

        {
            projectile = 'HomingMissile',
            ammo = 3
        }
    }
}

return Eagle