local Bullet = {
    texture = 'Projectiles',
    iconTexture = 'Projectiles',
    muzzleRect = {0, 57, 13, 19},
    rect = {14, 57, 9, 22},
    iconRect = {150, 0, 50, 50},

    scale = 1,
    iconScale = 2,
    muzzleScale = 2,
    damage = 20,
    fireRate = 0.05,
    speed = 1000,
    spreadAngle = 5,

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




return Bullet