#include "Player.h"
#include "Input.h"
#include "StateMachine.h"
#include "AIShipOrderStates.h"
#include "Data.h"

namespace SpaceSimNS
{

Player::Player(Input* pInput, System* pSystem) : 
	VesselController()
{
		m_pInput = pInput;

		m_pControlMap = ControlMap();
		m_pPlayerVessel = m_pEntity;
		m_pPlayerFleet = new Fleet(PLAYERFACTION, pSystem);
		m_pPlayerFleet->AddMember(this);

		m_pControlMap.emplace(0x57, ACCEL);
		m_pControlMap.emplace(0x53, BACK);
		m_pControlMap.emplace(0x51, LEFT);
		m_pControlMap.emplace(0x45, RIGHT);
		m_pControlMap.emplace(0x41, TURNCCW);
		m_pControlMap.emplace(0x44, TURNCW);
		m_pControlMap.emplace(VK_SPACE, FIRE);
		m_pControlMap.emplace(VK_TAB, TARGET);
		m_pControlMap.emplace(0x54, BESTTARGET);
		m_pControlMap.emplace(0x43, UNTARGET);

}


Player::~Player()
{
}

void Player::UpdateActions()
{
	for (std::pair<UINT, Action> control : m_pControlMap)
	{
		if (m_pInput->IsKeyDown(control.first))
		{
			m_pEntity->DoAction(control.second);
		}
		else
		{
			m_pEntity->StopAction(control.second);
		}
	}
}

void Player::HandleMessage(const Telegram& telegram)
{

}

void Player::Update(double deltaT)
{

}

void Player::SetVessel(Vessel* pVessel)
{

}

/*
HRESULT Player::DrawInSector(
	ID2D1HwndRenderTarget* renderTarget,
	const D2DRes& res,
	double interpolation,
	Space::Point2D<double>& offset)
{
	HRESULT hr = S_OK;

	D2D1_SIZE_F size = renderTarget->GetSize();

	
	Space::Point2D<double> interpPos = GetInterpOffset(interpolation) + GetPos(); 

	// Draw dust

	
	if (size.height > 0 && size.width > 0)
	{
		for (double dustY = -(int)interpPos.GetY() % (int)(size.height / 10); dustY < size.height; dustY += size.height / 10)
		{
			for (double dustX = -(int)interpPos.GetX() % (int)(size.height / 10); dustX < size.width; dustX += size.height / 10)
			{
				renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(dustX, dustY), 1, 1), res.gray);
			}
		}
	}
	
	// Draw ship
	//ShipAI::DrawInSector(renderTarget, res, interpolation, offset);

	// Draw target stuff
	if (Target() != NULL)
	{
		Space::Point2D<double> tPos = Target()->GetInterpOffset(interpolation) + Target()->GetPos();
		Space::Ellipse tEll = Target()->GetEll();
		Space::Point2D<double> sizePt = Space::Point2D<double>(-(size.width / 2), size.height / 2);

		if (tPos.InRect(interpPos + sizePt, interpPos - sizePt))
		{
			double radX = tEll.RadX();
			double radY = tEll.RadY();

			Space::Point2D<double> topLeft = tPos + Space::Point2D<double>(radX + 20, radY + 20) + offset;
			Space::Point2D<double> botRight = tPos + Space::Point2D<double>(-radX - 20, -radY - 20) + offset;

			D2D1_RECT_F reticule = D2D1::RectF(topLeft.GetX(), topLeft.GetY(), botRight.GetX(), botRight.GetY());

			if (!IsInFiringRange())
			{
				renderTarget->DrawRectangle(reticule, res.blue);
			}
			else
			{
				renderTarget->DrawRectangle(reticule, res.red);
			}
			
		}
		else
		{
			renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			double tAngle = Vector2D::ComputeAngle(interpPos, tPos);
			double lineOffset = 50;
			Point2D<double> linePoint = interpPos + Vector2D::GetTerminalFromPolar(tAngle, lineOffset) + offset;

			if (!IsInFiringRange())
			{
				renderTarget->DrawLine(linePoint.D2DPoint2F(), (tPos + offset).D2DPoint2F(), res.blue);
			}
			else
			{
				renderTarget->DrawLine(linePoint.D2DPoint2F(), (tPos + offset).D2DPoint2F(), res.red);
			}
			
		}
		
	}
	
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	

	return hr;
}
*/
}