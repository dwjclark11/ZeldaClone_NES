-- StateMachine 
StateMachine = {}
StateMachine.__index = StateMachine

function StateMachine:Create(states)
	local this = 
	{
		empty = 
		{
			Enter		= function() end,
			Exit		= function() end,
			Update		= function() end,
		},
		states = states or {},
		_Current = nil
	}

	this._Current = this.empty
	setmetatable(this, self)
	print("Create State Machine")
	return this
	
end

-- Change the state
function StateMachine:Change(stateName)
	-- Check to see if the state exists
	print("Changing State")
	assert(self.states[stateName])
	
	-- Exit the current state
	self._Current:Exit()
	-- Set current state to the desired state
	self._Current = self.states[stateName]()
	self._Current:Enter()
end

-- Update the current state
function StateMachine:Update(dt)
	self._Current:Update(dt)
end

-- Get the current state
function StateMachine:Current()
	return self._Current
end
