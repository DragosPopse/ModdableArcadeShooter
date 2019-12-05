local Bullet = {
    texture = 'Projectiles',
    iconTexture = 'Projectiles',
    muzzleRect = {0, 57, 13, 19},
    rects = {
        {14, 57, 9, 22},
        {22, 56, 9, 22},
        {31, 56, 9, 22}
    },
    iconRect = {0 + 56 * 2, 0, 56, 56},

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
    fireRate = 0.05,
    speed = 1000,
    spreadAngle = 5,
    ammoFont = 'Pixel',
    ammoTextSize = 30,

    start = function (lthis, this)
        
    end,

    onCollision = function (lthis, this, airplane)
        airplane:damage(this:getDamage())
        this:destroy()

        local pso = engine.ParticleSystem.new()
        pso:setPosition(this:getWorldPosition())
        local em = thor.UniversalEmitter.new()
        em:setEmissionRate(20)
        em:setParticleVelocity(thor.Distributions.circle(sf.Vector2f.new(0, 0), 20))
        em:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(0.1)))
        em:setParticleRotation(thor.Distributions.floatUniform(0, 360))
        em:setParticleTextureIndex(thor.Distributions.uintUniform(0, 3))
        em:setParticleScale(engine.UniformVector2fDistribution.create(1, 2))
        pso:addEmitter(em, sf.seconds(0.2), 0, 0)
        pso:setRemoveAfterLifetime(3)
        pso.system:setTexture(this:getLevel():getTexture('Fragments'))
        for i = 1, 4 do 
            local rect = sf.IntRect.new()
            rect.left = i * 5
            rect.top = 0
            rect.width = 5
            rect.height = 5
            pso.system:addTextureRect(rect)
        end
        this:getLevel():addParticles(pso)
    end,

    onDestroy = function (lthis, this, reason) 
        if (reason == DestroyReasons.CollidedWithEnemy) then
            this:playAnimation('Explode')
            return true --return true if you want to destroy after the animation finishes
        end

        return false --return false if you want to destroy after this function is called
    end
}


local function create() 
    return {
        elapsedTime = 0
    }
end


return Bullet, create