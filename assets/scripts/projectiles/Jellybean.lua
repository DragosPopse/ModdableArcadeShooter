local Jellybean = {
    texture = 'Projectiles',
    iconTexture = 'Projectiles',
    muzzleRect = {0, 57, 13, 19},
    rects = {
        {0, 80, 7, 11},
        {7, 80, 9, 12},
        {16, 80, 10, 14},
        {26, 80, 9, 12},
        {140, 0, 7, 11},
        {131, 0, 9, 12},
        {121, 0, 10, 14},
        {112, 0, 9, 12}
    },
    iconRect = {147, 0, 56, 56},

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


    scale = 1.7,
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