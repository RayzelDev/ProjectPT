#pragma once
#pragma once
struct Alerta {
public:

	static Alerta* GetInstance() { static Alerta instance; return &instance; };
	void Init();
	void Draw();
	void DrawImage(int Mat, int x, int y, int w, int h);


private:
	int Imagens[5];

};

#define ALERTATELA Alerta::GetInstance()
