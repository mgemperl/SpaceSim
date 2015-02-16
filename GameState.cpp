#include "GameState.h"
#include "Universe.h"
#include "Sector.h"
#include "MessageDispatcher.h"
#include "Player.h"



namespace SpaceSimNS
{

/**************************************************/

EncounterGameState::EncounterGameState(Universe* pUniverse, 
	Player* pPlayer, 
	Sector* pSector,
	const std::vector<Fleet*>& participants) :
	GameState(pUniverse)
{
	m_pPlayer = pPlayer;
	m_pEncounterSector = pSector;
	m_Participants = participants;
}

EncounterGameState::~EncounterGameState()
{

}

void EncounterGameState::Enter()
{
	m_TickCount = 0;
	m_pEncounterSector->AddFleets(m_Participants);
	m_dTimeSinceConstUpdate = 0;
}

void EncounterGameState::Execute()
{
}

void EncounterGameState::Exit()
{
	delete m_pEncounterSector;
}

StateType EncounterGameState::GetState() const
{
	return ENCOUNTER;
}

void EncounterGameState::Update(double elapsedTime)
{
	if (m_dTimeSinceConstUpdate >= PHYS_UPDATE_RATE)
	{
		m_pEncounterSector->ConstUpdate();
		m_dTimeSinceConstUpdate = 0;
		m_TickCount++;
	}

	m_pEncounterSector->Update(elapsedTime);

	MessageDispatcher::Instance()->DispatchMessages();
}

void EncounterGameState::Render()
{
/*
	Space::Vector2D playerVelocity(*m_pPlayer->GetVelocity());
	playerVelocity.scaleBy(interp);
	Space::Point2D<double> interpOffset = playerVelocity.GetTerminal();

	D2D1_SIZE_F size = renderTarget->GetSize();
	Space::Point2D<double> screenOffset = Space::Point2D<double>(size.width / 2, size.height / 2);

	m_pPlayer->GetSector()->Draw(renderTarget, res, interp, -m_pPlayer->GetPos() + screenOffset - interpOffset);
*/
	double interp = m_dTimeSinceConstUpdate / PHYS_UPDATE_RATE;
	Space::Point2D<double> offset = -(m_pPlayer->GetEntity()->GetInterpPos(interp) -
		Space::Point2D<double>(GAME_WIDTH, GAME_HEIGHT) * 0.5);

	m_pEncounterSector->Render(interp, offset);
}

void Exit();

/**************************************************/
/*
CampaignGameState::CampaignGameState(Universe* universe) :
	GameState(universe)
{

}

CampaignGameState::~CampaignGameState()
{

}

void CampaignGameState::Enter()
{

}

void CampaignGameState::Execute()
{

}

StateType CampaignGameState::GetState() const
{
	return CAMPAIGN;
}

void CampaignGameState::Exit()
{

}

LRESULT CampaignGameState::HandleUserInput(
	UINT message, WPARAM wParam, LPARAM lParam)
{


	return S_OK;
}

HRESULT CampaignGameState::Draw(ID2D1HwndRenderTarget* renderTarget,
	const D2DRes& resources,
	double interpolation)
{
	HRESULT hr = S_OK;



	return hr;
}
*/

}