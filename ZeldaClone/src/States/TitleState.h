#pragma once
#include "State.h"

class TitleState : public State
{
private:
	class Game& m_Game;
	class Registry& m_Registry;
	int m_Timer;
	bool m_bScroll, m_bScrollFinished, bOffset;
	std::unique_ptr<class Timer> m_pCamMoveTimer;
	const std::string m_sTitleID{"TITLESTATE"};

private:
	void UpdateCameraMovement(class Camera& camera, const float& dt);
	void AnimateTitle();

public:
	TitleState();
	~TitleState() = default;

	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit()  override;

	virtual void ProcessEvents(SDL_Event& event) override;
	virtual std::string GetStateID() const override { return m_sTitleID; }
};