local HomingMissile = {
    texture = 'Projectiles',
    iconTexture = 'Projectiles',
    muzzleRect = {0, 57, 13, 19},
    rect = {14, 57, 9, 22},
    iconRect = {100, 0, 50, 50},

    scale = 1,
    iconScale = 2, 
    muzzleScale = 1,
    damage = 1,
    fireRate = 0.05,
    speed = 1000,
    spreadAngle = 90,

    start = function (lthis, this)
        lthis.timer = 0
        local smoke = engine.ParticleSystem.new()
        --local fire = engine.ParticleSystem.new()
        
        smoke.system:setTexture(this:getLevel():getTexture('Smoke'))
        local smokeEmitter = thor.UniversalEmitter.new()
        smokeEmitter:setParticleTextureIndex(thor.Distributions.uintUniform(0, 4))
        smokeEmitter:setParticleScale(engine.UniformVector2fDistribution.create(2, 5))
        smokeEmitter:setEmissionRate(40)
        smokeEmitter:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(.2)))

        for i = 0, 4 do
            smoke.system:addTextureRect(sf.IntRect.new(i * 10, 0, 10, 10))
        end

        local fade = thor.FadeAnimation.new(0, 0.5)
        local fadeAffector = thor.AnimationAffector.new(fade)
        
        smoke:addEmitter(smokeEmitter, 0, 0)
        smoke.system:addAffector(fadeAffector)

        this:addChild(smoke)
        --this:addChild(fire)
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
        else
            command.category = engine.GameObject.PLAYER_AIRPLANE
            planesRoot = this:getLevel():getPlayerAirplane()
        end

        command.action = function (plane, dt) 
            local planePosition = plane:getWorldPosition()
            local currentDistance = engine.distance(thisPosition, planePosition)
            if currentDistance < closestDistance then
                closestDistance = currentDistance
                target = plane
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
        pso.system:addEmitter(em, sf.seconds(0.2))
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

        local fade = thor.FadeAnimation.new(0.5, 0.5)

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
    return { }
end

return HomingMissile, create