local Projectile = dofile('assets/scripts/projectiles/EnemyLollipop.lua')

Projectile.speed = 550

function Projectile.start(this)
    local lthis = { }
    local player = this:getLevel():getPlayerAirplane()
    if player then
        local playerPosition = player:getWorldPosition()
        local projectilePosition = this:getWorldPosition()
        local direction = engine.direction(playerPosition, projectilePosition)
        local velocity = direction * this:getSpeed()
        local angle = math.atan(velocity.y, velocity.x)
        this:setVelocity(velocity)
        this:setRotation(engine.toDegree(angle) + 90)
    end

    return lthis
end



return Projectile