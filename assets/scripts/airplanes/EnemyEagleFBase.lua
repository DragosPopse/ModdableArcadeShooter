local EnemyEagleFBase = dofile('assets/scripts/airplanes/Eagle.lua')

EnemyEagleFBase.weapons = {
	{
        projectile = 'EnemyBullet',
        ammo = -1
    }
}

EnemyEagleFBase.speed = 300

return EnemyEagleFBase