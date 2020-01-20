local EnemyMissile = dofile('assets/scripts/projectiles/HomingMissile.lua')

EnemyMissile.speed = 300
EnemyMissile.spreadAngle = 0
EnemyMissile.fireRate = 3.5
EnemyMissile.damage = 30

EnemyMissile.start = function (this)
    local lthis = { }
    lthis.timer = 0
    lthis.smoke = engine.ParticleSystem.new()
    
    lthis.smoke.system:setTexture(this:getLevel():getTexture('Particles'))
    local smokeEmitter = thor.UniversalEmitter.new()
    smokeEmitter:setParticleTextureIndex(thor.Distributions.uintUniform(0, 4))
    smokeEmitter:setParticleScale(engine.UniformVector2fDistribution.create(2, 5))
    smokeEmitter:setEmissionRate(40)
    smokeEmitter:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(2)))

    lthis.smoke.system:addTextureRect(sf.IntRect.new(0, 12, 10, 7))
    lthis.smoke.system:addTextureRect(sf.IntRect.new(10, 12, 10, 7))
    lthis.smoke.system:addTextureRect(sf.IntRect.new(20, 12, 10, 7))
    lthis.smoke.system:addTextureRect(sf.IntRect.new(30, 12, 10, 7))
    lthis.smoke.system:addTextureRect(sf.IntRect.new(40, 12, 8, 7))

    local fade = thor.FadeAnimation.new(0, 1)
    local fadeAffector = thor.AnimationAffector.new(fade)

    
    lthis.smoke:addEmitter(smokeEmitter, sf.seconds(0), 0, 22)

    lthis.smoke.system:addAffector(fadeAffector)

    this:addChild(lthis.smoke)

    return lthis
end

EnemyMissile.fixedUpdate = function (lthis, this, dt)
    local approachRate = 700
    local thisPosition = this:getWorldPosition()
    local target = nil
    local closestDistance = 999999
    local player = this:getLevel():getPlayerAirplane()

    if player then
        --solution taken from SFML Game Development and adapted for Lua
        local directionToTarget = engine.direction(player:getWorldPosition(), thisPosition)
        local newVelocity = sf.Vector2f.normalize(approachRate * dt * directionToTarget + this:getVelocity())
        newVelocity = this:getSpeed() * newVelocity 
        local angle = math.atan(newVelocity.y, newVelocity.x)
        this:setRotation(engine.toDegree(angle) + 90)
        this:setVelocity(newVelocity)
    end

    lthis.timer = lthis.timer + dt
    if (lthis.timer > 7) then
        this:playDestroySound()
        this:destroy()
    end
end

function EnemyMissile.onCollision(lthis, this, airplane)
    airplane:damage(this:getDamage())
    local init = function (lthis, this) 
        local params = { }
        params.elapsedTime = 0
        params.count = 0
        params.tickDuration = 0.7
        params.ticks = 4
        params.tickDamage = 5
        lthis.fireSystem:addEmitter(lthis.fireEmitter, sf.seconds(0), 0, 25)
        return params
    end
    local action = function (lthis, this, dt, params)
        params.elapsedTime = params.elapsedTime + dt
        if params.elapsedTime > params.tickDuration then
            params.count = params.count + 1
            this:damage(params.tickDamage)
            params.elapsedTime = 0     
            if params.count == params.ticks then
                lthis.fireSystem.system:clearEmitters()
                this:removeAction()
            end
        end
    end
    
    airplane:setAction(action, init)
    this:destroy()
end


return EnemyMissile