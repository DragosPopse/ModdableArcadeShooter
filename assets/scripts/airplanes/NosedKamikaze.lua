local NosedKamikaze = {
	name = 'Nosed Kamikaze',
    texture = 'Airplanes',
    hitpoints = 200,
    leftRect = {0, 60, 42, 47},
    idleRect = {42, 60, 45, 47},
    rightRect = {87, 60, 42, 47},
    scale = 1,
    speed = 200,
    healthFont = 'Mont',
    healthCharSize = 24,
    score = 25,
    collisionDamage = 50,

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
        this:getLevel():shakeScreen(10, 0.2)
    end,

    start = function (this)
        return { }
    end
}

    
return NosedKamikaze