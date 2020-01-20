local Lollipop = {
    texture = 'Projectiles',
    iconTexture = 'Projectiles',
    muzzleRect = {0, 57, 13, 19},
    rects = {
        {39, 56, 17, 31},
        {112, 14, 18, 31}
    },
    iconRect = {0, 0, 56, 56},

    muzzleSound = {
        sound = 'Bullet',
        minPitch = 0.5,
        maxPitch = 1.22,
        minVolumeFactor = 0.92,
        maxVolumeFactor = 1,
    },

    destroySound = {
        sound = 'Hit',
        minPitch = 1,
        maxPitch = 1,
        minVolumeFactor = 1,
        maxVolumeFactor = 1,
    },


    scale = 1.1,
    iconScale = 2,
    muzzleScale = 2,
    damage = 20,
    fireRate = 1.25,
    speed = 920,
    spreadAngle = 0,
    ammoFont = 'Pixel',
    ammoTextSize = 30,

    start = function (this)
        return { }
    end,

    onCollision = function (lthis, this, airplane)
        airplane:damage(this:getDamage())
        this:destroy()
    end,

    onDestroy = function (lthis, this)
        local pso = engine.ParticleSystem.new()
        pso:setPosition(this:getWorldPosition())
        local em = thor.UniversalEmitter.new()
        em:setEmissionRate(20)
        em:setParticleVelocity(thor.Distributions.circle(sf.Vector2f.new(0, 0), 20))
        em:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(0.1)))
        em:setParticleRotation(thor.Distributions.floatUniform(0, 360))
        em:setParticleTextureIndex(thor.Distributions.uintUniform(0, 4))
        em:setParticleScale(engine.UniformVector2fDistribution.create(1, 2))
        pso:addEmitter(em, sf.seconds(0.2), 0, 0)
        pso:setRemoveAfterLifetime(3)
        pso.system:setTexture(this:getLevel():getTexture('Particles'))

        pso.system:addTextureRect(sf.IntRect.new(0, 4, 4, 4))
        pso.system:addTextureRect(sf.IntRect.new(4, 4, 4, 4))
        pso.system:addTextureRect(sf.IntRect.new(8, 4, 4, 4))
        pso.system:addTextureRect(sf.IntRect.new(12, 4, 4, 4))
        pso.system:addTextureRect(sf.IntRect.new(16, 4, 4, 4))

        this:getLevel():addParticles(pso)
    end
}


return Lollipop