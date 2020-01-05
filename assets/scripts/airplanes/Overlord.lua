local Overlord = {
	name = 'Overlord',
    texture = 'Airplanes',
    hitpoints = 200,
    leftRect = {0, 100, 43, 47},
    idleRect = {43, 100, 57, 47},
    rightRect = {100, 100, 43, 47},
    scale = 1,
    speed = 200,
    healthFont = 'Mont',
    healthCharSize = 24,
    score = 30,
    collisionDamage = 100,

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
        this:getLevel():shakeScreen(20, 0.4)
        --end
    end,

    start = function (this)
        return { }
    end,

    weapons = {
        {
            projectile = 'EnemyMissile',
            ammo = 1
        }
    }
}


return Overlord