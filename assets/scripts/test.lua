


function ptrTest(obj)
    local command = engine.Command.new()
    local health
    command.category = engine.GameObject.ENEMY_AIRPLANE
    command.action =  function (plane, dt) 
        print("EXECUTING:")
        health = plane:getHealth()
    end

    obj:onCommand(command, 0.6)
    
    print("HEALTH: " .. health)
    
end