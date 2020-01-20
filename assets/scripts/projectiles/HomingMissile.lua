local HomingMissile = {
    texture = 'Projectiles',
    iconTexture = 'Projectiles',

    muzzleSound = {
        sound = 'Bullet',
        minPitch = 0.5,
        maxPitch = 1.12,
        minVolumeFactor = 1,
        maxVolumeFactor = 1,
    },

    destroySound = {
        sound = 'Hit',
        minPitch = 1,
        maxPitch = 1,
        minVolumeFactor = 1,
        maxVolumeFactor = 1,
    },

    muzzleRect = {0, 57, 13, 19},
    rects = {
        {49, 56, 19, 43}
    },
    iconRect = {0 + 56 * 1, 0, 56, 56},

    scale = 1.5,
    iconScale = 2, 
    muzzleScale = 5,
    damage = 200,
    fireRate = 0.5,
    speed = 1000,
    spreadAngle = 90,   

    ammoFont = 'Pixel',
    ammoTextSize = 20,

    start = function (this)
        local lthis = { }
        lthis.smoke = engine.ParticleSystem.new()
        
        lthis.smoke.system:setTexture(this:getLevel():getTexture('Particles'))
        local smokeEmitter = thor.UniversalEmitter.new()
        smokeEmitter:setParticleTextureIndex(thor.Distributions.uintUniform(0, 4))
        smokeEmitter:setParticleScale(engine.UniformVector2fDistribution.create(2, 5))
        smokeEmitter:setEmissionRate(40)
        smokeEmitter:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(0.2)))

        lthis.smoke.system:addTextureRect(sf.IntRect.new(0, 12, 10, 7))
        lthis.smoke.system:addTextureRect(sf.IntRect.new(10, 12, 10, 7))
        lthis.smoke.system:addTextureRect(sf.IntRect.new(20, 12, 10, 7))
        lthis.smoke.system:addTextureRect(sf.IntRect.new(30, 12, 10, 7))
        lthis.smoke.system:addTextureRect(sf.IntRect.new(40, 12, 8, 7))

        local fade = thor.FadeAnimation.new(0, 0.5)
        local fadeAffector = thor.AnimationAffector.new(fade)

        
        lthis.smoke:addEmitter(smokeEmitter, sf.seconds(0), 0, 22)

        lthis.smoke.system:addAffector(fadeAffector)

        this:addChild(lthis.smoke)

        return lthis
    end,

    fixedUpdate = function (lthis, this, dt)
        local approachRate = 10000
        local thisPosition = this:getWorldPosition()
        local target = nil
        local closestDistance = 999999
        local planesRoot
        local command = engine.Command.new()
        if this:isPlayerControlled() then
            command.category = engine.GameObject.ENEMY_AIRPLANE
            planesRoot = this:getLevel():getEnemyAirplanesRoot()
            command.action = function (plane, dt) 
                local planePosition = plane:getWorldPosition()
                local currentDistance = engine.distance(thisPosition, planePosition)
                if (currentDistance < closestDistance) and (planePosition.y < this:getLevel():getPlayerAirplane():getWorldPosition().y) then -- Ensures that only airplanes in front of you are targeted
                    closestDistance = currentDistance
                    target = plane
                end
            end
        else
            command.category = engine.GameObject.PLAYER_AIRPLANE
            planesRoot = this:getLevel():getPlayerAirplane()
            command.action = function (plane, dt) 
                local planePosition = plane:getWorldPosition()
                local currentDistance = engine.distance(thisPosition, planePosition)
                if currentDistance < closestDistance then
                    closestDistance = currentDistance
                    target = plane
                end
            end
        end

        

        planesRoot:onCommand(command, dt)

        if target then
            --solution taken from SFML Game Development and adapted for Lua
            local directionToTarget = engine.direction(target:getWorldPosition(), thisPosition)
            local newVelocity = sf.Vector2f.normalize(approachRate * dt * directionToTarget + this:getVelocity())
            newVelocity = this:getSpeed() * newVelocity 
            local angle = math.atan(newVelocity.y, newVelocity.x)
            this:setRotation(engine.toDegree(angle) + 90)
            this:setVelocity(newVelocity)
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
        em:setParticleVelocity(thor.Distributions.circle(sf.Vector2f.new(0, 0), 1000))
        --em:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(0.1)))
        em:setParticleLifetime(thor.Distributions.timeUniform(sf.seconds(1), sf.seconds(2)))
        em:setParticleRotation(thor.Distributions.floatUniform(0, 360))
        em:setParticleTextureIndex(thor.Distributions.uintUniform(0, 3))
        em:setParticleScale(engine.UniformVector2fDistribution.create(1, 3))
        pso:addEmitter(em, sf.seconds(0.2), 0, 0)
        pso:setRemoveAfterLifetime(3)
        pso.system:setTexture(this:getLevel():getTexture('Particles'))
        pso.system:addTextureRect(sf.IntRect.new(0, 12, 10, 7))
        pso.system:addTextureRect(sf.IntRect.new(10, 12, 10, 7))
        pso.system:addTextureRect(sf.IntRect.new(20, 12, 10, 7))
        pso.system:addTextureRect(sf.IntRect.new(30, 12, 10, 7))
        pso.system:addTextureRect(sf.IntRect.new(40, 12, 8, 7))
        

        local fade = thor.FadeAnimation.new(0.5, 0.5)

        this:getLevel():addParticles(pso)

        lthis.smoke:setRemoveAfterLifetime(3)
    end
}

return HomingMissile