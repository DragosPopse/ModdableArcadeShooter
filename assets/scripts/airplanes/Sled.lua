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
        local lthis = { }
        lthis.isDamaged = false
        lthis.particleSystem = engine.ParticleSystem.new()
        lthis.particleSystem.system:setTexture(this:getLevel():getTexture('Particles'))
        lthis.emitter = thor.UniversalEmitter.new()
        lthis.emitter:setParticleTextureIndex(thor.Distributions.uintUniform(0, 3))
        lthis.emitter:setParticleRotation(thor.Distributions.floatUniform(0, 360))
        lthis.emitter:setParticleRotationSpeed(thor.Distributions.floatUniform(-800, 800))
        lthis.emitter:setParticleScale(engine.UniformVector2fDistribution.create(1.5, 1.7))
        lthis.emitter:setEmissionRate(5)
        lthis.emitter:setParticleVelocity(thor.Distributions.deflect(sf.Vector2f.new(0, 200), 10))
        lthis.emitter:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(1)))

        lthis.particleSystem.system:addTextureRect(sf.IntRect.new(20, 0, 12, 9))
        lthis.particleSystem.system:addTextureRect(sf.IntRect.new(33, 0, 10, 8))
        lthis.particleSystem.system:addTextureRect(sf.IntRect.new(50, 21, 10, 8))
        lthis.particleSystem.system:addTextureRect(sf.IntRect.new(60, 24, 10, 8))

        local fade = thor.FadeAnimation.new(0, 0.5)
        lthis.fadeAffector = thor.AnimationAffector.new(fade)

        lthis.particleSystem.system:addAffector(lthis.fadeAffector)
        this:addChild(lthis.particleSystem)
    
        return lthis
    end,

    onDamage = function (lthis, this)
        if (not lthis.isDamaged and this:getHealth() <= 100) then
            lthis.particleSystem:addEmitter(lthis.emitter, sf.seconds(0), 0, 25)
            lthis.isDamaged = true
        end
    end,

    onHeal = function (lthis, this)
        if (lthis.isDamaged and this:getHealth() > 100) then
            lthis.particleSystem.system:clearEmitters()
            lthis.isDamaged = false
        end
    end,

    clean = function (lthis, this)
        lthis.particleSystem:setRemoveAfterLifetime(3)
    end,

    onDestroy = function (lthis, this)
        --if math.random() < 0.5 then
        this:getLevel():shakeScreen(10, 0.2)
        lthis.particleSystem.system:clearEmitters()
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