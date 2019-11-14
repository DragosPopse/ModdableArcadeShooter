local pickup = {
    texture = 'Projectiles',
    firstRect = {50, 94, 19, 19},
    frames = 7,
    frameDuration = 0.1,
    scale = 2,

    destroyAnimation = {
        texture = 'SmokeExplosion',
        firstRect = {0, 0, 16, 16},
        frameDuration = 0.1,
        scale = 4,
        frames = 5
    },

    onPickup = function (player)
        player:addAmmo('HomingMissile', 2)
        player:getLevel():playVignetteAnimation(gl.Vec4.new(0.5, 0.3, 0, 1), 0.2, 0.8, 0.5, 0.5, 0.5, 0.5);
    end
}

return pickup