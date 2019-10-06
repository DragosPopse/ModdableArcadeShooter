local Bullet = {
    texture = 'Projectiles',
    muzzleRect = {0, 56, 48, 40},
    rect = {49, 56, 20, 40},
    damage = 20,
    fireRate = 0.2,
    speed = 500,
    spreadAngle = 30,

    collision = function (this, airplane)
        airplane:dealDamage(this:getDamage())
        --this:destroy()
        print('Collision!!!!!!!!@$@!$!@$!@$!@')
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