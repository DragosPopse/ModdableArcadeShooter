local Eagle = {
    texture = 'Eagle',
    hitpoints = 200,
    leftRect = {54 * 0, 0, 54, 62},
    idleRect = {54 * 1, 0, 54, 62},
    rightRect = {54 * 2, 0, 54, 62},
    speed = 200,

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