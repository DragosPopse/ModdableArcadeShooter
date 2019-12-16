local EnemyEagleFBase = dofile('assets/scripts/airplanes/Eagle.lua')

EnemyEagleFBase.weapons = {
	{
        projectile = 'EnemyBulletF',
        ammo = -1
    }
}

return EnemyEagleFBase