local StrikerFRight = dofile('assets/scripts/airplanes/Striker.lua')

StrikerFRight.aiPattern = {
	{70, 80}
}

StrikerFRight.weapons = {
	{
		projectile = 'EnemyBullet',
		ammo = 3
	}
}

return StrikerFRight