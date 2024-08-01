#pragma once

class State {
	
public:

	virtual void HandleEvents(float deltaTime) {}
	virtual void Update(float deltaTime) {}
	virtual void Render(float deltaTime) {}
	inline bool IsComplete() { return m_Completed; }

	virtual ~State() {}

protected:

	inline void Complete() { m_Completed = true; }

private:

	bool m_Completed = false;

};