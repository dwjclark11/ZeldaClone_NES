-- TestState
TestState = { _Name = "test" }
TestState.__index = TestState

function TestState:Create()
	local this = 
	{
		--_entity = entity
		_prevState = nil
	}

	setmetatable(this, self)
	print("Created Test State")
	return this
	
end

function TestState:Enter()
	--self._prevState = state
	print("Entered the state")
end

function TestState:Exit()
end

function TestState:Update(dt)
	print("Updating State")
end

