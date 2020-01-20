local EnemyBullet = dofile('assets/scripts/projectiles/Bullet.lua')

EnemyBullet.fireRate = 1.25
EnemyBullet.spreadAngle = 0
EnemyBullet.damage = 10
EnemyBullet.scale = 1.3
EnemyBullet.speed = 700

EnemyBullet.muzzleSound = {
    sound = 'Bullet',
    minPitch = 0.7,
    maxPitch = 1.2,
    minVolumeFactor = 0.5,
    maxVolumeFactor = 0.6,
}

return EnemyBullet