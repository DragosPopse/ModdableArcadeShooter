local Eagle = {
    texture = 'Eagle',
    hitpoints = 200,
    idleRect = {0, 0, 50, 50},
    leftRect = {50, 0, 50, 50},
    rightRect = {100, 0, 50, 50},
    speed = 1000,

    onDestroy = function (this)
        this:playAnimation('Explode')
    end,
    
    aiPattern = { --{angle, distance} 
        {45, 80},
        {-45, 160},
        {45, 80}
    },

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