local Eagle = {
    texture = 'Airplanes',
    hitpoints = 150,
    leftRect = {0, 56, 42, 44},
    idleRect = {42, 56, 45, 44},
    rightRect = {87, 56, 42, 44},
    scale = 1.2,
    speed = 90,
    healthFont = 'Mont',
    healthCharSize = 24,
    score = 20,

    switchSound = {
        sound = 'Switch',
        minPitch = 1,
        maxPitch = 1,
        minVolumeFactor = 1,
        maxVolumeFactor = 1
    },

    explosionData = {
        texture = 'Explosions',
        framesPerAnimation = 10,
        numberOfAnimations = 3,
        frameSize = {64, 64},
        frameDuration = 0.1,
        minScale = 4,
        maxScale = 8,
        maxRotation = 0,
        sounds = {
            {
                sound = 'Exp1',
                minPitch = 0.7,
                maxPitch = 1.1,
                minVolumeFactor = 0.7,
                maxVolumeFactor = 1
            }
        }
    },

    onDestroy = function (this)
        --if math.random() < 0.5 then
        this:getLevel():shakeScreen(10, 0.2)
        --end
    end,
    
    aiPattern = { --{angle, distance} 
        {45, 80},
        {-45, 80}
    },

    weapons = {
        {
            projectile = 'BulletSlow',
            ammo = -1
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