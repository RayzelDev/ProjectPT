#include "UIRect2D.h"

#include <algorithm>

namespace UI
{
	Rect2D::Rect2D()
	{
		m_X = m_Y = m_Width = m_Height = 0;
	}

	Rect2D::Rect2D(int X, int Y, int Width, int Height)
	{
		m_X = X;
		m_Y = Y;
		m_Width = Width;
		m_Height = Height;
	}

	Rect2D::Rect2D(float X, float Y, float Width, float Height)
	{
		m_X = static_cast<int>(X);
		m_Y = static_cast<int>(Y);
		m_Width = static_cast<int>(Width);
		m_Height = static_cast<int>(Height);
	}

	Rect2D::Rect2D(const RECT& rhs)
	{
		m_X = rhs.left;
		m_Y = rhs.top;
		m_Width = rhs.right - rhs.left;
		m_Height = rhs.bottom - rhs.top;
	}

	Rect2D::Rect2D(const Rect2D& rhs)
	{
		m_X = rhs.m_X;
		m_Y = rhs.m_Y;
		m_Width = rhs.m_Width;
		m_Height = rhs.m_Height;
	}

	Rect2D& Rect2D::operator=(const Rect2D& rhs)
	{
		m_X = rhs.m_X;
		m_Y = rhs.m_Y;
		m_Width = rhs.m_Width;
		m_Height = rhs.m_Height;
		return *this;
	}

	Rect2D& Rect2D::operator=(const RECT& rhs)
	{
		m_X = rhs.left;
		m_Y = rhs.top;
		m_Width = rhs.right - rhs.left;
		m_Height = rhs.bottom - rhs.top;
		return *this;
	}

	Rect2D& Rect2D::operator*(const float& rhs)
	{
		const float X = (m_Width * rhs) - (float)m_Width;
		const float Y = (m_Height * rhs) - (float)m_Height;

		m_X -= (int)(X / 2.0f);
		m_Y -= (int)(Y / 2.0f);
		m_Width += (int)(X);
		m_Height += (int)(Y);

		return *this;
	}

	Rect2D::operator RECT() const
	{
		return { m_X, m_Y, m_X + m_Width, m_Y + m_Height };
	}

	bool Rect2D::operator==(const UI::Rect2D& r) const
	{
		return memcmp(this, &r, sizeof(UI::Rect2D)) == 0;
	}

	bool Rect2D::Contains(int X, int Y) const
	{
		if (X > m_X && X <= m_X + m_Width && Y > m_Y && Y <= m_Y + m_Height)
			return true;

		return false;
	}

	bool Rect2D::Contains(const POINT& p) const
	{
		return Contains(p.x, p.y);
	}

	bool Rect2D::Contains(const RECT& r) const
	{
		return Contains(UI::Rect2D(r));
	}

	bool Rect2D::Contains(const Rect2D& r) const
	{
		if (std::min<int>(m_X + m_Width, r.GetX() + r.GetWidth()) >=
			std::max<int>(m_X, r.GetX()) &&
			std::min<int>(m_Y + m_Height, r.GetY() + r.GetHeight()) >=
			std::max<int>(m_Y, r.GetY()))
			return true;

		return false;
	}

	void Rect2D::SetPosition(int X, int Y)
	{
		m_X = X;
		m_Y = Y;
	}

	void Rect2D::SetX(int X)
	{
		m_X = X;
	}

	void Rect2D::SetY(int Y)
	{
		m_Y = Y;
	}

	void Rect2D::SetSize(int Width, int Height)
	{
		m_Width = Width;
		m_Height = Height;
	}

	void Rect2D::SetWidth(int Width)
	{
		m_Width = Width;
	}

	void Rect2D::SetHeight(int Height)
	{
		m_Height = Height;
	}

	int Rect2D::GetX() const
	{
		return m_X;
	}

	int Rect2D::GetY() const
	{
		return m_Y;
	}

	int Rect2D::GetWidth() const
	{
		return m_Width;
	}

	int Rect2D::GetHeight() const
	{
		return m_Height;
	}

	int Rect2D::GetMaxX() const
	{
		return m_X + m_Width;
	}

	int Rect2D::GetMaxY() const
	{
		return m_Y + m_Height;
	}

	Rect2D AdjustTextureToOrigin(const Rect2D& Origin, IDirect3DTexture9* BaseTexture, bool MoveToCenter)
	{
		D3DSURFACE_DESC SurfaceDescription;

		if (!BaseTexture || FAILED(BaseTexture->GetLevelDesc(0, &SurfaceDescription)))
			return Origin;

		Rect2D AdjustRect = Origin;

		if (static_cast<int>(SurfaceDescription.Width) > AdjustRect.GetWidth() || static_cast<int>(SurfaceDescription.Height) > AdjustRect.GetHeight())
		{
			auto Aspect = static_cast<float>(SurfaceDescription.Width) / static_cast<float>(SurfaceDescription.Height);
			auto Width = static_cast<float>(AdjustRect.GetWidth());
			auto Height = static_cast<float>(AdjustRect.GetHeight());

			if (SurfaceDescription.Width > SurfaceDescription.Height)
			{
				Height = roundf(Width / Aspect);

				if (MoveToCenter)
					AdjustRect.SetY(AdjustRect.GetY() + (AdjustRect.GetHeight() - static_cast<int>(Height)) / 2);
			}
			else if (SurfaceDescription.Width < SurfaceDescription.Height)
			{
				Width = roundf(Height * Aspect);

				if (MoveToCenter)
					AdjustRect.SetX(AdjustRect.GetX() + (AdjustRect.GetWidth() - static_cast<int>(Width)) / 2);
			}

			AdjustRect.SetSize(static_cast<int>(Width), static_cast<int>(Height));
		}
		else
		{
			auto Width = static_cast<int>(SurfaceDescription.Width);
			auto Height = static_cast<int>(SurfaceDescription.Height);

			if (MoveToCenter)
				AdjustRect.SetPosition(AdjustRect.GetX() + (AdjustRect.GetWidth() - Width) / 2, AdjustRect.GetY() + (AdjustRect.GetHeight() - Height) / 2);

			AdjustRect.SetSize(Width, Height);
		}

		return AdjustRect;
	}
}