local StrikerFPatternRight = dofile('assets/scripts/airplanes/Striker.lua')

StrikerFPatternRight.aiPattern = {
	{70,300},
	{180,1200},
	{70,5000}
}

StrikerFPatternRight.weapons = {
	{
		projectile = 'EnemyBullet',
		ammo = 3
	}
}

return StrikerFPatternRight