local Sled = {
	name = 'Sled',
    texture = 'Airplanes',
    hitpoints = 200,
    leftRect = {0, 0, 55, 48},
    idleRect = {55, 0, 64, 40},
    rightRect = {119, 0, 55, 48},
    scale = 1,
    speed = 400,
    healthFont = 'Mont',
    healthCharSize = 24,
    score = 20,

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

    onDestroy = function (this)
        --if math.random() < 0.5 then
        this:getLevel():shakeScreen(10, 0.2)
        --end
    end,
    
    aiPattern = { --{angle, distance} 
        --{45, 80},
        --{-45, 80},
        {0, 80}
    },

    weapons = {
        {
            projectile = 'Jellybean',
            ammo = -1
        }
    },  
}


return Sled