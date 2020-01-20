local Plane = dofile('assets/scripts/airplanes/NosedKamikaze.lua')

Plane.speed = 400

Plane.collisionDamage = 35

function Plane.start(this)
    local lthis = { }
    lthis.snowflakes = engine.ParticleSystem.new()
    lthis.snowflakes.system:setTexture(this:getLevel():getTexture('Particles'))
    local emitter = thor.UniversalEmitter.new()
    emitter:setParticleTextureIndex(thor.Distributions.uintUniform(0, 3))
    emitter:setParticleScale(engine.UniformVector2fDistribution.create(1, 1.5))
    emitter:setEmissionRate(30)
    emitter:setParticleVelocity(thor.Distributions.deflect(sf.Vector2f.new(0, 200), 4))
    emitter:setParticleLifetime(thor.TimeDistribution.new(sf.seconds(1)))

    lthis.snowflakes.system:addTextureRect(sf.IntRect.new(0, 29, 19, 19))
    lthis.snowflakes.system:addTextureRect(sf.IntRect.new(19, 32, 13, 13))
    lthis.snowflakes.system:addTextureRect(sf.IntRect.new(32, 33, 13, 11))
    lthis.snowflakes.system:addTextureRect(sf.IntRect.new(45, 29, 14, 17))

    local fade = thor.FadeAnimation.new(0, 0.5)
    local fadeAffector = thor.AnimationAffector.new(fade)

    lthis.snowflakes:addEmitter(emitter, sf.seconds(0), 0, 40)
    lthis.snowflakes.system:addAffector(fadeAffector)
    
    this:addChild(lthis.snowflakes)
    return lthis
end

function Plane.onDestroy(lthis, this)
    --if math.random() < 0.5 then
    this:getLevel():shakeScreen(10, 0.2)
    --end
end

function Plane.clean(lthis, this)
    lthis.snowflakes.system:clearEmitters()
    lthis.snowflakes:setRemoveAfterLifetime(2)
end

Plane.drops = {
    {
        pickup = 'SmallHealth',
        dropRate = 40
    },
    {
        pickup = 'GiftAmmo',
        dropRate = 20
    }
}

return Plane