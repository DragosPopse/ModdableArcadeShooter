local Jellybean = {
    texture = 'Projectiles',
    iconTexture = 'Projectiles',
    muzzleRect = {0, 57, 13, 19},
    rects = {
        {13, 56, 10, 21},
        {23, 56, 8, 23},
        {31, 56, 8, 24}
    },
    iconRect = {0, 0, 56, 56},

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


    scale = 1.1,
    iconScale = 2,
    muzzleScale = 2,
    damage = 20,
    fireRate = 0.05,
    speed = 1000,
    spreadAngle = 5,
    ammoFont = 'Pixel',
    ammoTextSize = 30,

    start = function (this)
        return { }
    end,

    onCollision = function (lthis, this, airplane)
        airplane:damage(this:getDamage())
        this:destroy()
    end
}


return Jellybean