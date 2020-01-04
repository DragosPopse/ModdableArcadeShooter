local EnemyMissile = dofile('assets/scripts/projectiles/HomingMissile.lua')

EnemyMissile.speed = 100

EnemyMissile.start = function (this)
    local lthis = { }
    lthis.timer = 0
    local smoke = engine.ParticleSystem.new()
    
    smoke.system:setTexture(this:getLevel():getTexture('Smoke'))
    local smokeEmitter = thor.UniversalEmitter.new()
    smokeEmitter:setParticleTextureIndex(thor.Distributions.uintUniform(0, 4))
    smokeEmitter:setParticleScale(engine.UniformVector2fDistribution.create(2, 5))
    smokeEmitter:setEmissionRate(40)
    smokeEmitter:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(0.2)))

    for i = 0, 4 do
        smoke.system:addTextureRect(sf.IntRect.new(i * 10, 0, 10, 10))
    end

    local fade = thor.FadeAnimation.new(0, 0.5)
    local fadeAffector = thor.AnimationAffector.new(fade)

    
    smoke:addEmitter(smokeEmitter, sf.seconds(0), 0, 22)

    smoke.system:addAffector(fadeAffector)

    this:addChild(smoke)

    return lthis
end

EnemyMissile.fixedUpdate = function (lthis, this, dt)
    local approachRate = 1000
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
    if (lthis.timer > 3) then
        this:destroy()
    end
end


return EnemyMissile