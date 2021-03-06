local Striker = {
	name = 'Nosed Striker',
    texture = 'Airplanes',
    hitpoints = 200,
    leftRect = {0, 0, 54, 60},
    idleRect = {54, 0, 56, 60},
    rightRect = {110, 0, 54, 60},
    scale = 1,
    speed = 200,
    healthFont = 'Mont',
    healthCharSize = 24,
    score = 20,
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
        this:getLevel():shakeScreen(10, 0.2)
        --end
    end,

    start = function (this)
        return { }
    end
}


return Striker