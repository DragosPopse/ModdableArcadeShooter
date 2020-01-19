local Plane = dofile('assets/scripts/airplanes/NosedStriker.lua')

Plane.weapons = {
	{
		projectile = 'EnemyLollipop',
		ammo = 4
	}
}

Plane.drops = {
	{
		pickup = 'GiftAmmo',
		dropRate = 40
	}
}

return Plane