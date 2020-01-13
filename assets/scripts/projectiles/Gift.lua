local Gift = {
    texture = 'Projectiles',
    iconTexture = 'Projectiles',
    muzzleRect = {0, 57, 13, 19},
    rects = {
        {55, 56, 24, 18}
    },
    iconRect = {56, 0, 56, 56},

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


    scale = 2,
    iconScale = 2,
    muzzleScale = 2,
    damage = 200,
    fireRate = 1.2,
    speed = 400,
    spreadAngle = 0,
    ammoFont = 'Pixel',
    ammoTextSize = 30,

    start = function (this)
        local lthis = { }
        lthis.timer = 0
        if math.random() < 0.5 then
            lthis.direction = -1
        else
            lthis.direction = 1
        end

        return lthis
    end,

    fixedUpdate = function (lthis, this, dt)
        this:rotate(lthis.direction * 800 * dt)

        lthis.timer = lthis.timer + dt   
        if lthis.timer > 1.8 then
            this:playDestroySound()
            this:destroy()
        end
    end,

    onCollision = function (lthis, this, airplane)
        airplane:damage(this:getDamage())
        this:destroy()
    end,

    onDestroy = function (lthis, this)
        local pso = engine.ParticleSystem.new()
        pso:setPosition(this:getWorldPosition())
        local em = thor.UniversalEmitter.new()
        em:setEmissionRate(40)
        em:setParticleVelocity(engine.AnnulusDistribution.create(300 ,400))
        em:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(2.5)))
        em:setParticleRotation(thor.Distributions.floatUniform(0, 360))
        em:setParticleRotationSpeed(thor.Distributions.floatUniform(-800, 800))
        em:setParticleTextureIndex(thor.Distributions.uintUniform(0, 2))
        em:setParticleScale(engine.UniformVector2fDistribution.create(1, 2))
        pso:addEmitter(em, sf.seconds(0.2), 0, 0)
        pso:setRemoveAfterLifetime(3)
        pso.system:setTexture(this:getLevel():getTexture('Projectiles'))

        pso.system:addTextureRect(sf.IntRect.new(13, 56, 10, 21))
        pso.system:addTextureRect(sf.IntRect.new(23, 56, 8, 23))
        pso.system:addTextureRect(sf.IntRect.new(31, 56, 8, 24))

        this:getLevel():addParticles(pso)

        local command = engine.Command.new()
        command.category = engine.GameObject.ENEMY_AIRPLANE
        command.action = function (plane, dt) 
            plane:damage(this:getDamage()) -- Damage all enemies on the screen
        end
        this:getLevel():getEnemyAirplanesRoot():onCommand(command, 0)
    end
}


return Gift