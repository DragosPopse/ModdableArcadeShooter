local StrikerFPatternLeft = dofile('assets/scripts/airplanes/Striker.lua')

StrikerFPatternLeft.aiPattern = {
	{-70,300},
	{180,1200},
	{-70,5000}
}

StrikerFPatternLeft.weapons = {
	{
		projectile = 'EnemyBullet',
		ammo = 3
	}
}


return StrikerFPatternLeft