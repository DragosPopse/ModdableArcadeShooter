local HomingMissile = { }


HomingMissile.Sprite = {
    texture = 'Projectiles',
    rect = {0, 0, 20, 20}
}

HomingMissile.Animation = {
    {
        id = 'Destroy',
        rects = {
            
        }
    }
}

HomingMissile.Projectile = {
    damage = 20,
    fireRate = 1,

    fixedUpdate = function (this, context)
        local approachRate = 200
        newVelocity = sf.Vector2f.normmalize(approachRate * context.deltaTime * TODO_targetPosition + this:getVelocity())
        newVelocity = newVelocity * this:getMaxSpeed()
        local angle = math.atan2(newVelocity.y, newVelocity.x)
        this:setRotation(engine.toDegree(angle) + 90)
        this:setVelocity(newVelocity)
    end,

    collision = function (this, airplane)
        airplane:dealDamage(this:getDamage())
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