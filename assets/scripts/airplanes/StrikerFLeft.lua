local StrikerFLeft = dofile('assets/scripts/airplanes/Striker.lua')

StrikerFLeft.aiPattern = {
	{-70, 80}
}

StrikerFLeft.weapons = {
	{
		projectile = 'EnemyBullet',
		ammo = 3
	}
}

return StrikerFLeft