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
        this:getLevel():shakeScreen(10, 0.2)
        lthis.particleSystem.system:clearEmitters()
        lthis.fireSystem.system:clearEmitters()
    end,

    start = function (this)
        local lthis = { }
        lthis.isDamaged = false
        lthis.particleSystem = engine.ParticleSystem.new()
        lthis.particleSystem.system:setTexture(this:getLevel():getTexture('Particles'))
        lthis.emitter = thor.UniversalEmitter.new()
        lthis.emitter:setParticleTextureIndex(thor.Distributions.uintUniform(0, 4))
        lthis.emitter:setParticleScale(engine.UniformVector2fDistribution.create(1, 1.5))
        lthis.emitter:setEmissionRate(40)
        lthis.emitter:setParticleVelocity(thor.Distributions.deflect(sf.Vector2f.new(0, 200), 10))
        lthis.emitter:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(1)))

        lthis.particleSystem.system:addTextureRect(sf.IntRect.new(0, 12, 10, 7))
        lthis.particleSystem.system:addTextureRect(sf.IntRect.new(10, 12, 10, 7))
        lthis.particleSystem.system:addTextureRect(sf.IntRect.new(20, 12, 10, 7))
        lthis.particleSystem.system:addTextureRect(sf.IntRect.new(30, 12, 10, 7))
        lthis.particleSystem.system:addTextureRect(sf.IntRect.new(40, 12, 8, 7))

        local fade = thor.FadeAnimation.new(0, 0.5)
        lthis.fadeAffector = thor.AnimationAffector.new(fade)

        lthis.particleSystem.system:addAffector(lthis.fadeAffector)
        this:addChild(lthis.particleSystem)


        lthis.fireSystem = engine.ParticleSystem.new()
        lthis.fireSystem.system:setTexture(this:getLevel():getTexture('Particles'))
        lthis.fireEmitter = thor.UniversalEmitter.new()
        lthis.fireEmitter:setParticleTextureIndex(thor.Distributions.uintUniform(0, 5))
        lthis.fireEmitter:setParticleScale(engine.UniformVector2fDistribution.create(2, 2.5))
        lthis.fireEmitter:setEmissionRate(60)
        lthis.fireEmitter:setParticleVelocity(thor.Distributions.deflect(sf.Vector2f.new(0, 200), 10))
        lthis.fireEmitter:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(1)))

        lthis.fireSystem.system:addTextureRect(sf.IntRect.new(0, 19, 8, 8))
        lthis.fireSystem.system:addTextureRect(sf.IntRect.new(8, 19, 8, 8))
        lthis.fireSystem.system:addTextureRect(sf.IntRect.new(16, 19, 8, 8))
        lthis.fireSystem.system:addTextureRect(sf.IntRect.new(24, 19, 7, 8))
        lthis.fireSystem.system:addTextureRect(sf.IntRect.new(31, 19, 9, 9))
        lthis.fireSystem.system:addTextureRect(sf.IntRect.new(40, 19, 8, 9))

        lthis.fireAffector = thor.AnimationAffector.new(fade)

        lthis.particleSystem.system:addAffector(lthis.fireAffector)
        this:addChild(lthis.fireSystem)
    
        return lthis
    end,

    clean = function (lthis, this)
        lthis.fireSystem:setRemoveAfterLifetime(3)
        lthis.particleSystem:setRemoveAfterLifetime(3)
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

    weapons = {
        {
            projectile = 'Bullet',
            ammo = -1
        },

        {
            projectile = 'HomingMissile',
            ammo = 5
        }
    }
}


return Eagle