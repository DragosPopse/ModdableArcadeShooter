local Eagle = {
    texture = 'Eagle',
    hitpoints = 200,
    leftRect = {54 * 0, 0, 54, 62},
    idleRect = {54 * 1, 0, 54, 62},
    rightRect = {54 * 2, 0, 54, 62},
    scale = 1,
    speed = 300,
    healthFont = 'Mont',
    healthCharSize = 24,

    explosionData = {
        texture = 'Explosions',
        framesPerAnimation = 10,
        numberOfAnimations = 3,
        frameSize = {64, 64},
        frameDuration = 0.1,
        minScale = 4,
        maxScale = 7,
        maxRotation = 0
    },

    onDestroy = function (this)
        this:playAnimation('Explode')
    end,
    
    aiPattern = { --{angle, distance} 
        {45, 80},
        {-45, 80},
        {0, 80}
    },

    weapons = {
        {
            projectile = 'Bullet',
            ammo = 9999
        },

        {
            projectile = 'HomingMissile',
            ammo = 9999
        }
    },

    drops = {
        {
            pickup = 'SmallHealth',
            dropRate = 50
        },

        {
            pickup = 'HomingMissileAmmo',
            dropRate = 50
        }
    }
}


return Eagle