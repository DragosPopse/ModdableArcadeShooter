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
    fireRate = 1,
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

        local closestAirplane = nil
        local closestDistance = 99999
        local command = engine.Command.new()
        command.category = engine.GameObject.ENEMY_AIRPLANE
        command.action = function (plane, dt) 
            local distance = engine.distance(plane:getWorldPosition(), this:getWorldPosition())
            if distance < closestDistance then
                closestDistance = distance
                closestAirplane = plane
            end
        end

        this:getLevel():getEnemyAirplanesRoot():onCommand(command, 0)

        if closestAirplane then

            local actionInit = function () 
                local params = { }
                params.elapsedTime = 0
                params.timeUntilDestroy = 0
                return params
            end

            local airplaneAction = function (this, dt, params)
                params.elapsedTime = params.elapsedTime + dt
                if (params.elapsedTime > params.timeUntilDestroy) then
                    local thisPosition = this:getWorldPosition()
                    local closestAirplane = nil
                    local closestDistance = 99999
                    local command = engine.Command.new()
                    command.category = engine.GameObject.ENEMY_AIRPLANE
                    command.action = function (plane, dt)
                        if plane ~= this and not plane:isDestroyed() then
                            local distance = engine.distance(plane:getWorldPosition(), thisPosition)
                            if distance < closestDistance then
                                closestDistance = distance
                                closestAirplane = plane
                            end
                        end
                    end
                    this:getLevel():getEnemyAirplanesRoot():onCommand(command, 0)
                    if closestAirplane and closestDistance < 200 then
                        print("CLOSE ENEMY: " .. closestDistance)
                        closestAirplane:setAction(this:getAction(), function () 
                            local params = { }
                            params.elapsedTime = 0
                            params.timeUntilDestroy = 0.2
                            return params
                        end)
                    else
                        print("NO CLOSE ENEMY: " .. closestDistance)
                    end
                    this:removeAction()
                    this:damage(9999)
                end
            end

            closestAirplane:setAction(airplaneAction, actionInit)
        end
    end
}


return Gift