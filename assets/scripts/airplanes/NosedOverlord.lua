local NosedOverlord = {
	name = 'Nosed Overlord',
    texture = 'Airplanes',
    hitpoints = 400,
    leftRect = {0, 107, 43, 51},
    idleRect = {43, 108, 57, 47},
    rightRect = {100, 107, 43, 51},
    scale = 1,
    speed = 140,
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

    onDestroy = function (lthis, this)
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
            ammo = 2
        }
    }
}


return NosedOverlord