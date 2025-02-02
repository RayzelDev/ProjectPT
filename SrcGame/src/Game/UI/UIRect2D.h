#pragma once
#include "../smLib3d/smd3d.h"

namespace UI
{
	class Rect2D
	{
	public:
		Rect2D();
		Rect2D(int X, int Y, int Width, int Height);
		Rect2D(float X, float Y, float Width, float Height);
		Rect2D(const RECT& rhs);
		Rect2D(const Rect2D& rhs);
		Rect2D& operator=(const RECT& rhs);
		Rect2D& operator=(const Rect2D& rhs);
		Rect2D& operator*(const float& rhs);

		operator RECT() const;

		bool operator==(const UI::Rect2D& r) const;

		bool Contains(int X, int Y) const;
		bool Contains(const POINT& p) const;
		bool Contains(const RECT& r) const;
		bool Contains(const Rect2D& r) const;

		void SetPosition(int X, int Y);
		void SetX(int X);
		void SetY(int Y);

		void SetSize(int Width, int Height);
		void SetWidth(int Width);
		void SetHeight(int Height);

		int GetX() const;
		int GetY() const;
		int GetWidth() const;
		int GetHeight() const;

		int GetMaxX() const;
		int GetMaxY() const;

	private:
		int m_X, m_Y, m_Width, m_Height;
	};

	Rect2D AdjustTextureToOrigin(const Rect2D& Origin, IDirect3DTexture9* BaseTexture, bool MoveToCenter);
}