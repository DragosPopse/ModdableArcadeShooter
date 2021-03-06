local Jellybean = {
    texture = 'Projectiles',
    iconTexture = 'Projectiles',
    muzzleRect = {0, 57, 13, 19},
    rects = {
        {0, 80, 7, 11},
        {7, 80, 9, 12},
        {16, 80, 10, 14},
        {26, 80, 9, 12},
        {140, 0, 7, 11},
        {131, 0, 9, 12},
        {121, 0, 10, 14},
        {112, 0, 9, 12}
    },
    iconRect = {147, 0, 56, 56},

    muzzleSound = {
        sound = 'Jellybean',
        minPitch = 0.7,
        maxPitch = 1.1,
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


    scale = 1.7,
    iconScale = 2,
    muzzleScale = 2,
    damage = 25,
    fireRate = 0.05,
    speed = 1000,
    spreadAngle = 5,
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
        em:setParticleVelocity(thor.Distributions.circle(sf.Vector2f.new(0, 0), 70))
        em:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(0.1)))
        em:setParticleRotation(thor.Distributions.floatUniform(0, 360))
        em:setParticleTextureIndex(thor.Distributions.uintUniform(0, 4))
        em:setParticleScale(engine.UniformVector2fDistribution.create(2, 2.5))
        pso:addEmitter(em, sf.seconds(0.2), 0, 0)
        pso:setRemoveAfterLifetime(3)
        pso.system:setTexture(this:getLevel():getTexture('Particles'))

        pso.system:addTextureRect(sf.IntRect.new(0, 8, 4, 4))
        pso.system:addTextureRect(sf.IntRect.new(4, 8, 4, 4))
        pso.system:addTextureRect(sf.IntRect.new(8, 8, 4, 4))
        pso.system:addTextureRect(sf.IntRect.new(12, 8, 4, 4))
        pso.system:addTextureRect(sf.IntRect.new(16, 8, 4, 4))

        this:getLevel():addParticles(pso) 
    end
}


return Jellybean