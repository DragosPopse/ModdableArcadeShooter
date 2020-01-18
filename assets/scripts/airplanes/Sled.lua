local Sled = {
	name = 'Sled',
    texture = 'Airplanes',
    hitpoints = 200,
    leftRect = {0, 158, 55, 48},
    idleRect = {55, 158, 64, 40},
    rightRect = {119, 158, 55, 48},
    scale = 1,
    speed = 500,
    healthFont = 'Mont',
    healthCharSize = 24,
    score = 20,
    collisionDamage = 100,

    healthTextColor = {0, 255, 0},
    ammoTextColor = {255, 0, 0},

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
        maxScale = 7,
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

    start = function (this)
        return { } 
    end,

    onDestroy = function (lthis, this)
        --if math.random() < 0.5 then
        this:getLevel():shakeScreen(10, 0.2)
        --end
    end,

    weapons = {
        {
            projectile = 'Jellybean',
            ammo = -1
        },
        {
            projectile = 'Gift',
            ammo = 10
        }
    },  
}


return Sled