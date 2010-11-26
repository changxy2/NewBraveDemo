module("EsePhysXScene", package.seeall)

-- Behavior
function OnEntitySetFinished(self, params)
    local activate = self.ActivateOnSetFinished
    if (activate) then
        bapiPhysXBase.StartSimulating(self)
    end
end
