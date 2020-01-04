local Eagle = {
	name = 'Eagle',
    texture = 'Airplanes',
    hitpoints = 200,
    leftRect = {0, 147, 27, 47},
    idleRect = {27, 147, 47, 47},
    rightRect = {74, 147, 27, 47},
    scale = 1,
    speed = 400,
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

    onDestroy = function (this)
        --if math.random() < 0.5 then
        this:getLevel():shakeScreen(10, 0.2)
        --end
    end,

    weapons = {
        {
            projectile = 'Bullet',
            ammo = -1
        },

        {
            projectile = 'HomingMissile',
            ammo = 5
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