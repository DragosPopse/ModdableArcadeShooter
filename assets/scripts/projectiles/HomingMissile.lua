local HomingMissile = {
    texture = 'Projectiles',
    iconTexture = 'Projectiles',
    muzzleRect = {0, 57, 13, 19},
    rect = {14, 57, 9, 22},
    iconRect = {100, 0, 50, 50},

    scale = 1,
    iconScale = 2, --WIP
    muzzleScale = 1,
    damage = 1,
    fireRate = 0.05,
    speed = 1000,
    spreadAngle = 0,

    fixedUpdate = function (this, dt)
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
            planesroot = this:getLevel():getPlayerAirplane()
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

    onCollision = function (this, airplane)
        airplane:damage(this:getDamage())
        this:destroy()
    end,

    onDestroy = function (this, reason) 
        if (reason == DestroyReasons.CollidedWithEnemy) then
            this:playAnimation('Explode')
            return true --return true if you want to destroy after the animation finishes
        end

        return false --return false if you want to destroy after this function is called
    end
}




return HomingMissile